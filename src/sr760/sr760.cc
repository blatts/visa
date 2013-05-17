// -*- mode: C++ -*-
// Time-stamp: "2012-03-26 17:28:16 sb"

/*
  file       sr760.cc
  copyright  (c) Sebastian Blatt 2011, 2012

 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "exception.hh"
#include "string_vector.hh"
#include "visa.hh"
#include "cmd_line.hh"

int main(int argc, char** argv){
  try{
    CommandLine cl(argc,argv);
    cl.Parse();

    VisaInstrument::InitializeVisaLibrary();
    VisaInstrument v;
    v.DebugProtocol(true);


    v.OpenFirstByIDN("SR760");

    v.Clear();
    std::cout << "Connected to " << v.Query("*IDN?") << std::endl;


    VisaInstrument::FinalizeVisaLibrary();
  }
  catch(const Exception& e){
    std::cerr << "Exception caught from " << e.FILE << ":" << e.LINE
              << " " << e.FUNCTION << "()\n" << e.msg << std::endl;
  }

  return 0;
}

// sr760.cc ends here
