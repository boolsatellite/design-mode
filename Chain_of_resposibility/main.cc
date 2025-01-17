#include <string>
#include <iostream>
#include <list>

enum class RequeseType {
    REQ_HENDLER1,
    REQ_HENDLER2,
    REQ_HENDLER3
};

class Request {
    std::string description;
    RequeseType reqType;
public:
    Request(const std::string& desc , RequeseType type) 
        : description(desc) , reqType(type) { }
    RequeseType getReqType() const { return reqType; }
    const std::string& getDescription() const { return description; }
};

class ChainHandler {
    ChainHandler* nextChain;
    void sendReqestToNextHandler(const Request& req) {
        if(nextChain != nullptr) {
            nextChain->handle(req);
        }
    }
protected:
    virtual bool canHandleRequest(const Request& req) = 0;
    virtual void processRequest(const Request& req) = 0;
public:
    ChainHandler() { nextChain = nullptr; }
    void setNextChain(ChainHandler* next) { nextChain = next; }

    void handle(const Request& req) {
        if(canHandleRequest(req)) {
            processRequest(req);
        } else {
            sendReqestToNextHandler(req);
        }
    }
};

class Headler1 : public ChainHandler {
protected:
    bool canHandleRequest(const Request& req) override {
        return req.getReqType() == RequeseType::REQ_HENDLER1;
    }

    void processRequest(const Request& req) override {
        std::cout << "Headler1 is handle reqest" << std::endl;
    }
};

class Headler2 : public ChainHandler {
protected:
    bool canHandleRequest(const Request& req) override {
        return req.getReqType() == RequeseType::REQ_HENDLER2;
    }

    void processRequest(const Request& req) override {
        std::cout << "Headler2 is handle reqest" << std::endl;
    }
};

class Headler3 : public ChainHandler {
protected:
    bool canHandleRequest(const Request& req) override {
        return req.getReqType() == RequeseType::REQ_HENDLER3;
    }

    void processRequest(const Request& req) override {
        std::cout << "Headler3 is handle reqest" << std::endl;
    }
};

int main() {
    Headler1 h1;
    Headler2 h2;
    Headler3 h3;
    h1.setNextChain(&h2);
    h2.setNextChain(&h3);
    Request req("process task", RequeseType::REQ_HENDLER3);
    h1.handle(req);
}