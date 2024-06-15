#pragma once

#include <sstream>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <list>

#include "utils/imp_value.hh"

using namespace std;

class ImpGeneralVisitor;
class ImpValueVisitor;
class ImpTypeVisitor;

enum BinaryOp { PLUS, MINUS, MULT, DIV, EXP, LT, LTEQ, EQ, AND, OR };
  
  
class Exp {
public:
  virtual int accept(ImpGeneralVisitor* v) = 0;
  virtual ImpValue accept(ImpValueVisitor* v) = 0;
  virtual ImpType accept(ImpTypeVisitor* v) = 0;
  static string binopToString(BinaryOp op);
  virtual ~Exp() = 0;
};

class BinaryExp : public Exp {
public:
  Exp *left, *right;
  BinaryOp op;
  BinaryExp(Exp* l, Exp* r, BinaryOp op);
  int accept(ImpGeneralVisitor* v);
  ImpValue accept(ImpValueVisitor* v);
  ImpType accept(ImpTypeVisitor* v);
  ~BinaryExp();
};

class NumberExp : public Exp {
public:
  int value;
  NumberExp(int v);
  int accept(ImpGeneralVisitor* v);
  ImpValue accept(ImpValueVisitor* v);
  ImpType accept(ImpTypeVisitor* v);
  ~NumberExp();
};

class IdExp : public Exp {
public:
  string id;
  IdExp(string id);
  int accept(ImpGeneralVisitor* v);
  ImpValue accept(ImpValueVisitor* v);
  ImpType accept(ImpTypeVisitor* v);
  ~IdExp();
};

class ParenthExp : public Exp {
public:
  Exp *e;
  ParenthExp(Exp *e);
  int accept(ImpGeneralVisitor* v);
  ImpValue accept(ImpValueVisitor* v);
  ImpType accept(ImpTypeVisitor* v);
  ~ParenthExp();
};

class CondExp : public Exp {
public:
  Exp *cond, *etrue, *efalse;
  CondExp(Exp* c, Exp* et, Exp* ef);
  int accept(ImpGeneralVisitor* v);
  ImpValue accept(ImpValueVisitor* v);
  ImpType accept(ImpTypeVisitor* v);
  ~CondExp();
};

class BoolConstExp : public Exp {
public:
  bool value;
  BoolConstExp(bool v);
  int accept(ImpGeneralVisitor* v);
  ImpValue accept(ImpValueVisitor* v);
  ImpType accept(ImpTypeVisitor* v);
  ~BoolConstExp();
};


class Stm {
public:
  virtual void accept(ImpGeneralVisitor* v) = 0;
  virtual void accept(ImpValueVisitor* v)=0;
  virtual void accept(ImpTypeVisitor* v)=0;
  virtual ~Stm() = 0;
};

class StatementList;
class Body;

class AssignStatement : public Stm {
public:
  string id;
  Exp* rhs;  
  AssignStatement(string id, Exp* e);
  void accept(ImpGeneralVisitor* v);
  void accept(ImpValueVisitor* v);
  void accept(ImpTypeVisitor* v);
  ~AssignStatement();
};

class PrintStatement : public Stm {
public:
  Exp* e;  
  PrintStatement(Exp* e);
  void accept(ImpGeneralVisitor* v);
  void accept(ImpValueVisitor* v);
  void accept(ImpTypeVisitor* v);
  ~PrintStatement();
};

class IfStatement : public Stm {
public:
  Exp* cond;
  Body *tbody, *fbody;
  IfStatement(Exp* c, Body* tbody, Body *fbody);
  void accept(ImpGeneralVisitor* v);
  void accept(ImpValueVisitor* v);
  void accept(ImpTypeVisitor* v);
  ~IfStatement();
};

class WhileStatement : public Stm {
public:
  Exp* cond;
  Body *body;
  WhileStatement(Exp* c, Body* b);
  void accept(ImpGeneralVisitor* v);
  void accept(ImpValueVisitor* v);
  void accept(ImpTypeVisitor* v);
  ~WhileStatement();
};

class StatementList {
public:
  list<Stm*> slist;
  StatementList();
  void add(Stm* s);
  void accept(ImpGeneralVisitor* v);
  void accept(ImpValueVisitor* v);
  void accept(ImpTypeVisitor* v);
  ~StatementList();
};

class VarDec {
public:
  string type;
  list<string> vars;
  VarDec(string type, list<string> vars);
  void accept(ImpGeneralVisitor* v);
  void accept(ImpValueVisitor* v);
  void accept(ImpTypeVisitor* v);
  ~VarDec();
};


class VarDecList {
public:
  list<VarDec*> vdlist;
  VarDecList();
  void add(VarDec* s);
  void accept(ImpGeneralVisitor* v);
  void accept(ImpValueVisitor* v);
  void accept(ImpTypeVisitor* v);
  ~VarDecList();
};


class Body {
public:
  VarDecList* var_decs;
  StatementList* slist;
  Body(VarDecList* vdl, StatementList* sl);
  void accept(ImpGeneralVisitor* v);
  void accept(ImpValueVisitor* v);
  void accept(ImpTypeVisitor* v);
  ~Body();
};

class Program {
public:
  Body* body;
  Program(Body* body);
  void accept(ImpGeneralVisitor* v);
  void accept(ImpValueVisitor* v);
  void accept(ImpTypeVisitor* v);
  ~Program();
};
