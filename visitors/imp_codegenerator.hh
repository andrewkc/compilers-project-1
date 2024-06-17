#pragma once

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>

#include "ImpGeneralVisitor.hh"

#include "../imp_ast.hh"
#include "../utils/environment.hh"

class ImpCodeGenerator : public ImpGeneralVisitor {
public:
  void codegen(Program*, string outfname, int mem_size);
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

private:
  std::ostringstream code;
  string nolabel;
  int current_label;
  Environment<int> addresses;
  int next_address, mem_locals;
  int mem_size;
  void codegen(string label, string instr);
  void codegen(string label, string instr, int arg);
  void codegen(string label, string instr, string jmplabel);
  string next_label();
public:
  ImpCodeGenerator();
};
