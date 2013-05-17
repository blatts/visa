// -*- mode: C++ -*-
// Time-stamp: "2012-01-21 23:40:54 sb"

/*
  file       StringVector.hh
  copyright  (c) Sebastian Blatt 2011, 2012

 */


#ifndef STRINGVECTOR_HH__FF47430F_B7EA_4F71_9FE1_A950F830765C
#define STRINGVECTOR_HH__FF47430F_B7EA_4F71_9FE1_A950F830765C

#include <string>
#include <vector>

int string_to_int(const std::string& s);
size_t string_to_uint(const std::string& s);
double string_to_double(const std::string& s);

void vector_string_to_int(const std::vector<std::string>& from,
                          std::vector<int>& to);
void vector_string_to_uint(const std::vector<std::string>& from,
                           std::vector<size_t>& to);
void vector_string_to_double(const std::vector<std::string>& from,
                             std::vector<double>& to);

std::string join(const std::vector<std::string>& strings,
                 const std::string& separator);

#endif // STRINGVECTOR_HH__FF47430F_B7EA_4F71_9FE1_A950F830765C

// StringVector.hh ends here
