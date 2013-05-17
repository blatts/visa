// -*- mode: C++ -*-
// Time-stamp: "2013-05-17 13:01:14 sb"

/*
  file       lsvisa.cc
  copyright  (c) Sebastian Blatt 2011, 2012, 2013

 */

#include <iostream>
#include <string>
#include <vector>

#include "Visa.hh"
#include "CommandLine.hh"

int main(int argc, char** argv){
  try{
    CommandLine cl(argc,argv);
    cl.Parse();

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
  }
  catch(const Exception& e){
    std::cerr << e << std::endl;
  }

  VisaInstrument::FinalizeVisaLibrary();

  return 0;
}

// lsvisa.cc ends here
