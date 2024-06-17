# Proyecto Parte I
**Integrantes:** 
- Kelvin Cahuana Condori
- Sergio Sotil Lozada
- Mauro Bobadilla Castillo

En este documento veremos los avances que se nos han pedido para el proyecto 1

## 1. Verificador de tipos (typechecker) y Generador de código (codegenerator)
***REPORTE** ¿Cómo se calculó la memoria necesaria para las variables globales?* 
**Modificaciones de código:**
* Se agregó las siguientos atributos enteros a la clase ImpTypeCheker: ``mem_size`` y ``count_addrs_vars`` y fueron incializadas en el construtor con el valor de cero.
* ``mem_size`` es la variable que contiene el valor óptimo para hacer la asignación de memoria al momento de generar el codigo en SVM.
```cpp
ImpTypeChecker::ImpTypeChecker():inttype(),booltype() {
  inttype.set_basic_type("int");
  booltype.set_basic_type("bool");
  sp = max_sp = 0;
  count_addrs_vars = 0; // <-
  mem_size = 0; // <-
}
```
* ``count_addrs_vars`` es una variable auxiliar que sirve para almacenar la cantidad de variables que se definen en cada scope ya sea anidado o no. Es decir almacena la cantidad de variables que se definen haciendo un recorrido profundo por los scopes. También, al salir de un scope anidado recupera el valor correcto que debería tener.
* En el método visit para Body se observa que al principio se crea una varibale cnt que es la que almacena el valor de la cantidad de variables que se definen antes de entrar a un scope anidado y luego en la penúltina recupera el valor al salir del scope anidado.
* El condicional if sirve para que mem_size sea actualizado con el valor máximo que puede tomar count_addrs_vars.
```cpp
void ImpTypeChecker::visit(Body* b) {
  int cnt = count_addrs_vars; // <-
  env.add_level();
  b->var_decs->accept(this);
  b->slist->accept(this);
  env.remove_level();
  if (count_addrs_vars > mem_size) mem_size = count_addrs_vars; // <-
  count_addrs_vars = cnt; // <-
  return;
}
```
* También se agregó, en la últina linea del for del método visit para VarDec, la línea de código para que count_addrs_vars vaya aumentado cuando se agregán variables en algún scope de un algún Body.
```cpp

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
    count_addrs_vars++; // <-
  }   
  return;
}
```
* Luego se agregó un nuevo parámetro a la función codegen de la clase ImpCodeGenerator para que el valor de mem_size sea el valor correspondiente valor para hacer alloc.
```cpp
//                                                                  v
void ImpCodeGenerator::codegen(Program* p, string outfname, int mem_size) {
  nolabel = "";
  current_label = 0;
  next_address = 0;
  mem_locals = 0;
  this->mem_size = mem_size; // <-
  p->accept(this);
  ofstream outfile;
  outfile.open(outfname);
  outfile << code.str();
  outfile.close();
  cout << "Local variable memory: " << mem_locals << endl;
}
```
* Finalemente en el archivo imp_compiler se hace la siguiente llamada al método codegen con el valor de checker.mem_size.
```cpp
int main(int argc, const char* argv[]) {

  Program *program; 
   
  if (argc != 2) {
    cout << "Incorrect number of arguments" << endl;
    exit(1);
  }

  std::ifstream t(argv[1]);
  std::stringstream buffer;
  buffer << t.rdbuf();
  Scanner scanner(buffer.str());
  
  Parser parser(&scanner);
  program = parser.parse();
  
  ImpPrinter printer;
  ImpInterpreter interpreter;
  ImpTypeChecker checker;
  ImpCodeGenerator cg;
  
  printer.print(program);
  
  cout << endl << "Type checking:" << endl;
  checker.typecheck(program);
  
  cout << endl << "Run program:" << endl;
  interpreter.interpret(program);

  string outfname = argv[1];
  outfname += ".svm";
  cout << endl << "Compiling to: " << outfname << endl;
  cg.codegen(program, outfname, checker.mem_size); // <- with mem_size for alloc
  delete program;
}
```
## 2. Generación de código I
***REPORTE:** ¿Qué cambios se hicieron al scanner y/o parser para lograr la inclusión de
comentarios?*

**Modificación en el Scanner:** 
* En el método ``nextToken`` del Scanner se modificó el case '/' con la necesidad de que se reconozca '//' y se itere hasta el final de la linea para retornar el token COMMENT.  
```cpp
case '/':
  c = nextChar();
  if (c == '/') {
    c = nextChar();
    while (c != '\0' && c != '\n') {
      c = nextChar();
    }
    rollBack();
    token = new Token(Token::COMMENT, getLexema());
  else {
    rollBack();
    token = new Token(Token::DIV);
  }
  break;
```
**Modificación en el Parser:**
* Se agregó un if más en el método ``parseVarDec`` para hacer match con el token COMMENT.
```cpp
VarDec* Parser::parseVarDec() {
  VarDec* vd = NULL;
  if (match(Token::VAR)) {
    if (!match(Token::ID)) parserError("Expecting type in var declaration");
    string var, type = previous->lexema;
    list<string> vars;
    if (!match(Token::ID)) parserError("Expecting id in var declaration");
    var = previous->lexema;
    vars.push_back(var);
    while(match(Token::COMMA)) {
      if (!match(Token::ID)) parserError("Expecting id in comma in var declaration");
      var = previous->lexema;
      vars.push_back(var);
    }
    if (match(Token::COMMENT)) {
      // do nothing
    }
    if (!match(Token::SEMICOLON)) parserError("Expecting semicolon at end of var declaration");
    vd = new VarDec(type,vars);
  }
  return vd;
}
```
* Se agregó whiles para que se haga match con el token COMMENT en el método ``parseStatementlist``.
```cpp
StatementList* Parser::parseStatementList() {
  StatementList* p = new StatementList();
  p->add(parseStatement());
  while(match(Token::SEMICOLON)) {
    while (match(Token::COMMENT)) {
      // do nothing
    }
    p->add(parseStatement());
  }
  while (match(Token::COMMENT)) {
    // do nothing
  }
  return p;
}
```

## 3. Sentencia do-while
***REPORTE:** Indicar el cambio a la gramática y los puntos donde se hicieron cambios al código. Además, proveer las definiciones de tcheck y codegen usadas.*

**Modificaciones de código:**   
* Se agregó el método visit en imp_codegenerator.cpp
```cpp
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
```
* Se agregó el método visit en imp_interpreter.cpp
```cpp
void ImpInterpreter::visit(DoWhileStatement* s) {
  ImpValue v = s->cond->accept(this);
  if (v.type != ImpType::TBOOL) {
    cout << "Type error in DoWhile: expected bool in conditional" << endl;
    exit(0);
  }  
  do {
    s->body->accept(this);
    v = s->cond->accept(this);
  } while (v.bool_value);
  return;
}
```
* Se agregó el método visit en imp_printer.cpp
```cpp
void ImpPrinter::visit(DoWhileStatement* s) {
  cout << "do" << endl;
  s->body->accept(this);
  cout << "while (";
  s->cond->accept(this);
  cout << ")";
  return;
}
```
* Se agregó el método visit en imp_typechecker.cpp
```cpp
void ImpTypeChecker::visit(DoWhileStatement* s) {
  s->body->accept(this);
  if (!s->cond->accept(this).match(booltype)) {
    cout << "Conditional in DoWhileStm must be: " << booltype << endl;
    exit(0);
  }
  sp_decr(1);
  return;
}
```
* Se agregó la condición para hacer match con el token DO dentro del método ``parseStatement`` en el Parser.
```cpp
else if (match(Token::DO)) {
  tb = parseBody();
  if (!match(Token::WHILE))
    parserError("Expected 'while'");
  e = parseBExp();
  s = new DoWhileStatement(e,tb);
}
```
* Se agregó la declaración del método virtual visit en ImpValueVisitor, ImpGeneralVisitor y ImpTypeVisitor. Luego fueron sobrescritos en ImpPrinter, ImpCodegenerator, ImpInterpreter y ImpTypeChecker:
```cpp
virtual void visit(DoWhileStatement* e) = 0;
```
* Se agregó la clase DoWhileStatement en imp_ast.hh y su implementación en imp_ast.cpp
```cpp
class DoWhileStatement : public Stm {
public:
  Exp* cond;
  Body *body;
  DoWhileStatement(Exp* c, Body* b);
  void accept(ImpGeneralVisitor* v);
  void accept(ImpValueVisitor* v);
  void accept(ImpTypeVisitor* v);
  ~DoWhileStatement();
};
```
**Definiciones:**
* Tcheck
```
tcheck(env, DoWhileStm(e, bd)) ifi tcheck(env, bd) &&
                                   tcheck(env, e) = bool 
```
* Codegen
```
codegen(addr, DoWhileStm(e, bd) = LENTRY: skip
                                  codegen(addr, bd)
                                  codegen(addr, e)
                                  jmpz LEND
                                  goto LENTRY
                                  LEND: skip
```
## 4. Run Program:
```
git clone https://github.com/andrewkc/compilers-project-1.git
cd compilers-project-1
cmake .
make
```
Ejemplo de uso:
```
./compiler scripts/example00.imp
./svm scripts/example00.imp.svm
./imp scripts/example11.imp
```
