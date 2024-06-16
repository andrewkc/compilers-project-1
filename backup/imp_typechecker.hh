#pragma once

#include <unordered_map>

#include "ImpTypeVisitor.hh"

#include "../imp_ast.hh"
#include "../utils/environment.hh"

using namespace std;

class ImpTypeChecker : public ImpTypeVisitor {
public:
  ImpTypeChecker();
private:
  Environment<ImpType> env;

public:
  void typecheck(Program* p);
  void visit(Program* p);
  void visit(Body* b);
  void visit(VarDecList* v);
  void visit(VarDec* v);
  void visit(StatementList* s);
  void visit(AssignStatement* s);
  void visit(PrintStatement* s);
  void visit(IfStatement* s);
  void visit(WhileStatement* s);
  
  ImpType visit(BinaryExp* e);
  ImpType visit(NumberExp* e);
  ImpType visit(IdExp* e);
  ImpType visit(ParenthExp* e);
  ImpType visit(CondExp* e);
  ImpType visit(BoolConstExp* e);
};
