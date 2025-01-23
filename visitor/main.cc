#include <string>
#include <iostream>
#include <list>
#include <vector>

class ElementA;
class ElementB;

class Visitor {
public:
    virtual void visitElementA(ElementA& element) = 0;
    virtual void visitElementB(ElementB& element) = 0;
    virtual ~Visitor() = default;
};

class Element {
public:
    //预知未来会添加新操作,但不知道添加的具体方法
    virtual void accept(Visitor& visitor) = 0;  
    virtual ~Element() { }
};

class ElementA : public Element {
public:
    void accept(Visitor& visitor) override {
        visitor.visitElementA(*this);
    }
};

class ElementB : public Element {
public:
    void accept(Visitor& visitor) override {
        visitor.visitElementB(*this);
    }
};


// ==============
class Visitor1 : public Visitor {
public:
    void visitElementA(ElementA& element) override {
        std::cout << "Visitor1 is processing ElementA" << std::endl;
    }
    void visitElementB(ElementB& element) override {
        std::cout << "Visitor1 is processing ElementB" << std::endl;
    }
};

class Visitor2 : public Visitor {
public:
    void visitElementA(ElementA& element) override {
        std::cout << "Visitor2 is processing ElementA" << std::endl;
    }
    void visitElementB(ElementB& element) override {
        std::cout << "Visitor2 is processing ElementB" << std::endl;
    }
};

int main() {
    Visitor2 visitor;
    ElementB element;
    element.accept(visitor);
}