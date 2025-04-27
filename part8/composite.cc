#include <algorithm>
#include <cmath>
#include <csignal>
#include <functional>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <exception>

using namespace std;

#if 0
class Creature {
  int strength, agility, intelligence;
public:
  int get_strength() const { return strength; }
  void set_strength(int strength) { strength = strength; }

  int get_agility() const { return agility; }
  void set_agility(int agility) { agility = agility; }

  int get_intelligence() const { return intelligence; }
  void set_intelligence(int intelligence) { intelligence = intelligence; }

  int sum() const { return strength + agility + intelligence; }
  double average() const { return sum() / 3.0; }
  int max() const { return ::max(::max(strength, agility), intelligence); }
};
#endif

/*
想象一下如果再增加一个新的属性，这个时候我们需要对sum()，average()，max()重构，这是十分糟糕的
*/

#if 0
class Creature {
  enum Abilities { strength, agility, intelligence, count };
  // 上面的枚举定义了一个名为count的额外值，标记着有多少个属性
  array<int, count> abilities;
  int get_strength() const { return abilities[strength]; }
  void set_strength(int value) { abilities[strength] = value; }
  int get_agility() const { return abilities[agility]; }
  void set_agility(int agility) { abilities[agility] = agility; }

  int get_intelligence() const { return abilities[intelligence]; }
  void set_intelligence(int intelligence) {
    abilities[intelligence] = intelligence;
  }

  int sum() const {
    return accumulate(abilities.begin(), abilities.end(), 0);
  }
  double average() const {
    return sum() / (double)count;
  }
  int max() const {
    return *max_element(abilities.begin(), abilities.end());
  }
};
#endif

#ifdef ezCombine
struct GraphicObject {
  virtual void draw() = 0;
};

struct Circle : GraphicObject {
  void draw() override { std::cout << "Circle" << std::endl; }
};

struct Group : GraphicObject {
  std::string name;
  explicit Group(const std::string &name) : name(name) {}
  void draw() override {
    std::cout << "Group" << name.c_str() << " contains:" << std::endl;
    for (auto &&o : objects)
      o->draw();
  }
  std::vector<GraphicObject *> objects;
};
#endif

struct Neuron {
  vector<Neuron *> in, out;
  unsigned int id;
  Neuron() {
    static int id = 1;
    this->id = id++;
  }
  template <typename T> void connect_to(Neuron &);
  template <> void connect_to<Neuron>(Neuron &other) {
    out.push_back(&other);
    other.in.push_back(this);
  }
};

struct NeuronLayer : vector<Neuron> {
  NeuronLayer(int count) {
    while (count-- > 0)
      emplace_back(Neuron{});
  }
};

template <typename Self> struct SomeNeurons {
  template <typename T> void connect_to(T &other) {
    for (Neuron &from : *static_cast<Self *>(this)) {
      for (Neuron &to : other) {
        from.out.push_back(&to);
        to.in.push_back(&from);
      }
    }
  }
};
