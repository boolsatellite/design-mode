#include <cmath>
#include <csignal>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <exception>

using namespace std;

#if 0
struct Database {
 protected:
  Database() { /* do what you need to do */
  }

 public:
  static Database& get() {
    // thread-safe in C++11
    static Database database;
    return database;
  }
  Database(Database const&) = delete;
  Database(Database&&) = delete;
  Database& operator=(Database const&) = delete;
  Database& operator=(Database&&) = delete;
};

#endif

class Database {
 public:
  virtual int get_population(const std::string& name) = 0;
};

class SingletonDatabase : public Database {
  SingletonDatabase() { /* read data from database */ }
  std::map<std::string, int> capitals;

public:
  SingletonDatabase(SingletonDatabase const &) = delete;
  void operator=(SingletonDatabase const &) = delete;
  static SingletonDatabase &get() {
    static SingletonDatabase db;
    return db;
  }

  int get_population(const std::string &name) override {
    return capitals[name];
  }
};

// struct SingletonRecordFinder {
//   int total_population(std::vector<std::string> names) {
//     int result = 0;
//     for (auto& name : names)
//       result += SingletonDatabase::get().get_population(name);
//     return result;
//   }
// };

struct ConfigurableRecordFinder {
  explicit ConfigurableRecordFinder(Database& db) : db{db} {}

  int total_population(std::vector<std::string> names) {
    int result = 0;
    for (auto& name : names) result += db.get_population(name);
    return result;
  }

  Database& db;
};