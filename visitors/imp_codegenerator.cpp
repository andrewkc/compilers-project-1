#include "imp_codegenerator.hh"

ImpCodeGenerator::ImpCodeGenerator() {}
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

void ImpCodeGenerator::codegen(Program* p, string outfname, int mem_size) {
  nolabel = "";
  current_label = 0;
  next_address = 0;
  mem_locals = 0;
  this->mem_size = mem_size;
  p->accept(this);
  ofstream outfile;
  outfile.open(outfname);
  outfile << code.str();
  outfile.close();
  cout << "Local variable memory: " << mem_locals << endl;
}

void ImpCodeGenerator::visit(Program* p) {
  // int mem_size = 10; // retract
  codegen(nolabel,"alloc", mem_size);
  p->body->accept(this);
  codegen(nolabel, "halt");
}

void ImpCodeGenerator::visit(Body * b) {
  int dir = next_address; // checkpoint address
  addresses.add_level();  
  b->var_decs->accept(this);
  b->slist->accept(this);
  addresses.remove_level();
  if (next_address > mem_locals) mem_locals = next_address;
  next_address = dir; // recovers the address value that it had before entering a new body
}

void ImpCodeGenerator::visit(VarDecList* s) {
  list<VarDec*>::iterator it;
  for (it = s->vdlist.begin(); it != s->vdlist.end(); ++it) {
    (*it)->accept(this);
  }  
}
			  
void ImpCodeGenerator::visit(VarDec* vd) {
  list<string>::iterator it;
  for (it = vd->vars.begin(); it != vd->vars.end(); ++it){
    addresses.add_var(*it, next_address++);
  }
}

void ImpCodeGenerator::visit(StatementList* s) {
  list<Stm*>::iterator it;
  for (it = s->slist.begin(); it != s->slist.end(); ++it) {
    (*it)->accept(this);
  }
}

void ImpCodeGenerator::visit(AssignStatement* s) {
  s->rhs->accept(this);
  codegen(nolabel,"store",addresses.lookup(s->id));
}

void ImpCodeGenerator::visit(PrintStatement* s) {
  s->e->accept(this);
  code << "print" << endl;;
}

void ImpCodeGenerator::visit(IfStatement* s) {
  string l1 = next_label();
  string l2 = next_label();
  
  s->cond->accept(this);
  codegen(nolabel,"jmpz",l1);
  s->tbody->accept(this);
  codegen(nolabel,"goto",l2);
  codegen(l1,"skip");
  if (s->fbody!=NULL) {
    s->fbody->accept(this);
  }
  codegen(l2,"skip");
}

void ImpCodeGenerator::visit(WhileStatement* s) {
  string l1 = next_label();
  string l2 = next_label();

  codegen(l1,"skip");
  s->cond->accept(this);
  codegen(nolabel,"jmpz",l2);
  s->body->accept(this);
  codegen(nolabel,"goto",l1);
  codegen(l2,"skip");
}

void ImpCodeGenerator::visit(DoWhileStatement* s) {
  string l1 = next_label();
  string l2 = next_label();

  codegen(l1,"skip");
  s->body->accept(this);
  s->cond->accept(this);
  codegen(nolabel,"jmpz",l2);
  codegen(nolabel,"goto",l1);
  codegen(l2,"skip");
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
  codegen(nolabel, op);
  return 0;
}

int ImpCodeGenerator::visit(NumberExp* e) {
  codegen(nolabel,"push ",e->value);
  return 0;
}

int ImpCodeGenerator::visit(IdExp* e) {
  codegen(nolabel,"load",addresses.lookup(e->id));
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
  codegen(nolabel, "jmpz", l1);
  e->etrue->accept(this);
  codegen(nolabel, "goto", l2);
  codegen(l1,"skip");
  e->efalse->accept(this);
  codegen(l2, "skip");
  return 0;
}

int ImpCodeGenerator::visit(BoolConstExp* e) {
  codegen(nolabel,"push",e->value);
  return 0;
}