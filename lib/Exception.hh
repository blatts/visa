// -*- mode: C++ -*-
// Time-stamp: "2012-01-21 09:30:36 sb"

/*
  file       Exception.hh
  copyright  (c) Sebastian Blatt 2011, 2012

 */


#ifndef EXCEPTION_HH__E6538F8B_918B_4E70_BC73_61B773C37BA7
#define EXCEPTION_HH__E6538F8B_918B_4E70_BC73_61B773C37BA7

#include <iostream>
#include <string>
#include <exception>

class Exception : public std::exception {
  public:
    const std::string msg;
    const std::string FILE;
    const int LINE;
    const std::string FUNCTION;
    Exception(const std::string& msg_,
              const std::string& FILE_,
              const int LINE_,
              const std::string& FUNCTION_) throw ()
      : std::exception(),
        msg(msg_),
        FILE(FILE_),
        LINE(LINE_),
        FUNCTION(FUNCTION_)
    {}
    ~Exception() throw (){}
};

#define EXCEPTION(msg) Exception((msg),(__FILE__),(__LINE__),(__FUNCTION__))

inline std::ostream& operator<<(std::ostream& out, const Exception& e){
#ifdef DEBUG
  out << "Exception from " << e.FILE << ":" << e.LINE
      << " in function " << e.FUNCTION << "\n";
#endif
  out << e.msg;
  return out;
}


#endif // EXCEPTION_HH__E6538F8B_918B_4E70_BC73_61B773C37BA7

// Exception.hh ends here
