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
#include <boost/bimap.hpp>

using namespace std;

typedef uint32_t key;
struct User {
  User(const string &first_name, const string &last_name)
      : first_name{add(first_name)}, last_name{add(last_name)} {}
protected:
  key first_name, last_name;
  static boost::bimap<key, string> names;
  static key seed;
  static key add(const string &s) {
    auto it = names.right.find(s);
    if (it == names.right.end()) {
      names.insert({++seed, s});
      return seed;
    }
    return it->second;
  }
  string get_first_name() const {
    return names.left.at(first_name);
  }
  string get_last_name() const {
    return names.left.at(last_name);
  }
  friend ostream &operator<<(ostream &os, const User &obj) {
    return os << "first_name: " << obj.get_first_name()
              << " last_name: " << obj.get_last_name();
  }
};