// -*- mode: C++ -*-
// Time-stamp: "2012-11-19 19:24:03 sb"

/*
  file       ProgressIndicator.cc
  copyright  (c) Sebastian Blatt 2012

 */

#include "ProgressIndicator.hh"
#include "OutputManipulator.hh"
#include "Representable.hh"

#include <sstream>
#include <string>
#include <ctime>

ProgressIndicator::ProgressIndicator()
  : line_width(50),
    percent_width(6),
    start_time(0),
    current_position(0)
{}

class ProgressTime : public Representable {
  public:
    size_t dt;
    size_t seconds;
    size_t minutes;
    size_t hours;

    ProgressTime(size_t start_time){
      dt = (size_t)(time(0) - start_time);
      seconds = dt;
      hours = seconds / (3600);
      seconds -= hours * 3600;
      minutes = seconds / 60;
      seconds -= minutes * 60;
    }

    std::ostream& Represent(std::ostream& out) const {
      if(hours > 0){
        out << right_justified<size_t>(hours, 2, '0') << ":";
      }
      out << right_justified<size_t>(minutes, 2, '0') << ":"
          << right_justified<size_t>(seconds, 2, '0');
      return out;
    }
};


std::ostream& ProgressIndicator::Show(std::ostream& out, size_t i, size_t N) {
  if(i == 0){
    current_position = 0;
    start_time = time(0);
  }

  double p = ((double)(i+1))/((double)N);
  if(p > ((double)current_position)/((double)line_width)){
    out << '\r' << right_justified<int>((int)(100.0 * p), percent_width-2) << "% "
        << std::string(current_position+1, '|')
        << std::string(line_width-current_position-1, '.');
    out.flush();
    ++current_position;
  }
  if(i == N - 1){
    std::ostringstream os;
    os << "  calculation took " << ProgressTime(start_time) << ".";
    out << '\r' << os.str();
    size_t l = os.str().size();
    if(l < line_width + percent_width){
      out << std::string(line_width + percent_width - l, ' ') << std::endl;
    }
  }
  return out;
}

// ProgressIndicator.cc ends here
