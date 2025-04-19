#include <csignal>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

#ifdef OCP
enum class Color { Red, Green, Blue };
enum class Size { Small, Medium, Large };

struct Product {
  string name;
  Color color;
  Size size;
};

#ifdef SRP
struct ProductFilter {
  typedef vector<Product *> Items;
  Items by_color(Items items, Color color);
  Items by_size(Items items, Size size);
  Items by_color_and_size(Items items, Size size, Color color);
};

ProductFilter::Items ProductFilter::by_color(Items items, Color color) {
  Items result;
  for (auto &i : items)
    if (i->color == color)
      result.push_back(i);
  return result;
}

ProductFilter::Items ProductFilter::by_size(Items items, Size size) {
    Items result;
    for (auto& i : items)
        if (i->size == size)
            result.push_back(i);
    return result;
}

ProductFilter::Items ProductFilter::by_color_and_size(Items items, Size size,
                                                      Color color) {
  Items result;
  for (auto &i : items)
    if (i->size == size && i->color == color)
      result.push_back(i);
  return result;
}
#else 
/*
我们从概念上（SRP!）将我们的过滤过程分为两部分：
筛选器（接受所有项并且只返回某些项的过程）和规范（应用于数据元素的谓词的定义）
*/

template <typename T> struct AndSpecification;

template <typename T> struct Specification {
  virtual bool is_satisfied(T *item) = 0;
  AndSpecification<T> operator&& (Specification&& other) {
    return AndSpecification<T>(*this, other);
  }
};
template<typename T> struct Filter {
  virtual vector<T*> filter(vector<T*> items, Specification<T>& spec) = 0;
};
struct BetterFilter : Filter<Product> {
  virtual vector<Product *> filter(vector<Product *> items,
                                   Specification<Product> &spec) override {
    vector<Product*> result;
    for(auto& p : items) {
      if(spec.is_satisfied(p)) {
        result.push_back(p);
      }
    }
    return result;
  }
};

template <typename T> struct AndSpecification : Specification<T> {
  Specification<T> &first;
  Specification<T> &second;
  AndSpecification(Specification<T> &first, Specification<T> &second)
      : first(first), second(second) {}
  bool is_satisfied(T *item) override {
    return first.is_satisfied(item) && second.is_satisfied(item);
  }
};

struct ColorSpecification : Specification<Product> {
  Color color;
  explicit ColorSpecification(const Color color) : color(color) { }
  bool is_satisfied(Product* item) override {
    return item->color == color;
  }
};

struct SizeSpecification : Specification<Product> {
  Size size;
  explicit SizeSpecification(const Size size) : size(size) { }
  bool is_satisfied(Product* item) override {
    return item->size== size;
  }
};

int main() {
  Product apple{"Apple", Color::Green, Size::Small};
  Product tree{"Tree", Color::Green, Size::Large};
  Product house{"House", Color::Blue, Size::Large};

  vector<Product *> all{&apple, &tree, &house};

  BetterFilter bf;
  ColorSpecification green(Color::Green);

  auto green_things = bf.filter(all, green);
  for (auto &x : green_things) {
    cout << x->name << " is green" << endl;
  }

  // --------------------------------------

  SizeSpecification large(Size::Large);
  // ColorSpecification green(Color::Green);
  AndSpecification<Product> green_and_large{large, green};

  // auto green_and_big =
  // ColorSpecification(Color::Green) && SizeSpecification(Size::Large);
  auto big_green_things = bf.filter(all, green_and_large);
  for (auto &x : big_green_things) {
    cout << x->name << " is large and green" << endl;
  }
}
#endif
#endif

#ifndef LSP
/*如果一个接口可以接受类型为 Parent 的对象，
那么它应该同样地可以接受类型为 Child 的对象，而不会有任何破坏。让我们来看看 LSP 被破坏的情况。
*/
class Rectangle {
protected:
  int width, height;
public:
  Rectangle(const int width, const int height) : width{width}, height{height} {}
  int get_width() const { return width; }
  int get_height() const { return height; }
  int area() const { return width * height; }
  virtual void set_height(const int height) { this->height = height; }
  virtual void set_width(const int width) { this->width = width; }
};

class Square : public Rectangle {
public:
  Square(int size) : Rectangle(size, size) {}
  void set_width(const int width) override { this->width = height = width; }
  void set_height(const int height) override { this->height = width = height; }
};

void process(Rectangle &r) {
  int w = r.get_width();
  r.set_height(10);
  cout << "expected area = " << (w * 10) << ", got " << r.area() << endl;
}

int main() {
  Square s{5};
  process(s);
}

#endif