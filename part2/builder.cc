#include <csignal>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
using namespace std;

#if 0
struct HtmlElement {
  string name;
  string text;
  vector<HtmlElement> elements;
  HtmlElement() {}
  HtmlElement(const string &name, const string &text)
      : name(name), text(text) {}

  string str(int indent = 0) const {
    // pretty-print the contents
  }
};
#endif

#ifdef OOP

void foo() {
  string words[] = {"hello", "world"};
  HtmlElement list{"ul", ""};
  for (auto w : words) {
    list.elements.emplace_back(HtmlElement{"li", w});
  }
  printf(list.str().c_str());
}

int main() {
  foo();
}
#endif

#ifdef ezBuilder
struct HtmlBuilder {
  HtmlElement root;

  HtmlBuilder(string root_name) { root.name = root_name; }

  void add_child(string child_name, string child_text) {
    HtmlElement e{child_name, child_text};
    root.elements.emplace_back(e);
  }

  string str() { return root.str(); }
};

void foo() {
  HtmlBuilder builder{"ul"};
  builder.add_child("li", "hello");
  builder.add_child("li", "world");
  cout << builder.str() << endl;
}
#endif

#ifdef fluent_builder
struct HtmlBuilder {
  HtmlElement root;

  HtmlBuilder(string root_name) { root.name = root_name; }

  //通过返回对建造者本身的引用，现在可以在建造者进行链式调用。
  //这就是所谓的流畅接口(`fluent interface`):
  HtmlBuilder& add_child(string child_name, string child_text) {
    HtmlElement e{child_name, child_text};
    root.elements.emplace_back(e);
    return *this;
  }

  string str() { return root.str(); }
};

void foo() {
  HtmlBuilder builder{"ul"};
  builder.add_child("li", "hello")
         .add_child("li", "world");
  cout << builder.str() << endl;
}
#endif

struct HtmlBuilder;

struct HtmlElement {
  friend struct HtmlBuilder;
  string name;
  string text;
  vector<HtmlElement> elements;
  const size_t indent_size = 2;
  static unique_ptr<HtmlBuilder> build(const string &root_name) {
    return make_unique<HtmlBuilder>(root_name);
  }

protected: // hide all constructors
  HtmlElement() {}
  HtmlElement(const string &name, const string &text)
      : name{name}, text{text} {}
};

struct HtmlBuilder {
  HtmlElement root;

  HtmlBuilder(string root_name) { root.name = root_name; }

  //通过返回对建造者本身的引用，现在可以在建造者进行链式调用。
  //这就是所谓的流畅接口(`fluent interface`):
  HtmlBuilder& add_child(string child_name, string child_text) {
    HtmlElement e{child_name, child_text};
    root.elements.emplace_back(e);
    return *this;
  }
  HtmlElement build() const {
    return root;
  }

  // string str() { return root.str(); }
};


// ---------------------

struct Tag {
  std::string name;
  std::string text;
  std::vector<Tag> children;
  std::vector<std::pair<std::string, std::string>> attributes;

protected:
  Tag(const std::string &name, const std::string &text)
      : name{name}, text{text} {}
  Tag(const std::string &name, const std::vector<Tag> &children)
      : name{name}, children{children} {}

  // friend std::ostream &operator<<(std::ostream &os, const Tag &tag) {
  //   // implementation omitted
  // }
};

struct P : Tag {
  explicit P(const std::string& text) : Tag{"p", text} {}

  P(std::initializer_list<Tag> children) : Tag("p", children) {}
};
struct IMG : Tag {
  explicit IMG(const std::string& url) : Tag{"img", ""} {
    attributes.emplace_back(std::pair<string,string>{"src", url});
  }
};

// --------------------
class PersonBuilderBase;
class PersonAddressBuilder;
class PersonJobBuilder;
class PersonBuilder;

struct Person {
  static PersonBuilder create();
  // address
  std::string street_address, post_code, city;

  // employment
  std::string company_name, position;
  int annual_income = 0;
  Person() {}
};

class PersonBuilderBase {
protected:
  Person &person;   // 正在构建的对象的引用
  explicit PersonBuilderBase(Person &person) : person{person} {}
public:
  operator Person() { return std::move(person); }
  // builder facets
  PersonAddressBuilder lives() const;
  PersonJobBuilder works() const;
};

class PersonBuilder : public PersonBuilderBase {
  Person p; // object being built
public:
  PersonBuilder() : PersonBuilderBase{p} {}
};

class PersonAddressBuilder : public PersonBuilderBase {
  typedef PersonAddressBuilder self;

 public:
  explicit PersonAddressBuilder(Person& person) : PersonBuilderBase{person} {}
  self& at(std::string street_address) {
    person.street_address = street_address;
    return *this;
  }
  self& with_postcode(std::string post_code){
    person.post_code = post_code;
    return *this;
  }
  self& in(std::string city) {
    person.city = city;
    return *this;
  }
};
class PersonJobBuilder : public PersonBuilderBase {
public:
  typedef PersonJobBuilder self;
  PersonJobBuilder(Person& p) : PersonBuilderBase(p) { }
  self& at(std::string company_name) {
    person.company_name = company_name;
    return *this;
  }
  self& as_a(std::string position) {
    person.position= position;
    return *this;
  }
  self& earning(int annual_income) {
    person.annual_income = annual_income;
    return *this;
  }
};
PersonAddressBuilder PersonBuilderBase::lives() const {
  return PersonAddressBuilder{person};
}
PersonJobBuilder PersonBuilderBase::works() const {
  return PersonJobBuilder(person);
}

PersonBuilder Person::create() {
  return PersonBuilder{};
}

void foo() {
  Person p = Person::create()
                 .lives()
                 .at("123 London Road")
                 .with_postcode("SW1 1GB")
                 .in("London")
                 .works()
                 .at("PragmaSoft")
                 .as_a("Consultant")
                 .earning(10e6);
}