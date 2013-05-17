// -*- mode: C++ -*-
// Time-stamp: "2011-12-16 17:59:23 sb"

/*
  file       keithley3390.cc
  copyright  (c) Sebastian Blatt 2011

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
    cl.AddFlag("frequency","f",1);
    cl.AddFlag("amplitude","a",1);
    cl.AddFlag("offset","o",1);
    cl.Parse();

    double freq = 1e6; // Hz
    double amp = 1.0; // Vpp
    double offset = 0.0; // V
    if(cl.IsFlagDefined("-f")){
      freq = string_to_double(cl.GetFlagData("-f"));
    }
    if(cl.IsFlagDefined("-a")){
      amp = string_to_double(cl.GetFlagData("-a"));
    }
    if(cl.IsFlagDefined("-o")){
      offset = string_to_double(cl.GetFlagData("-o"));
    }

    VisaInstrument::InitializeVisaLibrary();
    VisaInstrument v;
    v.OpenFirstByIDN("Keithley Instruments Inc.,3390,");

    v.Clear();
    std::cout << "Connected to " << v.Query("*IDN?") << std::endl;

    std::cout << "Frequency " << freq << " Hz" << "\n"
              << "Amplitude " << amp << " Vpp" << "\n"
              << "Offset " << offset << " V" << "\n";

    // Turn off output
    v.Write("OUTP OFF");

    std::ostringstream os;
    os << "FREQ " << freq << "; "
       << "VOLT:UNIT Vpp; ";
    v.Write(os.str());
    std::ostringstream oo;
    oo << "VOLT " << amp  << "; "
       << "VOLT:OFFS " << offset;
    v.Write(oo.str());

    // Turn on output
    v.Write("OUTP ON");

    // Release front panel
    v.Write("SYST:COMM:RLST LOC");


    VisaInstrument::FinalizeVisaLibrary();
  }
  catch(const Exception& e){
    std::cerr << "Exception caught from " << e.FILE << ":" << e.LINE
              << " " << e.FUNCTION << "()\n" << e.msg << std::endl;
  }

  return 0;
}

// keithley3390.cc ends here
