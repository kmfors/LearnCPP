#include <memory>
#include <iostream>


// 抽象产品类：所有具体产品类都需要实现该类下的接口（operation）
class Product {
public:
    virtual ~Product() {}

    // 核心业务
    virtual std::string operation() const = 0;
};

// 具体产品类A
class ConcreteProductA : public Product {
public:
    std::string operation() const override {
        return "{ Result of ConcreteProduct_A }";
    }
};

// 具体产品类B
class ConcreteProductB : public Product {
public:
    std::string operation() const override {
        return "{ Result of ConcreteProduct_B }";
    }
};

//#############################################################################

// 抽象创建者，声明了工厂方法和核心业务逻辑
class Creator {
public:
    virtual ~Creator() {}

    // 纯虚工厂方法：由具体子类实现，返回对应的具体产品实例
    virtual std::unique_ptr<Product> FactoryMethod() const = 0;

    // 核心业务逻辑：业务逻辑与具体产品解耦
    std::string someOperation() const {
        // 调用工厂方法动态创建具体产品实例（运行时多态）
        std::unique_ptr<Product> concreteProduct = this->FactoryMethod();
        std::string result = "Creator: The same creator's code has just worked with "
            + concreteProduct->operation();
        return result;
    }
};

// 具体创建者A
class ConcreteCreatorA : public Creator {
public:
    std::unique_ptr<Product> FactoryMethod() const override {
        return std::make_unique<ConcreteProductA>();
    }
};

// 具体创建者B
class ConcreteCreatorB : public Creator {
public:
    std::unique_ptr<Product> FactoryMethod() const override {
        return std::make_unique<ConcreteProductB>();
    }
};

//#############################################################################

// 客户端代码：通过传入具体创建者实例进行工作，不感知具体创建者类型，实现运行时的灵活切换
void ClientCode(const Creator& creator) {
    std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
        << creator.someOperation() << std::endl;
}

// 在实际应用中，通常根据配置或环境变量来选择具体的创建者类型。
int main() {
    std::cout << "App: Launched with the ConcreteCreator_A.\n";
    std::unique_ptr<Creator> c1 = std::make_unique<ConcreteCreatorA>();
    ClientCode(*c1);

    std::cout << std::endl;

    std::cout << "App: Launched with the ConcreteCreator_B.\n";
    std::unique_ptr<Creator> c2 = std::make_unique<ConcreteCreatorB>();
    ClientCode(*c2);

    return 0;
}

/**
 * 【核心思想】【工厂方法】
 * - 定义一个用于创建对象的接口，让子类决定实例化哪一个具体类。
 * - 工厂方法将对象的实例化延迟到子类中完成。
 *
 * 【设计思想】
 * 1. 定义抽象产品类，通过纯虚函数声明产品应具备的业务操作
 * 2. 定义具体产品类，公有继承抽象产品类，并实现其所有纯虚函数
 *
 * 3. 定义抽象创建者类，声明纯虚工厂方法（返回抽象产品指针），
 *    并可包含一个依赖于工厂方法返回产品的业务逻辑方法（如 someOperation）
 * 4. 定义具体创建者类，公有继承抽象创建者类，重写工厂方法以返回具体产品实例
 *
 * 5. 客户端仅依赖抽象创建者基类指针或引用，通过该接口调用业务逻辑，
 *    无需关心具体产品和具体创建者的类型，实现解耦
 *
 * 【注意】
 * - 创建者的主要职责并非创建产品，而是包含依赖于工厂方法所返回产品对象的
 *   核心业务逻辑。子类通过重写工厂方法来改变返回的具体产品类型，从而
 *   间接影响业务逻辑的行为，实现解耦。
 *
 * 【优点】
 * - 遵循开闭原则：新增产品时，通常只需新增具体产品和具体创建者，无需修改客户端代码
 * - 遵循单一职责原则：将产品创建逻辑集中在具体创建者中
 *
 * 【缺点】
 * - 每增加一种产品类型，通常就需要增加一个具体创建者类，可能导致类数量膨胀
 *
 * 【适用场景】
 * - 当无法预知对象的确切类型及其依赖关系时
 * - 希望将对象的创建与使用分离，使代码更易于扩展和维护时
 *
 * 【现实例子】
 * 客户想要购买椅子，但不关心是现代椅子还是古典椅子，他只依赖“椅子工厂”接口，由具体工厂决定实际创建哪一种椅子。
 * 
 */