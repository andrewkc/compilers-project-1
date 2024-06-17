#include "imp_ast.hh"
#include "visitors/ImpGeneralVisitor.hh"
#include "visitors/ImpValueVisitor.hh"
#include "visitors/ImpTypeVisitor.hh"

string Exp::binopToString(BinaryOp op) {
  switch(op) {
  case PLUS: return "+";
  case MINUS: return "-";
  case MULT: return "*";
  case DIV: return "/";
  case EXP: return "**";
  case LT: return "<";
  case LTEQ: return "<=";
  case EQ: return "==";
  case AND: return "and"; 
  case OR: return "or";
  }
  return "";
}

// EXP CONSTRUCTORS
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(int v):value(v) {}
IdExp::IdExp(string id):id(id) {}
ParenthExp::ParenthExp(Exp *e):e(e){}
CondExp::CondExp(Exp *c, Exp* et, Exp* ef):cond(c), etrue(et), efalse(ef){}
BoolConstExp::BoolConstExp(bool b):value(b) {}

// EXP DESTRUCTORS
Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
NumberExp::~NumberExp() { }
IdExp::~IdExp() { }
ParenthExp::~ParenthExp(){ delete e; }
CondExp::~CondExp(){ delete cond; delete etrue; delete efalse; }
BoolConstExp::~BoolConstExp() {}

// ImpGeneralVisitor
int BinaryExp::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
int NumberExp::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
int IdExp::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
int ParenthExp::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
int CondExp::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
int BoolConstExp::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}

// ImpValueVisitor
ImpValue BinaryExp::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
ImpValue NumberExp::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
ImpValue IdExp::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
ImpValue ParenthExp::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
ImpValue CondExp::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
ImpValue BoolConstExp::accept(ImpValueVisitor* v) {
  return v->visit(this);
}

// ImpTypeVisitor
ImpType BinaryExp::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
ImpType NumberExp::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
ImpType IdExp::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
ImpType ParenthExp::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
ImpType CondExp::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
ImpType BoolConstExp::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}

// STM CONSTRUCTORS
AssignStatement::AssignStatement(string id, Exp* e):id(id), rhs(e) { }
PrintStatement::PrintStatement(Exp* e):e(e) { }
IfStatement::IfStatement(Exp* c,Body *tb, Body* fb):cond(c),tbody(tb), fbody(fb) { }
WhileStatement::WhileStatement(Exp* c,Body *b):cond(c),body(b) { }
DoWhileStatement::DoWhileStatement(Exp* c, Body* b):cond(c), body(b) {}
StatementList::StatementList():slist() {}
VarDec::VarDec(string type, list<string> vars):type(type), vars(vars) {}
VarDecList::VarDecList():vdlist() {}
Body::Body(VarDecList* vdl, StatementList* sl):var_decs(vdl), slist(sl) {}
Program::Program(Body* b):body(b) {}

// STM DESTRUCTORS
Stm::~Stm() {}
AssignStatement::~AssignStatement() { delete rhs; }
PrintStatement::~PrintStatement() { delete e; }
IfStatement::~IfStatement() { delete fbody; delete tbody; delete cond; }
WhileStatement::~WhileStatement() { delete body; delete cond; }
DoWhileStatement::~DoWhileStatement() { delete body; delete cond; }
StatementList::~StatementList() { }
VarDec::~VarDec() { }
VarDecList::~VarDecList() { }
Body::~Body() { delete slist; delete var_decs; }
Program::~Program() { delete body; }

// ImpGeneralVisitor
void AssignStatement::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
void PrintStatement::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
void IfStatement::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
void WhileStatement::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
void DoWhileStatement::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
void StatementList::add(Stm* s) { slist.push_back(s);  }
void StatementList::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
void VarDec::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
void VarDecList::add(VarDec* vd) { vdlist.push_back(vd);  }
void VarDecList::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
void Body::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}
void Program::accept(ImpGeneralVisitor* v) {
  return v->visit(this);
}

// ImpValueVisitor
void AssignStatement::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
void PrintStatement::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
void IfStatement::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
void WhileStatement::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
void DoWhileStatement::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
void StatementList::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
void VarDec::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
void VarDecList::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
void Body::accept(ImpValueVisitor* v) {
  return v->visit(this);
}
void Program::accept(ImpValueVisitor* v) {
  return v->visit(this);
}

// ImpTypeVisitor
void AssignStatement::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
void PrintStatement::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
void IfStatement::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
void WhileStatement::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
void DoWhileStatement::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
void StatementList::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
void VarDec::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
void VarDecList::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
void Body::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
void Program::accept(ImpTypeVisitor* v) {
  return v->visit(this);
}
