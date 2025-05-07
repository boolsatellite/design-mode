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

#if 1
struct BankAccount {
  friend class BankAccountCommand;
  int balance = 0;
  int overdraft_limit = -500;
private:
  void deposit(int amount) {
    balance += amount;
    cout << "deposited " << amount << ", balance is now " << balance << "\n";
  }

  bool withdraw(int amount) {
    if (balance - amount >= overdraft_limit) {
      balance -= amount;
      cout << "withdrew " << amount << ", balance is now " << balance << "\n";
      return true;
    }
    return false;
  }
};
#endif

struct Command {
  virtual void call() = 0;
  virtual void undo() = 0;
};
struct BankAccountCommand : Command {
  BankAccount& account;
  enum Action { deposit, withdraw } action;
  int amount;
  bool withdrawal_succeded = false;
  BankAccountCommand(BankAccount& account, const Action action, const int amount) 
    :account(account), action(action), amount(amount) { }
  void call() override {
    switch (action) {
    case deposit:
      account.deposit(amount);
      break;
    case withdraw:
      withdrawal_succeded = account.withdraw(amount);
      break;
    default:
      break;
    }
  }
  void undo() override {
    switch (action) {
    case withdraw:
    if(withdrawal_succeded) {
      account.deposit(amount);
    }
    break;
    case deposit:
      account.withdraw(amount);
      break;
    }
  }
};

struct CompositeBankAccountCommand : vector<BankAccountCommand>, Command {
  CompositeBankAccountCommand(const initializer_list<value_type> &items)
      : vector<BankAccountCommand>(items) {}
  void call() override {
    for (auto &cmd : *this)
      cmd.call();
  }
  void undo() override {
    for (auto it = rbegin(); it != rend(); ++it)
      it->undo();
  }
};

enum class CreatureAbility { strength, agility };

struct CreatureCommand {
  enum Action { set, increaseBy, decreaseBy } action;
  CreatureAbility ability;
  int amount;
};

struct CreatureQuery {
  CreatureAbility ability;
};

class Creature {
  int strength, agility;

public:
  Creature(int strength, int agility) : strength{strength}, agility{agility} {}

  void process_command(const CreatureCommand &cc) {
    int *ability;
    switch (cc.ability) {
    case CreatureAbility::strength:
      ability = &strength;
      break;
    case CreatureAbility::agility:
      ability = &agility;
      break;
    }
    switch (cc.action) {
    case CreatureCommand::set:
      *ability = cc.amount;
      break;
    case CreatureCommand::increaseBy:
      *ability += cc.amount;
      break;
    case CreatureCommand::decreaseBy:
      *ability -= cc.amount;
      break;
    }
  }

  int process_query(const CreatureQuery &q) const {
    switch (q.ability) {
    case CreatureAbility::strength:
      return strength;
    case CreatureAbility::agility:
      return agility;
    }
  }
    };