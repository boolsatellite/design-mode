class Messager {
protected:
    MessagerImpl* messageImpl; //运行时确定是pcMessagerBase or mobileMessagerBase
public:
    Messager(MessagerImpl* impl) : messageImpl(impl) {}
    virtual void Login(string username , string password) = 0;
    virtual void SendMessage(string message) = 0;
    virtual void SendPicture(Image image) = 0;
    virtual ~Messager() = default;
};

class MessagerImpl {
    virtual void PlaySound() = 0;
    virtual void DrawShape() = 0;
    virtual void WriteText() = 0;
    virtual ~MessagerImpl() = default;
};

//平台实现
class PCMessagerBase : public MessagerImpl {
public:
    virtual void PlaySound() override {}
    virtual void DrawShape() override {}
    virtual void WriteText() override {}
};

class MobileMessagerBase : public MessagerImpl {
public:
    virtual void PlaySound() override {}
    virtual void DrawShape() override {}
    virtual void WriteText() override {}
};

//业务抽象
class MessagerLite : Messager  {  // 精简版
public:
    MessagerLite(MessagerImpl* impl) : Messager(impl) {}
    virtual void Login(string username , string password) override {
        messageImpl->Connect();
        //...
    }
    virtual void SendMessage(string message) override {
        message->WriteText();
        //...
    }
    virtual void SendPicture(Image image) override {
        messageImpl->DrawShape();
        //...
    }
};

class MessagerPerfect : Messager {   // 完美版
public:
    MessagerPerfect(MessagerImpl* impl) : Messager(impl) {}
    virtual void Login(string username , string password) override {
        message->PlaySound();
        //...
        message->Connect();
    }
    virtual void SendMessage(string message) override {
        message->PlaySound();
        message->WriteText();
        //...
    }
    virtual void SendPicture(Image image) override {
        message->PlaySound();
        message->DrawShape();
        //...
    }
};

void process() {
    MessagerImpl* impl = new PCMessagerBase;
    Messager* message = new MessagerLite(impl);
}