#pragma once

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>

using namespace std;

class ImpType {
public:
  enum TType { NOTYPE=0, TINT, TBOOL };
  static const char* type_names[3];
  TType ttype;
  // vector<TType> types;
  bool match(const ImpType&);
  bool set_basic_type(string s);
  bool set_basic_type(TType tt);
private:
  TType string_to_type(string s);
};

std::ostream& operator << ( std::ostream& outs, const ImpType & type );


class ImpValue {
public:
  ImpValue();
  ImpType::TType type;
  int int_value;
  bool bool_value;
  void set_default_value(ImpType::TType tt);
  static ImpType::TType get_basic_type(string s);
};

std::ostream& operator << ( std::ostream& outs, const ImpValue & v );
