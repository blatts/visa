// -*- mode: C++ -*-
// Time-stamp: "2013-05-17 16:20:15 sb"

/*
  file       sr760.cc
  copyright  (c) Sebastian Blatt 2011, 2012, 2013

 */

#define PROGRAM_NAME        "SR760"
#define PROGRAM_DESCRIPTION "Communicate with SRS SR760 via VISA."
#define PROGRAM_COPYRIGHT   "(C) Sebastian Blatt 2013"
#define PROGRAM_VERSION     "20130517"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "Visa.hh"
#include "CommandLine.hh"

// static const char* __command_line_options[] =
// {
//   };


int main(int argc, char** argv){
  int rc = 1;

  CommandLine cl(argc, argv);
  DWIM_CommandLine(cl,
                   PROGRAM_NAME,
                   PROGRAM_DESCRIPTION,
                   PROGRAM_VERSION,
                   PROGRAM_COPYRIGHT,
                   NULL,
                   0);
                   //  __command_line_options,
                   //sizeof(__command_line_options)/sizeof(char*)/4);

  try{
    CommandLine cl(argc,argv);
    cl.Parse();

    VisaInstrument::InitializeVisaLibrary();
    VisaInstrument v;
    v.DebugProtocol(true);


    v.OpenFirstByIDN("SR760");

    v.Clear();
    std::cout << "Connected to " << v.Query("*IDN?") << std::endl;

    rc = 0;
  }
  catch(const Exception& e){
    std::cerr << e << std::endl;
  }

  VisaInstrument::FinalizeVisaLibrary();
  return rc;
}

// sr760.cc ends here
