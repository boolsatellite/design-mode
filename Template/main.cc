// 库开发者
// struct Library {
//   void step1() {}
//   void step3() {}
//   void step5() {}
// };

// // 应用开发人员
// struct Application {
//   bool step2() {}
//   void step4() {}
// };

// int main() {
//   Library lib;
//   Application app;
//   lib.step1();
//   if(app.step2()) { lib.step3(); }
//   for(int i=0 ; i<4 ; ++i) {
//     app.step4();
//   }
//   return 0;
// }

// 库开发者
struct Library {
  void Run() {
    step1();
    if(step2()) {step3();} 
    for(int i=0 ; i<4 ; ++i) {
      step4();
    }
    step5();
  }
  virtual ~Library() {}
protected:
  void step1() {} // 稳定
  void step3() {} // 稳定
  void step5() {} // 稳定
  virtual bool step2() = 0; // 变化
  virtual void step4() = 0; // 变化
};

// 应用开发人员
struct Application : Library {
  bool step2() override {}
  void step4() override {}
};

int main() {
  Library* pLib = new Application;
  pLib->Run();
  delete pLib;
  return 0;
}