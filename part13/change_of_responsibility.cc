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
struct Creature {
  string name;
  int attack, defense;
  // 构造函数和<<运算符
};

class CreatureModifier {
  CreatureModifier *next{nullptr};
protected:
  Creature &creature; // 可选：指针或共享指针
public:
  explicit CreatureModifier(Creature &creature) : creature(creature) {}
  void add(CreatureModifier *cm) {
    if (next)
      next->add(cm);
    else
      next = cm;
  }
  virtual void handle() {
    if (next)
      next->handle(); // 关键之处！
  }
};

class DoubleAttackModifier : public CreatureModifier {
public:
  explicit DoubleAttackModifier(Creature &creature)
      : CreatureModifier(creature) {}

  void handle() override {
    creature.attack *= 2;
    CreatureModifier::handle();
  }
};

class DoubleAttackModifier2 : public CreatureModifier {
public:
  explicit DoubleAttackModifier2(Creature &creature)
      : CreatureModifier(creature) {}

  void handle() override {
    if (creature.attack <= 2)
      creature.attack += 1;
    CreatureModifier::handle();
  }
};
#endif

struct Game { // 中介者
  signal<void(Query &)> queries;
};

struct Query {
  string creature_name;
  enum Argument { attack, defense } argument;
  int result;
};

class Creature {
  Game &game;
  int attack, defense;
public:
  string name;
  Creature(Game &game, ...) : game{game}, ... { ... }
  // 其他成员函数
  int get_attack() const {
    Query q{name, Query::Argument::attack, attack};
    game.queries(q);
    return q.result;
  }
};

class CreatureModifier: {
    Game& game;
    Creature& creature;
    public:
        CreatureModifier(Game& game, Creature& creature) :
            game(game), creature(creature) 
        {}
};

class DoubleAttackModifier : public CreatureModifier {
  connection conn;

public:
  DoubleAttackModifier(Game &game, Creature &creature)
      : CreatureModifier(game, creature) {
    conn = game.queries.connect([&](Query &q) {
      if (q.creature_name == creature.name &&
          q.argument == Query::Argument::attack)
        q.result *= 2;
    });
  }
  ~DoubleAttackModifier() { conn.disconnect(); }
};
