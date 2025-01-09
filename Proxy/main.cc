class ISubject {
public:
    virtual void process() = 0;
};

class SubjectProxy : public ISubject {
    virtual void process() {
        // 对RealSubject的间接访问
    }

};

class ClientApp {
    ISubject* subject;
public:
    ClientApp() {
        subject = new SubjectProxy;
    }
    void DoTask() {
        //...
        subject->process();
        //...
    }
};