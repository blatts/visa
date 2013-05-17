// -*- mode: C++ -*-
// Time-stamp: "2012-01-21 23:41:17 sb"

/*
  file       StringVector.cc
  copyright  (c) Sebastian Blatt 2011, 2012

 */

#include "StringVector.hh"
#include <cstdlib>

int string_to_int(const std::string& s){
  return atoi(s.c_str());
}

size_t string_to_uint(const std::string& s){
  return static_cast<size_t>(strtoul(s.c_str(), NULL, 0));
}

double string_to_double(const std::string& s){
  return atof(s.c_str());
}


void vector_string_to_int(const std::vector<std::string>& from,
                          std::vector<int>& to)
{
  std::vector<std::string>::const_iterator a = from.begin();
  const std::vector<std::string>::const_iterator e = from.end();
  to.resize(from.size());
  std::vector<int>::iterator b = to.begin();
  for(; a!=e; ++a, ++b){
    *b = string_to_int(*a);
  }
}

void vector_string_to_uint(const std::vector<std::string>& from,
                           std::vector<size_t>& to)
{
  std::vector<std::string>::const_iterator a = from.begin();
  const std::vector<std::string>::const_iterator e = from.end();
  to.resize(from.size());
  std::vector<size_t>::iterator b = to.begin();
  for(; a!=e; ++a, ++b){
    *b = string_to_uint(*a);
  }
}


void vector_string_to_double(const std::vector<std::string>& from,
                             std::vector<double>& to)
{
  std::vector<std::string>::const_iterator a = from.begin();
  const std::vector<std::string>::const_iterator e = from.end();
  to.resize(from.size());
  std::vector<double>::iterator b = to.begin();
  for(; a!=e; ++a, ++b){
    *b = string_to_double(*a);
  }
}

std::string join(const std::vector<std::string>& strings,
                 const std::string& separator)
{
  const size_t n = strings.size();
  std::string rc;
  for(size_t j=0; j<n; ++j){
    rc.append(strings[j]);
    if(j < n-1){
      rc.append(separator);
    }
  }
  return rc;
}




// StringVector.cc ends here
