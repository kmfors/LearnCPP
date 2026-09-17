#include <iostream>
#include <string>
#include <unordered_map>

using std::string;

// 【产品类型枚举】
enum Type {
    PROTOTYPE_1 = 0,
    PROTOTYPE_2
};

// 【抽象原型】
// - 声明统一的克隆接口 Clone()
// - 所有具体原型都必须实现该接口，返回自身的拷贝
// - 抽象原型关注“怎么克隆”，不关心具体是什么对象
class Prototype {
protected:
    string prototype_name_;   // 原型名称，用于标识当前原型
    float prototype_field_;   // 原型内部状态字段，克隆后会被复制

public:
    Prototype() {}
    Prototype(string prototype_name)
        : prototype_name_(prototype_name) {

    }

    virtual ~Prototype() {}

    // 纯虚克隆接口，由具体原型实现
    virtual Prototype* Clone() const = 0;

    // 普通业务方法，用于演示克隆后的对象仍保留原有状态
    virtual void Method(float prototype_field) {
        this->prototype_field_ = prototype_field;
        std::cout << "Call Method from " << prototype_name_
            << " with field : " << prototype_field << std::endl;
    }
};

// 【具体原型1】
// - 继承 Prototype，重写 Clone()
// - 内部持有自己的状态字段 concrete_prototype_field1_
// - 通过拷贝构造自身完成克隆，返回一个全新的对象
class ConcretePrototype1 : public Prototype {
private:
    float concrete_prototype_field1_;

public:
    ConcretePrototype1(string prototype_name, float concrete_prototype_field)
        : Prototype(prototype_name), concrete_prototype_field1_(concrete_prototype_field) {

    }

    // 克隆自身：用 *this 调用拷贝构造，产生一个新对象
    Prototype* Clone() const override {
        return new ConcretePrototype1(*this);
    }
};

// 【具体原型2】
// - 与 ConcretePrototype1 结构相同，但持有不同的状态字段
// - 体现“同一克隆接口，不同具体表示”
class ConcretePrototype2 : public Prototype {
private:
    float concrete_prototype_field2_;

public:
    ConcretePrototype2(string prototype_name, float concrete_prototype_field)
        : Prototype(prototype_name), concrete_prototype_field2_(concrete_prototype_field) {

    }

    Prototype* Clone() const override {
        return new ConcretePrototype2(*this);
    }
};

// 【原型工厂】
// - 持有一个映射表，把 Type 枚举映射到预创建的原型对象
// - 对外提供 CreatePrototype()，返回原型的克隆体而非原型本身
// - 客户端通过工厂获取新对象，无需知道具体原型类
class PrototypeFactory {
private:
    std::unordered_map<Type, Prototype*, std::hash<int>> prototypes_;

public:
    // 构造时预先创建好各种原型对象，作为“克隆模板”
    PrototypeFactory() {
        prototypes_[Type::PROTOTYPE_1] = new ConcretePrototype1("PROTOTYPE_1", 50.f);
        // 注意：此处按设计意图应为 ConcretePrototype2
        prototypes_[Type::PROTOTYPE_2] = new ConcretePrototype2("PROTOTYPE_2", 60.f);
    }

    // 析构时释放所有原型对象
    ~PrototypeFactory() {
        delete prototypes_[Type::PROTOTYPE_1];
        delete prototypes_[Type::PROTOTYPE_2];
    }

    // 根据类型找到原型，并返回它的克隆体
    Prototype* CreatePrototype(Type type) {
        return prototypes_[type]->Clone();
    }
};

// 【客户端】
// - 只依赖抽象 Prototype 和 PrototypeFactory
// - 通过工厂获取克隆对象，调用业务方法，最后释放
// - 不关心具体原型类，也不关心克隆的具体实现
void Client(PrototypeFactory& prototype_factory) {
    std::cout << "Let's create a Prototype 1\n";

    Prototype* prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_1);
    prototype->Method(90);
    delete prototype;

    std::cout << "\nLet's create a Prototype 2 \n";

    prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_2);
    prototype->Method(10);

    delete prototype;
}

int main() {
    // 创建原型工厂（内部持有预定义原型）
    PrototypeFactory* prototype_factory = new PrototypeFactory();
    // 客户端通过工厂获取克隆对象并使用
    Client(*prototype_factory);
    // 释放工厂及其内部原型
    delete prototype_factory;

    return 0;
}

/**
 * 【核心思想】【原型】
 * - 用原型实例指定创建对象的种类，并且通过拷贝这些原型创建新的对象。
 * - 原型将“创建新对象”的职责交给对象自身，通过 Clone() 复制自己，
 *   而不是由客户端直接调用具体类的构造函数。
 *
 * 【设计思想】
 * 1. 定义抽象原型类，声明纯虚克隆接口 Clone()，
 *    规定所有具体原型都必须提供“复制自身”的能力
 *
 * 2. 定义具体原型类，公有继承抽象原型类，重写 Clone()，
 *    在 Clone() 中通过拷贝构造自身返回一个全新的对象
 *
 * 3. 定义原型工厂类，内部持有一组预创建的原型对象，
 *    对外提供按类型获取克隆体的接口，客户端无需知道具体原型类
 *
 * 4. 客户端通过工厂获取克隆对象，调用业务方法，
 *    克隆体拥有与原原型相同的初始状态，且修改克隆体不影响原型
 *
 * 5. 新增原型时，只需新增具体原型类并在工厂中注册，
 *    无需修改客户端代码，符合开闭原则
 *
 * 【注意】
 * - 原型的职责是“复制自身”，工厂的职责是“管理原型并提供克隆入口”。
 * - 克隆分为浅拷贝和深拷贝：若对象持有指针/资源，需自行实现深拷贝。
 * - 原型模式关注“通过复制创建对象”，抽象工厂关注“创建一族相关产品”。
 *
 * 【优点】
 * - 隐藏对象创建的复杂性，客户端无需知道具体类
 * - 可以在运行时动态添加或删除原型
 * - 克隆比频繁调用构造函数更高效，尤其当对象初始化代价大时
 * - 避免与具体产品类耦合，符合开闭原则
 *
 * 【缺点】
 * - 每个具体原型都必须实现 Clone()，类数量可能膨胀
 * - 若对象包含循环引用或复杂资源，深拷贝实现较复杂
 * - 使用裸指针时需手动管理内存，容易泄漏
 *
 * 【适用场景】
 * - 需要创建的对象类型在运行时才能确定时
 * - 对象的创建过程复杂、初始化代价大，希望避免重复构造时
 * - 需要避免与具体产品类耦合，只依赖抽象接口时
 * - 希望动态地注册、删除原型时
 *
 * 【现实例子】
 * - 细胞分裂：一个细胞通过复制自身产生新的细胞，新细胞保留原有特征。
 * - 文档模板：复制一份已有文档作为新文档的起点，再按需修改。
 * - 游戏中的怪物生成：预先配置好各种怪物原型，需要时克隆一份放入场景。
 * 例如：
 * - PROTOTYPE_1 克隆体 = ConcretePrototype1 的副本，初始状态为 { name: "PROTOTYPE_1", field: 50 }
 * - PROTOTYPE_2 克隆体 = ConcretePrototype2 的副本，初始状态为 { name: "PROTOTYPE_2", field: 60 }
 * - 客户端对克隆体调用 Method(90) / Method(10)，只修改克隆体自身状态，不影响原型
 *
 */