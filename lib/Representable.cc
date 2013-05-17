// -*- mode: C++ -*-
// Time-stamp: "2013-05-17 15:26:33 sb"

/*
  file       Representable.cc
  copyright  (c) Sebastian Blatt 2011, 2012, 2013

 */

#include "Representable.hh"

#include <ctime>


std::ostream& operator<<(std::ostream& out, const Representable& e){
  if(out.good()){
    e.Represent(out);
  }
  return out;
}

std::ostream& Combination::Represent(std::ostream& out) const{
  out << a << b;
  return out;
}

std::ostream& Alternative::Represent(std::ostream& out) const{
  if(c){
    out << a;
  }
  else{
    out << b;
  }
  return out;
}

static const char* const english_numerals[10] =
{"zero", "one", "two", "three", "four", "five", "six", "seven",
 "eight", "nine"};

std::ostream& EnglishNumeral::Represent(std::ostream& out) const{
  if(x < 10){
    out << english_numerals[x];
  }
  else{
    out << x;
  }
  return out;
}

std::ostream& EnglishPluralize::Represent(std::ostream& out) const{
  out << s;
  if(m == 0 || m >= 2){
    out << "s";
  }
  return out;
}

std::ostream& TimeNow::Represent(std::ostream& out) const{
  time_t t = time(0);

#if WIN32
  struct tm tmp;
  struct tm* p_tmp = &tmp;
  memset(p_tmp, 0, sizeof(struct tm));
  errno_t e = localtime_s(p_tmp, &t);
#else
  struct tm* p_tmp = NULL;
  p_tmp = localtime(&t);
#endif // WIN32

  char s[20];
  strftime(s, 20, "%H:%M:%S", p_tmp);
  out << s;
  return out;
}

// Representable.cc ends here
