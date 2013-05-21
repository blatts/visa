// -*- mode: C++ -*-
// Time-stamp: "2012-10-10 13:38:48 sb"

/*
  file       ProgressIndicator.hh
  copyright  (c) Sebastian Blatt 2012

 */


#ifndef PROGRESSINDICATOR_HH__4D3DDF44_D63B_4C2D_8A5C_72426F719D9C
#define PROGRESSINDICATOR_HH__4D3DDF44_D63B_4C2D_8A5C_72426F719D9C

#include <iostream>

class ProgressIndicator {
  private:
    size_t line_width;
    size_t percent_width;
    unsigned start_time;
    size_t current_position;
  public:
    ProgressIndicator();
    std::ostream& Show(std::ostream& out, size_t i, size_t N);
};


#endif // PROGRESSINDICATOR_HH__4D3DDF44_D63B_4C2D_8A5C_72426F719D9C

// ProgressIndicator.hh ends here
