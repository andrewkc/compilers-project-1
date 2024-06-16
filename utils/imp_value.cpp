#include "imp_value.hh"

// --------------- ImpValue ------------------ 

ImpValue::ImpValue():type(ImpType::NOTYPE) { }

ImpType::TType ImpValue::get_basic_type(string s) {
  ImpType::TType tt;
  if (s.compare("int") == 0) tt = ImpType::TINT;
  else if (s.compare("bool") == 0) tt = ImpType::TINT;
  else tt = ImpType::NOTYPE; 
  return tt;
}

void ImpValue::set_default_value(ImpType::TType tt) {
  type = tt;
  if (tt == ImpType::TINT) {
    int_value = 0;
  } else if (tt == ImpType::TBOOL) {
    bool_value = false; // false for default value
  }
  return;
}

std::ostream& operator << ( std::ostream& outs, const ImpValue & v ) {
  if (v.type == ImpType::TINT)
    outs << v.int_value;
  else if (v.type == ImpType::TBOOL) {
    if (v.bool_value)
      outs << "true";
    else
      outs << "false";
  } else {
    outs << "NOTYPE";
  }
  return outs;
}

// --------------- ImpType ------------------ 

const char* ImpType::type_names[3] = { "notype", "int", "bool" };

bool ImpType::match(const ImpType& t) {
  if (this->ttype != t.ttype) return false;
  return true;
}

bool ImpType::set_basic_type(string s) {
  TType tt;
  bool ret = true;
  tt = string_to_type(s);
  if (tt == ImpType::NOTYPE) ret = false;
  this->ttype = tt;
  return ret;
}

bool ImpType::set_basic_type(TType tt) {
  bool ret = true;
  switch(tt) {
  case ImpType::TINT:
  case ImpType::TBOOL:
    this->ttype = tt; break;
  default:
    ret = false;
  }
  return ret;
}

ImpType::TType ImpType::string_to_type(string s) {
  TType tt;
  if (s.compare("int")==0) tt = ImpType::TINT;
  else if (s.compare("bool")==0) tt = ImpType::TBOOL;
  else tt = ImpType::NOTYPE; 
  return tt;
}

std::ostream& operator << ( std::ostream& outs, const ImpType & type ) {
  outs << ImpType::type_names[type.ttype];
  return outs;
}
