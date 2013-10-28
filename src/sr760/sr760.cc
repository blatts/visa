// -*- mode: C++ -*-
// Time-stamp: "2013-10-28 17:32:53 sb"

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

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

#include "Visa.hh"
#include "CommandLine.hh"
#include "StringVector.hh"


#define SR760_IDN_STRING "Stanford_Research_Systems,SR760"


class SR760 : public VisaInstrument {
  public:
    enum Trace {ONE = 1, TWO = 2};

  private:
  public:
    SR760();
    void OpenFirst()
    void GetSpectrum(Trace trace, std::vector<double>& data);
};

SR760::SR760() {}

void SR760::OpenFirst(){
  OpenFirstByIDN(SR760_IDN_STRING);
}

void SR760::GetSpectrum(Trace trace, std::vector<double>& data){
  std::ostringstream os;
  os << "SPEC?" << (static_cast<int>(trace) - 1);
  std::string rc = Query(os.str(), 10000);

  std::vector<std::string> svals;
  boost::split(svals, rc, boost::is_any_of(","));

  //SR760 will always return 400 data bins
  svals.resize(400);
  vector_string_to_double(svals, data);
}


static const char* __command_line_options[] =
{
  "Trace to download (1, 2)", "trace", "t", "1",
  "Output file", "output", "o", "spectrum.txt"
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

    unsigned trace =  cl.GetFlagDataAsUint("-t");
    std::string output_file = cl.GetFlagData("-o");

    VisaInstrument::InitializeVisaLibrary();
    SR760 v;
    v.DebugProtocol(false);

    v.OpenFirst();
    v.Clear();


    std::string x = v.Query("*IDN?");
    std::cout << "Connected to " << x << std::endl;

    std::vector<double> vals;
    v.GetSpectrum(static_cast<SR760::Trace>(trace), vals);

    std::ofstream of;
    of.open(output_file.c_str());
    for(size_t i=0; i<vals.size(); ++i){
      of << i << "\t" << vals[i] << "\n";
    }
    of.close();

    rc = 0;
  }
  catch(const Exception& e){
    std::cerr << e << std::endl;
  }
  catch(const std::exception& e){
    std::cerr << e.what() << std::endl;
  }

  VisaInstrument::FinalizeVisaLibrary();
  return rc;
}

// sr760.cc ends here
