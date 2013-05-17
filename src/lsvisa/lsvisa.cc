// -*- mode: C++ -*-
// Time-stamp: "2013-05-17 15:43:52 sb"

/*
  file       lsvisa.cc
  copyright  (c) Sebastian Blatt 2011, 2012, 2013

 */


#define PROGRAM_NAME        "lsvisa"
#define PROGRAM_DESCRIPTION "List VISA devices."
#define PROGRAM_COPYRIGHT   "(C) Sebastian Blatt 2013"
#define PROGRAM_VERSION     "20130517"


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
    VisaInstrument v;
    std::vector<std::string> rs;
    try{
      v.FindResourceList(rs);
    }
    catch(const Exception& e){
      std::cerr << "No VISA devices found.\n"
                << "---\n"
                << "Details:\n";
      throw e;
    }
    for(size_t i=0; i<rs.size(); ++i){
      std::cout << rs[i] << "\n";
      v.Open(rs[i]);
      std::cout << "*IDN? -> \"" << v.Query("*IDN?") << "\"\n";
      v.Close();
      std::cout << "\n";
    }

    rc = 0;
  }
  catch(const Exception& e){
    std::cerr << e << std::endl;
  }

  VisaInstrument::FinalizeVisaLibrary();
  return rc;
}

// lsvisa.cc ends here
