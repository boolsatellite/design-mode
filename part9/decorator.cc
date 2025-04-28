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

//装饰器模式允许我们在不修改原始类型(开闭原则)或导致派生类型数量激增的情况下增加现有类的功能。

// 下面省略了Square 类的实现

// struct ColoredShape : Shape {
//   Shape &shape;
//   string color;
//   ColoredShape(Shape &shape, const string &color)
//       : shape(shape), color(color) {};
//   string str() const override {
//     ostringstream oss;
//     oss << shape.str() << " has the color " << color;
//     return oss.str();
//   }
// };

// struct TransparentShape : Shape {
//   Shape &shape;
//   uint8_t transparency;

//   TransparentShape(Shape &shape, const uint8_t transparency)
//       : shape{shape}, transparency{transparency} {}

//   string str() const override {
//     ostringstream oss;
//     oss << shape.str() << " has "
//         << static_cast<float>(transparency) / 255.f * 100.f << "% transparency";
//     return oss.str();
//   }
// };

struct Shape {
  virtual string str() const = 0;
};

struct Circle : Shape {
  float radius;
  Circle() = default;
  Circle(const float radius) : radius{radius} {};
  void resize(float factor) { radius = factor; }
  string str() const override {
    ostringstream oss;
    oss << "A circle of radius " << radius;
    return oss.str();
  }
};

template <typename T> struct ColoredShape : T {
  static_assert(is_base_of<Shape, T>::value,
                "Template argument must be a Shape");
  ColoredShape() = default;
  template<typename... Args>
  ColoredShape(const string& color, Args&&... args)
    : color(color), T(std::forward<Args>(args)...) { }
  string color;
  string str() const override {
    ostringstream oss;
    oss << T::str() << "has the color" << color;
    return oss.str();
  }
};

template<typename T> struct TransparentShape : T {
  static_assert(is_base_of<Shape, T>::value,
                "Template argument must be a Shape");
  TransparentShape() = default;
  template <typename... Args>
  TransparentShape(const uint8_t t, Args... args)
      : transparency(t), T(std::forward<Args>(args)...) {}
  uint8_t transparency;
  string str() const override {
    ostringstream oss;
    oss << T::str() << " has "
        << static_cast<float>(transparency) / 255.f * 100.f << "% transparency";
    return oss.str();
  }
};

void foo() {
  ColoredShape<TransparentShape<Circle>> square;
  square.radius = 2.f;
  square.transparency = 5;
  cout << square.str();
}

struct Logger {
  function<void()> func;
  string name;
  Logger(const function<void()> &func_, const string &name)
      : func{func_}, name{name} {}
  void operator()() const {
    cout << "Entering" << name << endl;
    func();
    cout << "Exiting" << name << endl;
  }
};

template <typename Func> struct Logger2 {
  Func func;
  string name;
  Logger2(const Func &func, const string &name) : func{func}, name{name} {}
  void operator()() const {
    cout << "Entering" << name << endl;
    func();
    cout << "Exiting" << name << endl;
  }
};

template <typename Func> auto make_logger2(Func func, const string &name) {
  return Logger2<Func>{func, name}; // () = call now
}

template <typename R, typename... Args> struct Logger3 {
  function<R(Args...)> func;
  string name;

  Logger3(const function<R(Args...)> &func, const string &name)
      : func{func}, name{name} {}
  R operator()(Args... args) const {
    cout << "Entering" << name << endl;
    R result = func(args...);
    cout << "Exiting" << name << endl;
    return result; 
  }
};

int main() {
  foo();
  auto call = make_logger2([]() { cout << "Hello!" << endl; }, "HelloFunction");
  call();
}
