#include <cmath>
#include <csignal>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
using namespace std;

#if 0
enum class PointType {
  cartesian,
  polar
};

struct Point {
  /*
  注意构造函数的前两个参数的名称被为了a和b， 
  我们无法告诉用户a和b的值应该来自哪个坐标系。
  与使用x、y、rho和theta这种清晰的名称来传达构造意图相比，
  这显然缺乏表现力。
  */
  Point(const float a, const float b, PointType type) {
    if(type == PointType::cartesian) {
      x = a;
      y = b;
    }
    x = a * cos(b);
    y = a * sin(b);
  }
private:
  float x, y; // 严格的直角坐标表示
};
#endif

// ------------ 工厂方法


#if 0

#if 0
class Point {
public:
  /*
  上面每一个静态方法都被称为**工厂方法**。
  它所做的只是创建一个点并返回它，
  其优点是方法的名称和参数的名称清楚地传达了需要哪种坐标。
  */
  static Point Newcartesian(const float x, const float y) {
    return {x, y};
  }
  static Point Newploar(float r, float theta) {
    return {r * cos(theta), r * sin(theta)};
  }
protected:
  Point(const float x, const float y)
    : x{x}, y{y} { }
private:
  float x;
  float y;
};
#endif

#ifdef Factory

struct Point {
  float x, y;
  friend class PointFactory;
private:
  //我们希望用户只能调用PointFactory中的方法构造对象
  Point(float x, float y) 
    : x(x), y(y) { }
};

struct PointFactory {
  static Point Newcartesian(const float x, const float y) {
    return Point{x, y};
  }
  static Point Newploar(float r, float theta) {
    return Point{r * cos(theta), r * sin(theta)};
  }
};
#endif

// ------------- 内部工厂

struct Point {
private:
  Point(float x, float y) : x(x), y(y) {}

  struct PointFactory {
    static Point NewCartesian(float x, float y) { return Point{x, y}; }
    static Point NewPolar(float r, float theta) {
      return Point { r *cos(theta), r *sin(theta); };
    }
  };

public:
  float x, y;
  static PointFactory Factory;
};


#endif

// -------------- 抽象工厂

struct HotDrink {
  virtual void prepare(int volume) = 0;
};

struct HotDrinkFactory {
  virtual unique_ptr<HotDrink> make() const = 0;
};

struct Tea : HotDrink {
  void prepare(int volume) override {
    cout << "Take tea bag, boil water, pour " << volume << "ml, add some lemon"
         << endl;
  }
};
struct TeaFactory : HotDrinkFactory {
  unique_ptr<HotDrink> make() const override { return make_unique<Tea>(); }
};

struct Coffee: HotDrink {
  void prepare(int volume) override {
    cout << "Take coffie, boil water, pour " << volume << "ml, add some milk"
         << endl;
  }
};
struct CoffeeFactory : HotDrinkFactory {
  unique_ptr<HotDrink> make() const override { return make_unique<Coffee>(); }
};
class DrinkFactory {
  map<string, unique_ptr<HotDrinkFactory>> hot_factories;
  DrinkFactory() {
    hot_factories["coffee"] = make_unique<CoffeeFactory>();
    hot_factories["tea"] = make_unique<TeaFactory>();
  }
  unique_ptr<HotDrink> make_drink(const string &name) {
    auto drink = hot_factories[name]->make();
    drink->prepare(200); // oops!
    return drink;
  }
};

class DrinkWithVolumeFactory {
  map<string, function<unique_ptr<HotDrink>()>> factories;

public:
  DrinkWithVolumeFactory() {
    factories["tea"] = [] {
      auto tea = make_unique<Tea>();
      tea->prepare(200);
      return tea;
    };
    // 对应Coffee类也是类似的。
  }
  unique_ptr<HotDrink> make_drink(const string &name) {
    return factories[name]();
  }
};
