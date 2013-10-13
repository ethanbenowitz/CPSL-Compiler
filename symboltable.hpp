#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>
extern void yyerror(const char *str);

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

class Symbol{
  public:
    std::string name;
    Symbol(std::string name):name(name)
    {};
    virtual void print();
    virtual bool isType();
};

class Type:public Symbol{
  public:
    enum TypeType{
      type,
      record,
      array
    };
    TypeType typeType;
    int size;
    Type(std::string name, int size, TypeType typeType=type);
    virtual void print();
    bool isType();
};

class Const:public Symbol{
  public:
    enum ConstType{
      intType,
      charType,
      stringType,
      booleanType,
      identType
    };
    ConstType type;
    int numVal;
    char charVal;
    std::string strVal;
    std::string location;
    bool boolVal;
    int getIntVal();
    Const(int numVal, std::string name="");
    Const(char charVal, std::string name="");
    Const(std::string strVal, std::string name="");
    Const(bool boolVal, std::string name="");
    Const(std::string name, ConstType type);
    void print();
};

class Var:public Symbol{
  public:
    std::shared_ptr<Type> type;
    int location;
    Var(Type type, int location, std::string name="");
    void print();
};

class Function:public Symbol{
  public:
    enum FunctionType{
      function,
      procedure
    };
    std::string name;
    std::string location;
    std::shared_ptr<Type> returnType;
    std::vector<std::pair<std::vector<std::string>, std::shared_ptr<Type>>> typeList;
    bool defined;
    FunctionType funcType;
    Function(std::string name, Type returnType, std::vector<std::pair<std::vector<std::string>, std::shared_ptr<Type>>> typeList, bool defined=false);
    Function(std::string name, std::vector<std::pair<std::vector<std::string>, std::shared_ptr<Type>>> typeList, bool defined=false);
    void print();
};

class Array:public Type{
  public:
    int lower;
    int upper;
    std::shared_ptr<Type> type;
    Array(Type* type, Const lower, Const upper, std::string name="");
    void print();
    bool isType(){
      return true;
    };
};

class Record:public Type{
  public:
    std::map<std::string, std::pair<std::shared_ptr<Type>, int>> layout;
    Record(std::vector<std::pair<std::vector<std::string>, std::shared_ptr<Type>>> typeList, std::string name="");
    void print();
    bool isType(){
      return true;
    };
};

class Simple:public Type{
  public:
    enum simpleType{
      integer,
      boolean,
      character,
      string,
    };
    simpleType simType;
    std::shared_ptr<Type> type;
    Simple(simpleType simType, std::string name="");
    void print();
    bool isType(){
      return true;
    };
};

class SymbolTable{
  public:
    std::vector<std::map<std::string, std::shared_ptr<Symbol>>> tables;
    std::vector<int> offset;
    int labels;
    static std::shared_ptr<SymbolTable> instance;
    static std::shared_ptr<SymbolTable> getInstance();
    void pushScope(Function funcName);
    void popScope();
    void addFunction(std::string name, Function func, bool forward=false);
    template <class T>
    void addSymbol(std::string name, T sym, bool init=false){
      if(tables.back().find(name)!=tables.back().end()){
        if(init){
          std::cout<<name<<std::endl;
          yyerror(std::string(name+" already defined\n").data());
        }
      }
      auto temp=std::make_pair(name, std::make_shared<T>(sym));
      tables.back().insert(temp);
    };
    bool lookup(std::string name);
    void checkType(std::string name);
    std::shared_ptr<Symbol> getSymbol(std::string name);
  private:
    SymbolTable();
};

Const* negative(Const val);
Const* notOp(Const val);
Const* mod(Const left, Const right);
Const* div(Const left, Const right);
Const* mult(Const left, Const right);
Const* sub(Const left, Const right);
Const* add(Const left, Const right);
Const* gt(Const left, Const right);
Const* lt(Const left, Const right);
Const* gte(Const left, Const right);
Const* lte(Const left, Const right);
Const* neq(Const left, Const right);
Const* eq(Const left, Const right);
Const* andOp(Const left, Const right);
Const* orOp(Const left, Const right);
bool sameType(Const &left, Const &right);
bool checkIdent(Const &val, Const::ConstType type);

#endif
