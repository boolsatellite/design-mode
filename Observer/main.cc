#include <string>
using namespace std;
class Form {};
class TextBox;

class Iprogress {     // 观察者抽象接口
public:
  virtual void DoProgress(float value) = 0;
  virtual ~Iprogress() {}
};

class FileSplitter {    // 主体
  string m_filePath;
  int m_fileNumber;
  vector<Iprogress*> m_iprogress;   // 支持多个观察者
public:
  FileSplitter(const string& filePath , int fileNumber)  // 新增进度条参数
    : m_filePath(filePath) , m_fileNumber(fileNumber) { }

  void split() {
    // 读取大文件
    for(int i=0 ; i<m_fileNumber ; ++i) {
      float progressValue = (float)(i + 1) / m_fileNumber;
      onProgress(progressValue);
    }
  }

  void register(Iprogress* iprogress) { m_iprogress.push_back(iprogress);}
  void unregister(Iprogress* iprogress) { m_iprogress.erase(iprogress);}
protected:
  void onProgress(Context context) {
    for_each(m_iprogress.begin() , m_iprogress.end() , 
          [&context](auto&& m_iprogress){m_iprogress->DoProgress(context);})
  }
};

class MainForm : public Form , public Iprogress {   //观察者
  TextBox* txtFilePath;     // 所要分割的文件路径
  TextBox* txtFileNumber;   // 所要分割的文件数目
  ProcessBar* processBar;   // 新增进度条
  void Button1_Click() {
    string filePath = txtFilePath->getText();
    int number = atoi(txtFileNumber->getText().c_str());
    FileSplitter splitter(filePath , number);
    splitter.register(this);
    splitter.split();
  }

  void void DoProgress(float value) override {
    precessBar->setValue(value);
  }
};