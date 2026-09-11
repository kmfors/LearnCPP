#include <memory>
#include <string>
#include <iostream>

class AbstractProductA{
public:
    virtual ~AbstractProductA(){};
    virtual std::string functionA() const = 0;
};

class ConcreteProductA1 : public AbstractProductA{
public:
    std::string functionA() const override {
        return "The result of the product A1.";
    }
};

class ConcreteProductA2 : public AbstractProductA{
public:
    std::string functionA() const override {
        return "The result of the product A2.";
    }
};

class AbstractProductB{
public:
    virtual ~AbstractProductB(){};
    virtual std::string functionB() const = 0;
    virtual std::string anotherFunctionB(const AbstractProductA& collaborator) const = 0;
};

class ConcreteProductB1 : public AbstractProductB{
public:
    std::string functionB() const override {
        return "The result of the product B1.";
    }
    std::string anotherFunctionB(const AbstractProductA& collaborator) const override {
        const std::string result = collaborator.functionA();
        return "The result of the B1 collaborating with ( " + result + " )";
    }
};

class ConcreteProductB2 : public AbstractProductB{
public:
    std::string functionB() const override {
        return "The result of the product B2.";
    }
    std::string anotherFunctionB(const AbstractProductA& collaborator) const override {
        const std::string result = collaborator.functionA();
        return "The result of the B2 collaborating with ( " + result + " )";
    }
};

class AbstractFactory {
public:
    virtual ~AbstractFactory(){};
    virtual std::unique_ptr<AbstractProductA> createProductA() const = 0;
    virtual std::unique_ptr<AbstractProductB> createProductB() const = 0;
};

class ConcreteFactory1 : public AbstractFactory {
public:
    std::unique_ptr<AbstractProductA> createProductA() const override {
        return std::make_unique<ConcreteProductA1>();
    }
    std::unique_ptr<AbstractProductB> createProductB() const override {
        return std::make_unique<ConcreteProductB1>();
    }
};

class ConcreteFactory2 : public AbstractFactory {
public:
    std::unique_ptr<AbstractProductA> createProductA() const override {
        return std::make_unique<ConcreteProductA2>();
    }
    std::unique_ptr<AbstractProductB> createProductB() const override {
        return std::make_unique<ConcreteProductB2>();
    }
};

void ClientCode(const AbstractFactory& factory) {
    auto product_a = factory.createProductA();
    auto product_b = factory.createProductB();

    std::cout << product_b->functionB() << "\n";
    std::cout << product_b->anotherFunctionB(*product_a) << "\n";
}

int main() {
    std::cout << "Client: Testing client code with the first factory type:\n";
    auto f1 = std::make_unique<ConcreteFactory1>();
    ClientCode(*f1);
    std::cout << std::endl;

    std::cout << "Client: Testing client code with the second factory type:\n";
    auto f2 = std::make_unique<ConcreteFactory2>();
    ClientCode(*f2);
}

/**
 * 【核心思想】【抽象工厂】
 * - 提供一个创建一系列相关或相互依赖对象的接口，而无需指定它们具体的类。
 * - 抽象工厂将一组产品（产品族）的创建延迟到具体工厂中完成。
 *
 * 【设计思想】
 * 1. 定义抽象产品类，通过纯虚函数声明产品应具备的业务操作
 * 2. 定义具体产品类，公有继承抽象产品类，并实现其所有纯虚函数
 *
 * 3. 定义抽象工厂类，声明多个纯虚工厂方法（每个方法返回一个抽象产品指针），
 *    用于创建一族相关产品
 * 4. 定义具体工厂类，公有继承抽象工厂类，重写所有工厂方法以返回同一产品族的具体产品实例
 *
 * 5. 客户端仅依赖抽象工厂基类指针或引用，通过该接口创建产品并调用其业务逻辑，
 *    无需关心具体产品和具体工厂的类型，实现解耦
 *
 * 【注意】
 * - 抽象工厂的职责是创建一族相关产品，而不是单个产品。
 * - 同一具体工厂创建出的产品属于同一产品族，彼此可以协作。
 * - 工厂方法关注“一个产品等级结构”，抽象工厂关注“多个产品等级结构”。
 *
 * 【优点】
 * - 遵循开闭原则：新增产品族时，通常只需新增具体工厂和具体产品，无需修改客户端代码
 * - 遵循单一职责原则：将一族产品的创建逻辑集中在具体工厂中
 * - 保证产品族内产品的一致性
 *
 * 【缺点】
 * - 增加新产品类型时，需修改抽象工厂接口及所有具体工厂类，违反开闭原则
 * - 可能导致类数量膨胀
 *
 * 【适用场景】
 * - 需要创建一系列相关或相互依赖的产品对象时
 * - 希望将产品的创建与使用分离时
 * - 需要保证客户端始终使用同一产品族中的产品时
 *
 * 【现实例子】
 * - 客户想要购买一套家具，但不关心是现代风格还是古典风格，
 * - 他只依赖“家具工厂”接口，由具体工厂决定实际创建哪一套家具。
 * - 现代家具工厂生产现代椅子、现代沙发；古典家具工厂生产古典椅子、古典沙发。
 * 例如：
 * - 产品族 1 = { A1, B1 }，由 ConcreteFactory1 生产
 * - 产品族 2 = { A2, B2 }，由 ConcreteFactory2 生产
 * - 产品族 = 同一个具体工厂生产出来的一组产品。
 *
 * 【智能指针补充】
 * - 智能指针表达“所有权”，借用（不涉及所有权）用 T& / const T& / T*。
 * - createProductA()/createProductB() 返回 unique_ptr，因为工厂将新建对象的所有权转移给客户端。
 * - anotherFunctionB(const AbstractProductA&) 接收 const 引用，因为只是借用协作，不涉及所有权。
 * - ClientCode(const AbstractFactory&) 接收 const 引用，因为只是借用工厂创建产品，不涉及所有权。
 */