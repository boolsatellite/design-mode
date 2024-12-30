#include <cstdlib>
#include <new>
class Form {};

class ISplitter {
public:
    virtual void split() = 0;
    virtual ISplitter* clone() = 0; // 通过clone自己来创建对象
    virtual ~ISplitter() = default;
};

class FileSplitter : public ISplitter {
public:
    void split() override {
        // ...
    }
    ISplitter* clone() override {
        return new FileSplitter(*this);
    }
};
class BinarySplitter : public ISplitter {
public:
    void split() override {
        // ...
    }
    ISplitter* clone() override {
        return new BinarySplitter(*this);
    }
};
class TxtSplitter : public ISplitter {
public:
    void split() override {
        // ...
    }
    ISplitter* clone() override {
        return new TxtSplitter(*this);
    }
};

class MainForm : public Form {
    ISplitter* prototype;   // 用于clone,而不是直接使用
public:
    MainForm(ISplitter* prototype) { // 依赖注入
        this->prototype=prototype;
    }
    void Button1_Click() {
        ISplitter* splitter = prototype->clone();    // 称为多肽new
        splitter->split();
    }
};