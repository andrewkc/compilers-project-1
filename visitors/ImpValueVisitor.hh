#pragma once

#include "../imp_ast.hh"
#include "../utils/imp_value.hh"

class ImpValueVisitor {
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
  virtual void visit(DoWhileStatement* s) = 0;
  
  virtual ImpValue visit(BinaryExp* e) = 0;
  virtual ImpValue visit(NumberExp* e) = 0;
  virtual ImpValue visit(IdExp* e) = 0;
  virtual ImpValue visit(ParenthExp* e) = 0;
  virtual ImpValue visit(CondExp* e) = 0;
  virtual ImpValue visit(BoolConstExp* e) = 0;
};
