#include <iostream>
#include <vector>

class Product1 {
public:
    std::vector<std::string> parts_;
    void ListParts() const {
        std::cout << "Product parts: ";
        for (size_t i = 0; i < parts_.size(); i++) {
            if (parts_[i] == parts_.back()) {
                std::cout << parts_[i];
            } else {
                std::cout << parts_[i] << ", ";
            }
        }
        std::cout << "\n\n";
    }
};

// 【抽象构建者】
// - 为一类的构建者声明统一的构建接口：A/B/C
// - 所有具体建造者都必须实现这些接口
// - 抽象构建者关注怎么造零件，不关心按什么顺序造
class Builder {
public:
    virtual ~Builder(){}
    virtual void ProducePartA() const = 0;
    virtual void ProducePartB() const = 0;
    virtual void ProducePartC() const = 0;
};

// 【具体构建者1】
// - 继承 Builder ，重写所有构建接口
// - 内部持有一个 Product1 指针，代表当前正在造的产品
// - 负责把零件加到产品上
class ConcreteBuilder1 : public Builder {
private:
    Product1* product; // 指向当前构造的产品

public:
    ConcreteBuilder1() { this->Reset(); }
    ~ConcreteBuilder1() { delete product; }

    // 重置，构建一个空产品
    void Reset() {
        this->product = new Product1();
    }

    void ProducePartA() const override {
        this->product->parts_.push_back("PartA1");
    }

    void ProducePartB() const override {
        this->product->parts_.push_back("PartB1");
    }

    void ProducePartC() const override {
        this->product->parts_.push_back("PartC1");
    }

    // 取出当前构造完的产品，重写构建一个新的产品
    Product1* GetProduct() {
        Product1* result = this->product;
        this->Reset();
        return result;
    }
};

// 【指挥者】
// - 持有一个 Builder 指针（多态，可指向任意具体建造者）
// - 负责决定按什么顺序、构建哪些零件，即构建流程
// - 不关心每个零件的具体实现
class Director {
private:
    Builder* builder;
public:
    void set_builder(Builder* builder) {
        this->builder = builder;
    }

    // 最简配方
    void BuildMinimalViableProduct() {
        this->builder->ProducePartA();
    }

    // 全配配方
    void BuildFullFeaturedProduct() {
        this->builder->ProducePartA();
        this->builder->ProducePartB();
        this->builder->ProducePartC();
    }
};

void ClientCode(Director& director) {
    ConcreteBuilder1* builder = new ConcreteBuilder1();
    director.set_builder(builder);
    std::cout << "Standard basic product:\n";
    director.BuildMinimalViableProduct();

    Product1* p = builder->GetProduct();
    p->ListParts();
    delete p;

    std::cout << "Standard full featured product:\n";
    director.BuildFullFeaturedProduct();

    p = builder->GetProduct();
    p->ListParts();
    delete p;

    std::cout << "Custom product:\n";
    builder->ProducePartA();
    builder->ProducePartC();
    p = builder->GetProduct();
    p->ListParts();
    delete p;

    delete builder;
}

int main() {
    Director* director = new Director();
    ClientCode(*director);
    delete director;
    return 0;
}

/**
 * 【核心思想】【建造者】
 * - 将一个复杂对象的构建过程与其表示分离，使得同样的构建过程可以创建不同的表示。
 * - 建造者将"构建步骤"延迟到具体建造者中完成，"构建顺序"由指挥者决定。
 *
 * 【设计思想】
 * 1. 定义产品类，表示最终要构建出来的复杂对象
 *
 * 2. 定义抽象建造者类，声明多个纯虚构建方法（每个方法构建一个零件），
 *    用于规定构建复杂对象所需的各个步骤
 * 3. 定义具体建造者类，公有继承抽象建造者类，重写所有构建方法，
 *    并提供一个获取最终产品的方法
 *
 * 4. 定义指挥者类，持有一个抽象建造者指针，
 *    负责按特定顺序调用构建方法，完成"套餐"式的构建流程
 *
 * 5. 客户端创建具体建造者，交给指挥者使用，或直接调用建造者自定义构建，
 *    最后通过建造者取出产品，无需关心产品是如何一步步组装出来的
 *
 * 【注意】
 * - 建造者的职责是"一步步构建零件"，指挥者的职责是"决定构建顺序"。
 * - 同一个建造者可以配合不同指挥者/不同配方，构建出不同表示的产品。
 * - 建造者模式关注"一个复杂对象的构建过程"，抽象工厂关注"一族相关产品的创建"。
 *
 * 【优点】
 * - 遵循单一职责原则：构建过程与表示分离，各司其职
 * - 遵循开闭原则：新增具体建造者时，无需修改指挥者和客户端代码
 * - 可以精细控制复杂对象的构建过程，支持逐步构建
 * - 同样的构建流程可以创建不同的表示
 *
 * 【缺点】
 * - 增加新的产品类型时，需修改抽象建造者接口及所有具体建造者类
 * - 可能导致类数量膨胀
 * - 如果产品之间的差异很大，建造者模式的通用性会下降
 *
 * 【适用场景】
 * - 需要构建复杂对象，且构建过程需要独立于对象的表示时
 * - 需要按不同顺序或不同组合构建同一类对象时
 * - 希望将对象的构建与使用分离时
 *
 * 【现实例子】
 * - 客户想要买一个汉堡，但不关心汉堡是怎么一步步做出来的，
 * - 他只依赖"快餐店"（指挥者）点套餐，由厨师（具体建造者）按流程制作。
 * - 最简套餐只加牛肉饼；全配套餐加牛肉饼、芝士、生菜；也可以自己搭配。
 * 例如：
 * - 最简产品 = { PartA1 }，由 Director::BuildMinimalViableProduct 构建
 * - 全配产品 = { PartA1, PartB1, PartC1 }，由 Director::BuildFullFeaturedProduct 构建
 * - 自定义产品 = { PartA1, PartC1 }，由客户端直接调用建造者构建
 *
 */