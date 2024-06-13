#include "imp_typechecker.hh"

ImpTypeChecker::ImpTypeChecker() {

}

void ImpTypeChecker::typecheck(Program* p) {
  env.clear();
  p->accept(this);
  cout << "Exito" << endl; //
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

// ----------------------------------------

void ImpTypeChecker::visit(VarDec* vd) { //
  ImpType tt = ImpValue::get_basic_type(vd->type);
  if (tt == NOTYPE) { 
    cout << "El tipo de variable " << vd->type << " no es aceptado\n"; 
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

void ImpTypeChecker::visit(AssignStatement* s) { //
  ImpType type = s->rhs->accept(this);
  if (!env.check(s->id)) {
    cout << "La variable " << s->id << " no ha sido definida previamente\n";
    exit(0);
  }
  ImpType idType = env.lookup(s->id);
  if (type != idType) {
    cout << "La asignacion no corresponde al tipo de variable: " << s->id << endl;
    exit(0);
  }
  return;
}

void ImpTypeChecker::visit(PrintStatement* s) {
  s->e->accept(this);
  return;
}

void ImpTypeChecker::visit(IfStatement* s) { //
  ImpType type = s->cond->accept(this);
  if (type != TBOOL) {
    cout << "Se esperaba un tipo booleano en la sentencia if\n";
    exit(0);
  }
  s->tbody->accept(this);
  if (s->fbody != NULL)
    s->fbody->accept(this);
  return;
}

void ImpTypeChecker::visit(WhileStatement* s) { //  
  ImpType tcond = s->cond->accept(this);
  if (tcond != TBOOL) {
    cout << "Se esperaba un tipo booleano en la sentencia while\n";
    exit(0);
  }
  s->body->accept(this);
 return;
}

ImpType ImpTypeChecker::visit(BinaryExp* e) { //
  ImpType t1 = e->left->accept(this);
  ImpType t2 = e->right->accept(this);
  // ImpType result;
  if (t1 != t2) {
    cout << "La operacion " << e->op << " tiene argumentos incorrectos\n";
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
  case AND: //
  case OR: //
    return TBOOL;
  default: return NOTYPE;
  }
}

ImpType ImpTypeChecker::visit(NumberExp* e) { //
  ImpType t = TINT;
  return t;
}

ImpType ImpTypeChecker::visit(IdExp* e) { //
  ImpType t = env.lookup(e->id);
  return t;
}

ImpType ImpTypeChecker::visit(ParenthExp* ep) {
  return ep->e->accept(this);
}

ImpType ImpTypeChecker::visit(CondExp* e) {
  ImpType btype = e->cond->accept(this);
  if (btype != TBOOL) {
    cout << "Se esperaba un tipo booleano en cexp\n";
    exit(0);
  }
  ImpType ttype = e->etrue->accept(this);
  ImpType ftype = e->efalse->accept(this);
  if (ttype != ftype) {
    cout << "Tipo distinto en cexp\n";
    exit(0);
  }
  return ttype;
}
//
ImpType ImpTypeChecker::visit(BoolConst* e) {
  ImpType t = TBOOL;
  return t;
}
//