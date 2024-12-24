class Stream {
public:
    virtual char Read(int number) = 0;
    virtual void Seek(int position) = 0;
    virtual void Wirte(char data) = 0;
    virtual ~Stream() {};
};

class FileStream : public Stream {
public:
    char Read(int number) { }
    void Seek(int position) { }
    void Wirte(char data) { }
};
class NetStream : public Stream {
public:
    char Read(int number) { }
    void Seek(int position) { }
    void Wirte(char data) { }
};

class DecoratorStream : public Stream { // 装饰类
protected:
    Stream* stream;
    DecoratorStream(Stream* stm) : stream(stm) {}
};

class CryptoStream : public DecoratorStream{ //任然为流保持接口规范
public:
    CryptoStream(Stream* stm) : DecoratorStream(stm) {}
    char Read(int number) {
        // 加密操作
        stream->Read(number);
    }
    void Seek(int position) {
        // 加密操作
        stream->Seek(position);
        // 加密操作
    }
    void Wirte(char data) {
        // 加密操作
        stream->Wirte(data);
        // 加密操作
    }
};
class BufferStream : public DecoratorStream{  // 文件流缓冲
public:
    BufferStream(Stream* stm) : DecoratorStream(stm) {}
    char Read(int number) {
        // 缓冲操作
        stream->Read(number);
    }
    void Seek(int position) {
        // 缓冲操作
        stream->Seek(position);
    }
    void Wirte(char data) {
        // 缓冲操作
        stream->Wirte(data);
    }

}; 
// 文件流即加密又缓冲
class CryptoBufferStream : public DecoratorStream{};

void process() {
    FileStream* s1 = new FileStream();
    CryptoStream* s2 = new CryptoStream(s1);    //加密文件流
    BufferStream* s3 = new BufferStream(s1);         //加密缓冲流
}