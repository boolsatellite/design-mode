#include <string>

class Memento {
  std::string state;
public:
  Memento(const std::string& s) : state(s) { }
  std::string getState() const { return state; }
  void setState(const std::string& s) { state = s; }
};

class Originator {
  std::string state;
public:
  Originator() = default;
  Memento createMomento() {
    Memento m(state);
    return m;
  }
  void setMomento(const Memento& m) { state = m.getState(); }
};

int main() {
  Originator originator;
  Memento mem = originator.createMomento(); // 存储到备忘录
  // ... originator 状态发生改变
  originator.setMomento(mem); // 从备忘录中恢复
}
