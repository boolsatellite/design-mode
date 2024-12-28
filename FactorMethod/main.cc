#include <cstdlib>
#include <new>
class Form {};

class ISplitter {
public:
    virtual void split() = 0;
    virtual ~ISplitter() {}
};

class SplitterFactory {
public:
    virtual ISplitter* CreateSplitter() = 0;
    virtual ~SplitterFactory() {}
};

class FileSplitter : public ISplitter {
public:
    void split() override {
        // ...
    }
};
class BinarySplitter : public ISplitter {
public:
    void split() override {
        // ...
    }
};
class TxtSplitter : public ISplitter {
public:
    void split() override {
        // ...
    }
};

class FileSplitterFactory : SplitterFactory{
public:
    ISplitter* CreateSplitter() override {
        return new FileSplitter;
    }
};

class BinarySplitterFactory : SplitterFactory{
public:
    ISplitter* CreateSplitter() override {
        return new BinarySplitter;
    }
};

class TxtSplitterFactory : SplitterFactory{
public:
    ISplitter* CreateSplitter() override {
        return new BinarySplitter;
    }
};

class MainForm : public Form {
    SplitterFactory* factory;
public:
    MainForm(SplitterFactory* factory) { // 依赖注入
        this->factory =factory;
    }
    void Button1_Click() {
        ISplitter* splitter = factory->CreateSplitter();    // 称为多肽new
        splitter->split();
    }
};