#pragma once

#include "../imp_ast.hh"

class ImpGeneralVisitor {
public:
  virtual void visit(Program* p) = 0;
  virtual void visit(Body* b) = 0;
  virtual void visit(VarDecList* v) = 0;
  virtual void visit(VarDec* v) = 0;  
  virtual void visit(StatementList* s) = 0;
  virtual void visit(AssignStatement* s) = 0;
  virtual void visit(PrintStatement* s) = 0;
  virtual void visit(IfStatement* s) = 0;
  virtual void visit(WhileStatement* s) = 0;
  
  virtual int visit(BinaryExp* e) = 0;
  virtual int visit(NumberExp* e) = 0;
  virtual int visit(IdExp* e) = 0;
  virtual int visit(ParenthExp* e) = 0;
  virtual int visit(CondExp* e) = 0;
  virtual int visit(BoolConstExp* e) = 0;
};