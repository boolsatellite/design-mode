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

#ifdef PROPERTY_PROXY
template<typename T>
struct Property {
  T value;
  Property(const T initial_value) { *this = initial_value; }
  operator T() { return value; }
  T operator= (T new_value) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return value = new_value;
  }
};

struct Creature {
  Property<int> strength{10};
  Property<int> agility{5};
};

void foo() {
  Creature creatrure;
  creatrure.agility = 20;
  auto x = creatrure.strength;
}
#endif

#ifdef VIRTUAL_PROXY
struct Image {
  virtual void draw() = 0;
};

struct Bitmap : Image {
  Bitmap(const string &filename) {
    cout << "Loading image from " << filename << endl;
  }
  void draw() override { cout << "Drawing image " << endl; }
};
//我们想要的是那种只在使用draw()方法时加载自身的位图。
//我们可以构建一个虚拟代理，它将聚合原来的位图

struct LazyBitmap : Image {
  LazyBitmap(const string filename) : filename(filename) {}
  ~LazyBitmap() { delete bmp; } // 如果*bmp没有先被new出来，这里就会出错

  void draw() override {
    if (!bmp)
      bmp = new Bitmap(filename);
    bmp->draw();
  }

private:
  Bitmap *bmp{nullptr};
  string filename;
};
#endif

struct Pingable {
  virtual string ping(const string& message) = 0;
};
struct Pong : Pingable {
  string ping(const string& message) {
    return message + "pong";
  }
};

int main() {
}
/*
与装饰器模式不同，代理不会尝试通过添加新成员来扩展对象的功能（除非它无能为力）。
它试图做的只是增强现有成员的潜在行为。
- 属性代理是替代对象，可以在分配和/或访问期间替换字段并执行附加操作。
- 虚拟代理提供对底层对象的虚拟访问，并且可以实现延迟对象加载等行为。
  你可能觉得你正在处理一个真实的对象，但底层实现可能尚未创建，例如，可以按需加载。
- 通信代理允许我们更改对象的物理位置（例如，将其移动到云端），
  但允许我们使用几乎相同的 `API`。
  当然，在这种情况下，`API` 只是远程服务（如 REST API）的一个垫片（`shim`）。
- 日志代理除了调用底层函数之外，还可以执行日志记录。
*/