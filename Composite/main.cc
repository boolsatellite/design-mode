#include <string>
#include <list>

// 树节点
class Component {
public:
  virtual void process() = 0;
  virtual ~Component() = default;
};

class Composite : public Component {
  std::string name;
  std::list<Component*> elements;
public:
  Composite(const std::string& s) : name(s) { }
  void add(Component* element) {
    elements.push_back(element);
  }
  void remove(Component* element) {
    elements.remove(element);
  }
  void process() override {
    // 1. process cur nodes
    // 2. process leaf nodes
    for(auto& e : elements) {
      e->process();
    }
  }
};

// 叶子节点
class Leaf : public Component {
  std::string name;
public:
  Leaf(std::string s) : name(s) { }
  void process() override {
    // process current node
  }
};

int main() {
  Composite root("root");
  Composite treeNode1("treeNode1");
  Composite treeNode2("treeNode2");
  Composite treeNode3("treeNode3");
  Composite treeNode4("treeNode4");
  Leaf left1("left1");
  Leaf left2("left2");

  root.add(&treeNode1);
  treeNode1.add(&treeNode2);
  treeNode2.add(&left1);
  root.add(&treeNode3);
  treeNode3.add(&treeNode4);
  treeNode4.add(&left2);

  root.process();
}