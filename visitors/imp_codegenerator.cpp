#include "imp_codegenerator.hh"

void ImpCodeGenerator::codegen(string label, string instr) {
  if (label !=  nolabel)
    code << label << ": ";
  code << instr << endl;
}

void ImpCodeGenerator::codegen(string label, string instr, int arg) {
  if (label !=  nolabel)
    code << label << ": ";
  code << instr << " " << arg << endl;
}

void ImpCodeGenerator::codegen(string label, string instr, string jmplabel) {
  if (label !=  nolabel)
    code << label << ": ";
  code << instr << " " << jmplabel << endl;
}

string ImpCodeGenerator::next_label() {
  string l = "L";
  string n = to_string(current_label++);
  l.append(n);
  return l;
}

void ImpCodeGenerator::codegen(Program* p, string outfname) {
  nolabel = "";
  current_label = 0;
  next_adress = 0;
  p->accept(this);
  ofstream outfile;
  outfile.open(outfname);
  outfile << code.str();
  outfile.close();

  return;
}

void ImpCodeGenerator::visit(Program* p) {
  int mem_size = 10;
  codegen(nolabel,"alloc",mem_size);
  p->body->accept(this);
  codegen(nolabel, "halt");
  return;
}

void ImpCodeGenerator::visit(Body * b) {
  addresses.add_level();  
  b->var_decs->accept(this);
  b->slist->accept(this);
  addresses.remove_level();
  return;
}

void ImpCodeGenerator::visit(VarDecList* s) {
  list<VarDec*>::iterator it;
  for (it = s->vdlist.begin(); it != s->vdlist.end(); ++it) {
    (*it)->accept(this);
  }  
  return;
}
			  
void ImpCodeGenerator::visit(VarDec* vd) {
  list<string>::iterator it;
  for (it = vd->vars.begin(); it != vd->vars.end(); ++it){
    // What is the next direction?
    addresses.add_var(*it, 0);
  }
  return;
}

void ImpCodeGenerator::visit(StatementList* s) {
  list<Stm*>::iterator it;
  for (it = s->slist.begin(); it != s->slist.end(); ++it) {
    (*it)->accept(this);
  }
  return;
}

void ImpCodeGenerator::visit(AssignStatement* s) {
  s->rhs->accept(this);

  return;
}

void ImpCodeGenerator::visit(PrintStatement* s) {
  s->e->accept(this);

  return;
}

void ImpCodeGenerator::visit(IfStatement* s) {
  string l1 = next_label();
  string l2 = next_label();
  
  s->cond->accept(this);

  s->tbody->accept(this);

  if (s->fbody!=NULL) {
    s->fbody->accept(this);
  }

 
  return;
}

void ImpCodeGenerator::visit(WhileStatement* s) {
  string l1 = next_label();
  string l2 = next_label();

  s->cond->accept(this);

  s->body->accept(this);

  return;
}

int ImpCodeGenerator::visit(BinaryExp* e) {
  e->left->accept(this);
  e->right->accept(this);
  string op = "";
  switch(e->op) {
  case PLUS: op =  "add"; break;
  case MINUS: op = "sub"; break;
  case MULT:  op = "mul"; break;
  case DIV:  op = "div"; break;
  case LT:  op = "lt"; break;
  case LTEQ: op = "le"; break;
  case EQ:  op = "eq"; break;
  default: cout << "binop " << Exp::binopToString(e->op) << " not implemented" << endl;
  }

  return 0;
}

int ImpCodeGenerator::visit(NumberExp* e) {

  return 0;
}

int ImpCodeGenerator::visit(IdExp* e) {

  return 0;
}

int ImpCodeGenerator::visit(ParenthExp* ep) {
  ep->e->accept(this);
  return 0;
}

int ImpCodeGenerator::visit(CondExp* e) {
  string l1 = next_label();
  string l2 = next_label();
 
  e->cond->accept(this);

  e->etrue->accept(this);

  e->efalse->accept(this);

  return 0;
}

int ImpCodeGenerator::visit(BoolConstExp* e) {

  return 0;
}