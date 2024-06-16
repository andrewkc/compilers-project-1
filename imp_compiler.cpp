#include <sstream>
#include <iostream>
#include <iostream>

#include "imp_ast.hh"
#include "imp_parser.hh"
#include "visitors/imp_printer.hh"
#include "visitors/imp_interpreter.hh"
#include "visitors/imp_typechecker.hh"
#include "visitors/imp_codegenerator.hh"

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
  program = parser.parse(); // the parser builds the aexp
  
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
  cg.codegen(program, outfname, checker.mem_size); // with mem_size for alloc
  delete program;
}
