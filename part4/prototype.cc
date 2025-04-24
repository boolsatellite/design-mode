#include <cmath>
#include <csignal>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using namespace std;

struct Address {
  string street;
  string city;
  int suite;
  Address(string s, string c, int su) : street(s), city(c), suite(su) { }
private:
  friend class boost::serialization::access;
  template <class Ar> void serialize(Ar &ar, const unsigned int version) {
    ar & street;
    ar & city;
    ar & suite;
  }
};

struct Contact {
  friend class EmployeeFactory;
  string name;
  Address *address = nullptr;
  Contact(string n, Address* a) : name(n), address(a) { }
private:
  friend class boost::serialization::access;
  template <class Ar> void serialize(Ar &ar, const unsigned int version) {
    ar & name;
    ar & address; // no *
  }
};

// auto Clone = [](const Contact& c) {
//   // 1. Serialize the contact
//   ostringstream oss;
//   boost::archive::text_oarchive oa(oss);
//   oa << c;
//   string s = oss.str();
//   // 2. Deserialize the contact
//   istringstream iss(oss.str());
//   boost::archive::text_iarchive ia(iss);
//   Contact result;
//   ia >> result;
//   return result;
// };

struct EmployeeFactory {
  static Contact Main;
  static Contact aux;
  static unique_ptr<Contact> NewMainOfficeEmployee(string name, int suite) {
    return NewEmployee(name, suite, Main);
  }

  static unique_ptr<Contact> NewAuxOfficeEmployee(string name, int suite) {
    return NewEmployee(name, suite, aux);
  }

 private:
  static unique_ptr<Contact> NewEmployee(string name, int suite,
                                         Contact& proto) {
    auto result = make_unique<Contact>(proto);
    result->name = name;
    result->address->suite = suite;
    return result;
  }
};

Contact EmployeeFactory::Main{"", new Address{"123 East Dr", "London", 0}};
Contact EmployeeFactory::aux{"", new Address{"123B East Dr", "London", 0}};

void foo() {
  auto john = EmployeeFactory::NewAuxOfficeEmployee("John Doe", 123);
  auto jane = EmployeeFactory::NewMainOfficeEmployee("Jane Doe", 125);
}


/*
原型设计模式阐释了对象*深度*拷贝的概念，
并不需要每次都通过构造函数完整初始化来创建一个对象，
可以对创建好的对象进行复制，复制产生的对象和原来的对象互不依赖，
稍加修改后就能得到想要的新对象。


- 在代码中正确的复制对象，即进行深拷贝。可以在拷贝构造函数/拷贝赋值运算符或单独的成员函数中实现。
- 在代码中支持序列化/反序列化，序列化后再进行反序列化实现拷贝。该方法需要额外的计算开销，
  拷贝频率越高，开销越大。该方法相对于拷贝构造函数的唯一优点是能复用已有的序列化代码。
*/
int main() {

}