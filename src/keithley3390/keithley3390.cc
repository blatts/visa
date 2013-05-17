// -*- mode: C++ -*-
// Time-stamp: "2013-05-17 16:20:15 sb"

/*
  file       keithley3390.cc
  copyright  (c) Sebastian Blatt 2011, 2012, 2013

 */

#define PROGRAM_NAME        "keithley3390"
#define PROGRAM_DESCRIPTION "Communicate with Keithley 3390 via VISA."
#define PROGRAM_COPYRIGHT   "(C) Sebastian Blatt 2013"
#define PROGRAM_VERSION     "20130517"


#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "Visa.hh"
#include "CommandLine.hh"


static const char* __command_line_options[] =
{
 "Signal frequency", "frequency", "f", "1e6",
 "Signal amplitude", "amplitude", "a", "1.0",
 "Signal offset", "offset", "o", "0.0",
  };



int main(int argc, char** argv){
  int rc = 1;

  CommandLine cl(argc, argv);
  DWIM_CommandLine(cl,
                   PROGRAM_NAME,
                   PROGRAM_DESCRIPTION,
                   PROGRAM_VERSION,
                   PROGRAM_COPYRIGHT,
                   __command_line_options,
                   sizeof(__command_line_options)/sizeof(char*)/4);

  try{
    double freq = cl.GetFlagDataAsDouble("-f"); // Hz
    double amp = cl.GetFlagDataAsDouble("-a"); // Vpp
    double offset = cl.GetFlagDataAsDouble("-o"); // V

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

    rc = 0;
  }
  catch(const Exception& e){
    std::cerr << e << std::endl;
  }

  VisaInstrument::FinalizeVisaLibrary();
  return rc;
}

// keithley3390.cc ends here
