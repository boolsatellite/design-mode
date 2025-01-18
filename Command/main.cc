#include <string>
#include <iostream>
#include <list>
#include <vector>

class Command {
public:
    virtual void execute() = 0;
};

class ConcreteCommand1 : public Command {
    std::string arg;
public:
    ConcreteCommand1(const std::string& a) : arg(a) { }
    void execute() override {
        std::cout << "#1 process..." << arg << std::endl;
    }
};

class ConcreteCommand2 : public Command {
    std::string arg;
public:
    ConcreteCommand2(const std::string& a) : arg(a) { }
    void execute() override {
        std::cout << "#2 process..." << arg << std::endl;
    }
};

class MacroCommand : public Command {
    std::vector<Command*> commands;
public:
    void addCommand(Command* c) {
        commands.push_back(c);
    }
    void execute() override {
        for(auto const& c : commands) {
            c->execute();
        }
    }
};
