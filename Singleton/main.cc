#include <atomic>
#include <cstddef>
#include <mutex>
class Singleton {
  Singleton();
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
public:
  static Singleton* getInstance();
  static Singleton* m_instance;
};
Singleton* Singleton::m_instance = nullptr;
//非线程安全
Singleton* Singleton::getInstance() {
  if(m_instance == nullptr) {
    m_instance = new Singleton();
  }
  return m_instance;
}
//线程安全,但锁消耗过大，当对象已经创建
//再次调用getInstance属于读m_instance加锁无意义
Singleton* Singleton::getInstance() {
  Lock lock;
  if(m_instance == nullptr) {
    m_instance = new Singleton();
  }
  return m_instance;
}
//双检查锁,但由于内存读写reorder不安全
Singleton* Singleton::getInstance() {
  if(m_instance == nullptr) {   // 1
    Lock lock;
    if(m_instance == nullptr) {
      m_instance = new Singleton(); // 2
      /*
      1. 分配内存
      2. 调用构造器
      3. 将地址返回
      在reorder后指令顺序可能变为
      1. 分配内存
      2. 将地址返回
      3. 调用构造器
      在经历指令重排后，线程一执行了1 2 步骤直接将地址返回
      线程二进入getInstance函数，发现m_instance不为空直接返回
      m_instance,但是此时m_instance所指地址并没有调用构造器
      */
    }
  }
  return m_instance;
}

//c++11版本之后的跨平台实现
std::atomic<Singleton*> Singleton::m_instance;
std::mutex Singleton::m_mutex;

Singleton* Singleton::getInstance() {
  Singleton* tmp = m_instance.load(std::memory_order_relaxed);
  std::atomic_thread_fence(std::memory_order_acquire);
  if(tmp == nullptr) {
    std::lock_guard<std::mutex> lock(m_mutex);
    tmp = m_instance.load(std::memory_order_relaxed);
    if(tmp == nullptr) {
      tmp = new Singleton;
      std::atomic_thread_fence(std::memory_order_release);
    }
  }
  return tmp;
}