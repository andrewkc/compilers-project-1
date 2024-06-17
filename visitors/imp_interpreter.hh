#pragma once

#include <unordered_map>

#include "ImpValueVisitor.hh"

#include "../imp_ast.hh"
#include "../utils/environment.hh"

using namespace std;

class ImpInterpreter : public ImpValueVisitor {
private:
  Environment<ImpValue> env;
public:
  void interpret(Program* p);
  void visit(Program* p);
  void visit(Body* b);
  void visit(VarDecList* v);
  void visit(VarDec* v);  
  void visit(StatementList* s);
  void visit(AssignStatement* s);
  void visit(PrintStatement* s);
  void visit(IfStatement* s);
  void visit(WhileStatement* s);
  void visit(DoWhileStatement* s);
  
  ImpValue visit(BinaryExp* e);
  ImpValue visit(NumberExp* e);
  ImpValue visit(IdExp* e);
  ImpValue visit(ParenthExp* e);
  ImpValue visit(CondExp* e);
  ImpValue visit(BoolConstExp* e);
};
