#include "imp_typechecker.hh"

void ImpTypeChecker::sp_incr(int n) {
  sp += n;
  if (sp > max_sp) max_sp = sp;
}

void ImpTypeChecker::sp_decr(int n) {
  sp -= n;
  if (sp < 0) {
    cout << "stack less than 0" << endl;
    exit(0);
  }
}

ImpTypeChecker::ImpTypeChecker():inttype(),booltype() {
  inttype.set_basic_type("int");
  booltype.set_basic_type("bool");
  sp = max_sp = 0;
  count_addrs_vars = 0;
  mem_size = 0; //
}

void ImpTypeChecker::typecheck(Program* p) {
  env.clear();
  p->accept(this);
  cout << "Success" << endl;
  cout << "Max stack size: " << max_sp << endl;
  return;
}

void ImpTypeChecker::visit(Program* p) {
  p->body->accept(this);
  return;
}

void ImpTypeChecker::visit(Body* b) {
  int cnt = count_addrs_vars; //
  env.add_level();
  b->var_decs->accept(this);
  b->slist->accept(this);
  env.remove_level();
  if (count_addrs_vars > mem_size) mem_size = count_addrs_vars; //
  count_addrs_vars = cnt; //
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
  ImpType type;
  type.set_basic_type(vd->type);
  if (type.ttype == ImpType::NOTYPE) {
    cout << "Invalid type: " << vd->type << endl;
    exit(0);
  }
  list<string>::iterator it;
  for (it = vd->vars.begin(); it != vd->vars.end(); ++it) {
    env.add_var(*it, type);
    count_addrs_vars++; //
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
    cout << "Variable " << s->id << " undefined" << endl;
    exit(0);
  }
  sp_decr(1);
  ImpType var_type = env.lookup(s->id);  
  if (!type.match(var_type)) {
    cout << "Invalid type in Assign a " << s->id << endl;
    exit(0);
  }
  return;
}

void ImpTypeChecker::visit(PrintStatement* s) {
  s->e->accept(this);
  sp_decr(1);
  return;
}

void ImpTypeChecker::visit(IfStatement* s) {
  if (!s->cond->accept(this).match(booltype)) {
    cout << "Conditional in IfStm must be: " << booltype << endl;
    exit(0);
  }
  sp_decr(1);
  s->tbody->accept(this);
  if (s->fbody != NULL)
    s->fbody->accept(this);
  return;
}

void ImpTypeChecker::visit(WhileStatement* s) {
  if (!s->cond->accept(this).match(booltype)) {
    cout << "Conditional in WhileStm must be: " << booltype << endl;
    exit(0);
  }
  sp_decr(1);
  s->body->accept(this);
  return;
}

void ImpTypeChecker::visit(DoWhileStatement* s) {
  s->body->accept(this);
  if (!s->cond->accept(this).match(booltype)) {
    cout << "Conditional in DoWhileStm must be: " << booltype << endl;
    exit(0);
  }
  sp_decr(1);
  return;
}

ImpType ImpTypeChecker::visit(BinaryExp* e) {
  ImpType t1 = e->left->accept(this);
  ImpType t2 = e->right->accept(this);
  if (t1.ttype != t2.ttype) {
    cout << "The operation " << e->op << " has arguments of different types\n";
    exit(0);
  }
  ImpType result;
  switch(e->op) {
  case PLUS: 
  case MINUS:
  case MULT:
  case DIV:
  case EXP:
    result = inttype;
    break;
  case LT: 
  case LTEQ:
  case EQ:
  case AND:
  case OR:
    result = booltype;
    break;
  }
  sp_decr(1);
  return result;
}

ImpType ImpTypeChecker::visit(NumberExp* e) {
  sp_incr(1);
  return inttype;
}

ImpType ImpTypeChecker::visit(IdExp* e) {
  sp_incr(1);
  if (env.check(e->id))
    return env.lookup(e->id);
  else {
    cout << "Undefined variable: " << e->id << endl;
    exit(0);
  }
}

ImpType ImpTypeChecker::visit(ParenthExp* ep) {
  return ep->e->accept(this);
}

ImpType ImpTypeChecker::visit(CondExp* e) {
  ImpType booltype;
  booltype.set_basic_type("bool");
  if (!e->cond->accept(this).match(booltype)) {
    cout << "Type in ifexp must be bool" << endl;
    exit(0);
  }
  sp_decr(1);
  int sp_start = sp;
  ImpType ttype =  e->etrue->accept(this);
  sp = sp_start;
  if (!ttype.match(e->efalse->accept(this))) {
    cout << "Types in ifexp must be equal" << endl;
    exit(0);
  }
  return ttype;
}

ImpType ImpTypeChecker::visit(BoolConstExp* e) {
  sp_incr(1);
  return booltype;
}
