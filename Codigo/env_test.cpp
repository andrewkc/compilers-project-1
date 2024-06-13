#include "environment.hh"
#include "imp_value.hh"

#include <iostream>

int main(int argc, const char* argv[]) {

  Environment<int> env;

  env.add_level();
  env.add_var("x", 10);
  env.add_var("y", 20);
}