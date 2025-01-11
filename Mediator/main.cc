#include <iostream>

using namespace std;

class Colleague;

//定义一个中介者接口，包含对象改变所需调用函数。
class Mediator {
public :
    virtual ~Mediator() = default;
    virtual void changed(Colleague *)=0;
};

//定义"同事"类接口，初始化需要一个中介者对象，并通过该类更新另外一个"同事"
class Colleague
{
public :
    //初始化中介者类对象
  Colleague(Mediator *mediator) { this->mediator = mediator; }
  // 更新另外一个类
  virtual void changed() { mediator->changed(this); }
private:
    Mediator *mediator;
};

//具体的同事类1
class ConcreteColleague1 :public Colleague
{
public:
  ConcreteColleague1(Mediator *mediator) : Colleague(mediator) {}
  void update() {
    cout << "update ConcreteColleague1 from ConcreteColleague2" << endl;
  }
};
//具体的同事类2
class ConcreteColleague2 :public Colleague
{
public :
  ConcreteColleague2(Mediator *mediator) : Colleague(mediator) {}
  void update() {
    cout << "update ConcreteColleague2 from ConcreteColleague1" << endl;
  }
};

//具体的中介者类，实现更新函数changed。
class ConcreteMediator :public Mediator {
    ConcreteColleague1 * colleague1;
    ConcreteColleague2 * colleague2;
public:
  void setColleague1(ConcreteColleague1 *colleague) { colleague1 = colleague; }
  void setColleague2(ConcreteColleague2 *colleague) { colleague2 = colleague; }
  ConcreteMediator() {
    // colleague1 = new ConcreteColleague1(this);
  }
  ~ConcreteMediator() {}
  virtual void changed(Colleague *colleague) {
    if (colleague == colleague1) {
      colleague2->update();
    } else if (colleague == colleague2) {
      colleague1->update();
    } else
      ;
  }
};
int main()
{
    ConcreteMediator concreteMediator;
    ConcreteColleague1  colleague1(&concreteMediator);
    ConcreteColleague2  colleague2(&concreteMediator);
    concreteMediator.setColleague1(&colleague1);
    concreteMediator.setColleague2(&colleague2);
    //"同事1"通过中介者更新"同事2"
    colleague1.changed();
    //"同事2"通过中介者更新"同事1"
    colleague2.changed();
}