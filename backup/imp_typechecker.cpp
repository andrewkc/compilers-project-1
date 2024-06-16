#include "imp_typechecker.hh"

ImpTypeChecker::ImpTypeChecker() {}

void ImpTypeChecker::typecheck(Program* p) {
  env.clear();
  p->accept(this);
  cout << "Success" << endl;
  return;
}

void ImpTypeChecker::visit(Program* p) {
  p->body->accept(this);
  return;
}

void ImpTypeChecker::visit(Body* b) {
  env.add_level();
  b->var_decs->accept(this);
  b->slist->accept(this);
  env.remove_level();  
  return;
}

void ImpTypeChecker::visit(VarDecList* decs) {
  list<VarDec*>::iterator it;
  for (it = decs->vdlist.begin(); it != decs->vdlist.end(); ++it) {
    (*it)->accept(this);
  }  
  return;
}

void ImpTypeChecker::visit(VarDec* vd) {
  ImpType tt = ImpValue::get_basic_type(vd->type);
  if (tt == NOTYPE) { 
    cout << "The type of variable " << vd->type << " is not accepted\n"; 
    exit(0); 
  }
  list<string>::iterator it;
  for (it = vd->vars.begin(); it != vd->vars.end(); ++it) {
    env.add_var(*it, tt);
  }
  return;
}

void ImpTypeChecker::visit(StatementList* s) {
  list<Stm*>::iterator it;
  for (it = s->slist.begin(); it != s->slist.end(); ++it) {
    (*it)->accept(this);
  }
  return;
}

void ImpTypeChecker::visit(AssignStatement* s) {
  ImpType type = s->rhs->accept(this);
  if (!env.check(s->id)) {
    cout << "The variable " << s->id << " has not been previously defined\n";
    exit(0);
  }
  ImpType idType = env.lookup(s->id);
  if (type != idType) {
    cout << "The assignment does not correspond to the variable type: " << s->id << endl;
    exit(0);
  }
  return;
}

void ImpTypeChecker::visit(PrintStatement* s) {
  s->e->accept(this);
  return;
}

void ImpTypeChecker::visit(IfStatement* s) {
  ImpType type = s->cond->accept(this);
  if (type != TBOOL) {
    cout << "Boolean type expected in if-statement\n";
    exit(0);
  }
  s->tbody->accept(this);
  if (s->fbody != NULL)
    s->fbody->accept(this);
  return;
}

void ImpTypeChecker::visit(WhileStatement* s) { 
  ImpType tcond = s->cond->accept(this);
  if (tcond != TBOOL) {
    cout << "Boolean type expected in while-statement\n";
    exit(0);
  }
  s->body->accept(this);
 return;
}

ImpType ImpTypeChecker::visit(BinaryExp* e) {
  ImpType t1 = e->left->accept(this);
  ImpType t2 = e->right->accept(this);
  if (t1 != t2) {
    cout << "The operation " << e->op << " has incorrect arguments\n";
    exit(0);
  }
  switch(e->op) {
  case PLUS: 
  case MINUS:
  case MULT:
  case DIV:
  case EXP:
    return TINT;
  case LT: 
  case LTEQ:
  case EQ:
  case AND:
  case OR:
    return TBOOL;
  default: return NOTYPE;
  }
}

ImpType ImpTypeChecker::visit(NumberExp* e) {
  ImpType t = TINT;
  return t;
}

ImpType ImpTypeChecker::visit(IdExp* e) {
  ImpType t = env.lookup(e->id);
  return t;
}

ImpType ImpTypeChecker::visit(ParenthExp* ep) {
  return ep->e->accept(this);
}

ImpType ImpTypeChecker::visit(CondExp* e) {
  ImpType btype = e->cond->accept(this);
  if (btype != TBOOL) {
    cout << "Boolean type expected in cexp\n";
    exit(0);
  }
  ImpType ttype = e->etrue->accept(this);
  ImpType ftype = e->efalse->accept(this);
  if (ttype != ftype) {
    cout << "Different type in cexp\n";
    exit(0);
  }
  return ttype;
}

ImpType ImpTypeChecker::visit(BoolConstExp* e) {
  ImpType t = TBOOL;
  return t;
}
