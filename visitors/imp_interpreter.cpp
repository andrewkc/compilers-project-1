#include "imp_interpreter.hh"

/* *************** End of program status management ****** */

void ImpInterpreter::interpret(Program* p) {
  env.clear();
  p->accept(this);
  return;
}

void ImpInterpreter::visit(Program* p) {
  p->body->accept(this);
  return;
}

void ImpInterpreter::visit(Body* b) {
  env.add_level(); 
  b->var_decs->accept(this);
  b->slist->accept(this);
  env.remove_level();  
  return;
}

void ImpInterpreter::visit(VarDecList* decs) {
  list<VarDec*>::iterator it;
  for (it = decs->vdlist.begin(); it != decs->vdlist.end(); ++it) {
    (*it)->accept(this);
  }  
  return;
}

void ImpInterpreter::visit(VarDec* vd) {
  list<string>::iterator it;
  ImpValue v;
  ImpType::TType tt = ImpValue::get_basic_type(vd->type);
  if (tt == ImpType::NOTYPE) {
    cout << "Invalid type: " << vd->type << endl;
    exit(0);
  }
  v.set_default_value(tt);
  for (it = vd->vars.begin(); it != vd->vars.end(); ++it) {
    env.add_var(*it,v);
  }   
  return;
}

void ImpInterpreter::visit(StatementList* s) {
  list<Stm*>::iterator it;
  for (it = s->slist.begin(); it != s->slist.end(); ++it) {
    (*it)->accept(this);
  }
  return;
}

void ImpInterpreter::visit(AssignStatement* s) {
  ImpValue v = s->rhs->accept(this);
  if (!env.check(s->id)) {
    cout << "Variable " << s->id << " undefined" << endl;
    exit(0);
  }
  env.update(s->id, v);
  return;
}

void ImpInterpreter::visit(PrintStatement* s) {
  ImpValue v = s->e->accept(this);
  cout << v << endl;
  return;
}

void ImpInterpreter::visit(IfStatement* s) {
  ImpValue v = s->cond->accept(this);
  if (v.type != ImpType::TBOOL) {
    cout << "Type error in If: expected bool in conditional" << endl;
    exit(0);
  }	      
  if (v.bool_value) {
    s->tbody->accept(this);
  } else {
    if (s->fbody != NULL)
      s->fbody->accept(this);
  }
  return;
}

void ImpInterpreter::visit(WhileStatement* s) {
  ImpValue v = s->cond->accept(this);
  if (v.type != ImpType::TBOOL) {
    cout << "Type error in While: expected bool in conditional" << endl;
    exit(0);
  }	        
 while (v.bool_value) {
    s->body->accept(this);
    v = s->cond->accept(this);
  }
 return;
}

ImpValue ImpInterpreter::visit(BinaryExp* e) {
  ImpValue result;
  ImpValue v1 = e->left->accept(this);
  ImpValue v2 = e->right->accept(this);
  if (v1.type != v2.type) {
    cout << "Type error: operands in binary operation must be of the same type" << endl;
    exit(0);
  }
  int iv, iv1, iv2;
  bool bv, bv1, bv2;
  ImpType::TType type = ImpType::NOTYPE;
  iv1 = v1.int_value;
  iv2 = v2.int_value;
  bv1 = v1.bool_value;
  bv2 = v2.bool_value;
  switch(e->op) {
  case PLUS: iv = iv1+iv2; type = ImpType::TINT; break;
  case MINUS: iv = iv1-iv2; type = ImpType::TINT; break;
  case MULT: iv = iv1 * iv2; type = ImpType::TINT; break;
  case DIV: iv = iv1 / iv2; type = ImpType::TINT; break;
  case EXP:
    iv = 1;
    while (iv2 > 0) { iv *= iv1; iv2--; }
    type = ImpType::TINT;
    break;
  case LT: bv = (iv1 < iv2) ? 1 : 0; type = ImpType::TBOOL; break;
  case LTEQ: bv = (iv1 <= iv2) ? 1: 0; type = ImpType::TBOOL; break;
  case EQ: bv = (iv1 == iv2) ? 1 : 0; type = ImpType::TBOOL; break;
  case AND: bv = (bv1 && bv2) ? 1 : 0; type = ImpType::TBOOL; break;
  case OR: bv = (bv1 || bv2) ? 1 : 0; type = ImpType::TBOOL; break;
  }
  if (type == ImpType::TINT) result.int_value = iv;
  else result.bool_value = bv;
  result.type = type;
  return result;
}

ImpValue ImpInterpreter::visit(NumberExp* e) {
  ImpValue v;
  v.set_default_value(ImpType::TINT);
  v.int_value = e->value;
  return v;
}

ImpValue ImpInterpreter::visit(IdExp* e) {
  if (env.check(e->id))
    return env.lookup(e->id);
  else {
    cout << "Undefined variable: " << e->id << endl;
    exit(0);
  }
}

ImpValue ImpInterpreter::visit(ParenthExp* ep) {
  return ep->e->accept(this);
}

ImpValue ImpInterpreter::visit(CondExp* e) {
  ImpValue v = e->cond->accept(this);
  if (v.type != ImpType::TBOOL) {
    cout << "Type error in ifexp: expected bool in conditional" << endl;
    exit(0);
  }
  if (v.bool_value == 0)
    return e->efalse->accept(this);
  else
    return e->etrue->accept(this);
}

ImpValue ImpInterpreter::visit(BoolConstExp* e) {
  ImpValue v;
  v.set_default_value(ImpType::TBOOL);
  v.bool_value = e->value;
  return v;
}
