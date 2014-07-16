// -*- mode: C++ -*-
// Time-stamp: "2014-07-16 11:16:33 sb"

/*
  file       keithley2701.cc
  copyright  (c) Sebastian Blatt 2011, 2012, 2013, 2014

 */


#define PROGRAM_NAME        "keithley2701"
#define PROGRAM_DESCRIPTION "Control Keithley 2701 digital multimeter"
#define PROGRAM_COPYRIGHT   "(C) Sebastian Blatt 2014"
#define PROGRAM_VERSION     "20140716"


#include <iostream>
#include <string>
#include <vector>

#include "Visa.hh"
#include "CommandLine.hh"

//static const char* __command_line_options[] = {};


int main(int argc, char** argv){
  int rc = 1;

  CommandLine cl(argc,argv);
  DWIM_CommandLine(cl,
                   PROGRAM_NAME,
                   PROGRAM_DESCRIPTION,
                   PROGRAM_VERSION,
                   PROGRAM_COPYRIGHT,
                   NULL,
                   0);
                   //__command_line_options,
                   //sizeof(__command_line_options)/sizeof(char*)/4);

  try{
    VisaInstrument::InitializeVisaLibrary();

    // Keithley 2701 does not support full VXI11, i.e. portmapper
    // discovery of instruments. Need to connect to socket directly.

    VisaInstrument v;
    v.DebugProtocol(true);
    v.OpenSocket("172.23.6.95", 1394);
    std::string x = v.Query("*IDN?");
    std::cout << "\"" << x << "\"" << std::endl;

    rc = 0;
  }
  catch(const Exception& e){
    std::cerr << e << std::endl;
  }

  VisaInstrument::FinalizeVisaLibrary();
  return rc;
}

// keithley2701.cc ends here
