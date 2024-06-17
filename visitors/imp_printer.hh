#pragma once

#include "ImpGeneralVisitor.hh"

#include "../imp_ast.hh"

class ImpPrinter : public ImpGeneralVisitor {
public:
  void print(Program* p);
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
  
  int visit(BinaryExp* e);
  int visit(NumberExp* e);
  int visit(IdExp* e);
  int visit(ParenthExp* e);
  int visit(CondExp* e);
  int visit(BoolConstExp* e);
};
