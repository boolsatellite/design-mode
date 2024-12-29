// 数据库访问有关的基类
class IDBConnection {};
class IDBCommand {};
class IDBDataReader {};

class IDBFactory {  // 将三个工厂合为一个工厂
public:
    virtual IDBConnection* CreateDBConnection() = 0;
    virtual IDBCommand* CreateDBCommand() = 0;
    virtual IDBDataReader* CreateDBDataReader() = 0;
};


// 支持SqlServer
class SqlConnection : public IDBConnection {

};
class SqlCommand : public IDBCommand {

};
class SqlDataReader : public IDataReader {

};

class SqlFactory : public IDBFactory {
  virtual IDBConnection *CreateDBConnection() { return new SqlConnection; }
  virtual IDBCommand *CreateDBCommand() { return new SqlCommand; }
  virtual IDBDataReader *CreateDBDataReader() { return new SqlDataReader; }
};

// 支持Oracle
class OracleConnection : public IDBConnection {

};
class OracleCommand : public IDBCommand {

};
class OracleDataReader : public IDataReader {

};

class EmployeeDAO {
    IDBFactory* DBfactory;  // 构造函数，依赖注入
public:
    vector<EmployeeD0> GetEmployees() {
        IDBConnection* connection = DBfactory->CreateDBConnection();
        connection->ConnectionSring = "...";
        IDBCommand* command = DBfactory->CreateDBCommand();
        command->CommandText = "...";
        command->SetConnection(connection);
        IDBDataReader* reader = DBfactory->CreateDBDataReader();
        while(reader->Read()) {

        }
    }
};