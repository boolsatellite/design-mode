#if 0
enum class NetWorkState {
  Network_Open,
  Network_Close,
  Network_Connect,
};

class NetworkProcessor {
  NetWorkState state;
public:
  void Operator1() {
    if(state == NetWorkState::Network_Open) {
      // ...
      state = NetWorkState::Network_Close;
    } else if(state == NetWorkState::Network_Close) {
      // ...
      state = NetWorkState::Network_Connect;
    } else if(state == NetWorkState::Network_Connect) {
      //...
      state = NetWorkState::Network_Open;
    }
  }

  void Operator2() {
    if(state == NetWorkState::Network_Open) {
      // ...
      state = NetWorkState::Network_Connect;
    } else if(state == NetWorkState::Network_Close) {
      // ...
      state = NetWorkState::Network_Open;
    } else if(state == NetWorkState::Network_Connect) {
      // ...
       state = NetWorkState::Network_Close;
    }
  }

};
#endif

class CloseState;

class NetworkState {
public:
  NetworkState* pNext;
  virtual void Operator1() = 0;
  virtual void Operator2() = 0;
  virtual ~NetworkState() = default;
};

class OpenState : public NetworkState {
  static NetworkState* m_instance;
public:
  static NetworkState* getInstance() {
    if(m_instance == nullptr) {
      m_instance = new OpenState{};
    }
    return m_instance;
  }
  void Operator1() override {
    // ...
    pNext = CloseState::getInstance();
  }
  void Operator2() override {
    // ...
    pNext = ConnectState::getInstance();
  }
};

class CloseState : public NetworkState {
  static NetworkState* m_instance;
public:
  static NetworkState* getInstance() {
    if(m_instance == nullptr) {
      m_instance = new CloseState{};
    }
    return m_instance;
  }
  void Operator1() override {
    // ...
    pNext = CloseState::getInstance();
  }
  void Operator2() override {
    pNext = OpenState::getInstance();
  }
};

class ConnectState: public NetworkState {
  static NetworkState* m_instance;
public:
  static NetworkState* getInstance() {
    if(m_instance == nullptr) {
      m_instance = new ConnectState{};
    }
    return m_instance;
  }
  void Operator1() override {

  }
  void Operator2() override {

  }
};

class NetworkProcessor {
  NetworkState* pState;
public:
  NetworkProcessor(NetworkState* pState) {
    this->pState = pState;
  }
  void Operator1() {
    //...
    pState->Operator1();
    pState = pState->pNext;
  }
  void Operator2()  {
    //...
    pState->Operator2();
    pState = pState->pNext;
  }
};