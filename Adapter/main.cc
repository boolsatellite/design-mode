class ITarget {     // 目标接口(新接口)
public:
    virtual void process() = 0;
};

class IAdaptee {    // 遗留接口(老接口)
public:
    virtual void foo(int data) = 0;
    virtual int bar() = 0;
};

class Adapter : public ITarget {    // 适配器
protected:
    IAdaptee* pAdaptee;     // 组合了老接口
public:
    virtual void process() {
        //...
        int data = pAdaptee->bar();
        pAdaptee->foo(2);
        //...
    }
};