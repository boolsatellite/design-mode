#include <cmath>
#include <csignal>
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <exception>

using namespace std;

#ifdef pimpl

struct Person {
  std::string name;
  void greet() { impl->greet(this); }
  Person() : impl(new PersonImpl) { }
  ~Person() { delete impl; }

  struct PersonImpl {
    void greet(Person* p) {
      stringstream ss;
      ss << "hello" << p->name.c_str();
      std::cout << ss.str();
    }
  };
  PersonImpl *impl; // good place for gsl::owner<T>
  /*
   更大比例的类的实现被隐藏起来。如果`Person`类的实现需要提供许多私有/受保护成员，
   那么你将向客户端公开所有这些细节，即使客户端由于私有/受保护访问修饰符永远无法访问这些成员。
   使用`Pimpl`编程技法，可以只提供公共接口。
   - 修改隐藏`Impl`类的数据成员不会影响二进制兼容性。
   头文件只需要包含声明所需的头文件，而不需要包含实现。
   例如，如果`Person`需要`vector<string>`类型的私有成员，
   您将被迫在头文件`Person.h`种`#include` `<vector>`和`<string>` 
   (这是传递性的，所以任何使用Person.h的人也会包括他们)。
   利用Pimpl编程技法，可以在`.cpp`文件中`#include` `<vector>`和`<string>` 。
  */
};

#endif

struct Renderer {
  virtual void render_circle(float x, float y, float radius) = 0;
};

struct VectorRenderer : Renderer {
  void render_circle(float x, float y, float radius) override {
    cout << "Rasterizing circle of radius " << radius << endl;
  }
};

struct RasterRenderer : Renderer {
  void render_circle(float x, float y, float radius) override {
    cout << "Drawing a vector circle of radius " << radius << endl;
  }
};

struct Shape {
protected:
  Renderer &renderer;
  Shape(Renderer &renderer) : renderer{renderer} {}
public:
  virtual void draw() = 0;
  virtual void resize(float factor) = 0;
};

struct Circle : Shape {
  float x, y, radius;
  void draw() override { renderer.render_circle(x, y, radius); }
  void resize(float factor) override { radius = factor; }
  Circle(Renderer &renderer, float x, float y, float radius)
      : Shape{renderer}, x{x}, y{y}, radius{radius} {}
};

