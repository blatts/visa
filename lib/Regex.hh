// -*- mode: C++ -*-
// Time-stamp: "2012-10-22 09:00:19 sb"

/*
  file       Regex.hh
  copyright  (c) Sebastian Blatt 2012

  Compatibility header that switches between using boost::regex or
  std::regex depending on whether we compile with the new C++11
  standard.

  Since std::regex is basically boost::regex, we typedef regex_t and
  do using XXX::regex_match etc.

 */


#ifndef REGEX_HH__4856B311_5E36_4EF0_A83A_758810D76FE9
#define REGEX_HH__4856B311_5E36_4EF0_A83A_758810D76FE9

// This test will only work on gcc >= 4.7, since otherwise
// __cplusplus has been defined as 1. See
//
//   http://gcc.gnu.org/bugzilla/show_bug.cgi?id=1773
//
// However, this is fine since we will not want to compile with
// --std=c++0x or --std=c++11 below 4.7 anyway.
#if __cplusplus < 201100L
#define USING_BOOST_REGEX 1
#endif

#if USING_BOOST_REGEX

#include <boost/regex.hpp>
using boost::regex_match;
typedef boost::regex regex_t;

#else

#include <regex>
using std::regex_match;
typedef std::regex regex_t;

#endif // USING_BOOST_REGEX


#endif // REGEX_HH__4856B311_5E36_4EF0_A83A_758810D76FE9

// Regex.hh ends here
