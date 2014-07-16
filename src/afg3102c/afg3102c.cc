// -*- mode: C++ -*-
// Time-stamp: "2013-08-28 17:37:06 sb"

/*
  file       afg3102c.cc
  copyright  (c) Sebastian Blatt 2011, 2012, 2013

 */


#define PROGRAM_NAME        "afg3102c"
#define PROGRAM_DESCRIPTION "Control Tektronix AFG3102C."
#define PROGRAM_COPYRIGHT   "(C) Sebastian Blatt 2013"
#define PROGRAM_VERSION     "20130828"


#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Visa.hh"
#include "CommandLine.hh"

static const char* __command_line_options[] = {
  "IP address", "address", "a", "172.23.6.26"
  };


int main(int argc, char** argv){
  int rc = 1;

  CommandLine cl(argc,argv);
  DWIM_CommandLine(cl,
                   PROGRAM_NAME,
                   PROGRAM_DESCRIPTION,
                   PROGRAM_VERSION,
                   PROGRAM_COPYRIGHT,
                   __command_line_options,
                   sizeof(__command_line_options)/sizeof(char*)/4);

  try{
    VisaInstrument::InitializeVisaLibrary();
    VisaInstrument v;

    std::ostringstream os;
    os << "TCPIP::" << cl.GetFlagData("-a");
    v.Open(os.str());
    std::cout << "Connected to : \"" << v.Query("*IDN?")
              << "\"" << std::endl;

    v.Write("SOUR1:FREQ:FIX 10 kHz");

    std::cout << v.Query("SOUR1:FREQ?") << std::endl;

    v.Close();

    rc = 0;
  }
  catch(const Exception& e){
    std::cerr << e << std::endl;
  }

  VisaInstrument::FinalizeVisaLibrary();
  return rc;
}

// afg3102c.cc ends here
