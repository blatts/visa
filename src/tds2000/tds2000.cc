// -*- mode: C++ -*-
// Time-stamp: "2013-05-17 15:55:25 sb"

/*
  file       tds2000.cc
  copyright  (c) Sebastian Blatt 2011, 2012, 2013

 */

#define PROGRAM_NAME        "tds2000"
#define PROGRAM_DESCRIPTION "Communicate with Tektronix TDS2000 via VISA."
#define PROGRAM_COPYRIGHT   "(C) Sebastian Blatt 2013"
#define PROGRAM_VERSION     "20130517"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "Visa.hh"
#include "CommandLine.hh"
#include "StringVector.hh"

static const char* __command_line_options[] =
{
 "Output file", "output", "o", "trace.dat",
 "Channel", "channel", "c", "1",
 "Timebase", "timebase", "t", "1e-3",
 "Scale", "scale", "s", "1.0",
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
    std::string out_file = cl.GetFlagData("-o");
    int channel = cl.GetFlagDataAsUint("-c");
    double timebase = cl.GetFlagDataAsDouble("-t");
    double scale = cl.GetFlagDataAsDouble("-s");

    std::ostringstream os;
    os << "CH" << channel;
    std::string channel_string = os.str();

    VisaInstrument::InitializeVisaLibrary();
    VisaInstrument v;
    v.OpenFirstByIDN("TEKTRONIX,TDS 2004B");

    v.Clear();
    std::cout << "Connected to " << v.Query("*IDN?") << std::endl;

    v.Write("ACQUIRE:STATE OFF");
    v.Write("SELECT:" + channel_string + " ON");
    v.Write("DATA:SOURCE " + channel_string);
    v.Write("DATA:WIDTH 1");
    v.Write("DATA:ENCDG ASCII");
    v.Write("ACQUIRE:STOPAFTER SEQUENCE");
    {
      std::ostringstream os;
      os << "HORIZONTAL:MAIN:SCALE " << timebase;
      v.Write(os.str());
    }
    {
      std::ostringstream os;
      os << channel_string << ":SCALE " << scale;
      v.Write(os.str());
    }
    v.Write("ACQUIRE:STATE ON");

    std::cout << "Download " << channel_string << " trace." << std::endl;
    std::string x = v.Query("CURVE?",100000, 10000);
    std::vector<std::string> s;
    boost::split(s,x,boost::is_any_of(","));
    std::vector<double> vals;
    vector_string_to_double(s, vals);

    double sec_per_div = string_to_double(v.Query("HORIZONTAL:MAIN:SCALE?"));
    double horizontal_pos = string_to_double(v.Query("HORIZONTAL:MAIN:POSITION?"));
    double volt_per_div = string_to_double(v.Query(channel_string + ":SCALE?"));
    double vertical_pos = string_to_double(v.Query(channel_string + ":POS?"));
    std::cout << "Horizontal scale  = " << sec_per_div << " s/DIV\n"
              << "Horizontal offset = " << horizontal_pos << " s\n"
              << "Vertical scale    = " << volt_per_div << " V/DIV\n"
              << "Vertical offset   = " << vertical_pos << " DIV\n";

    v.Write("ACQUIRE:STOPAFTER RUNSTOP");
    v.Write("ACQUIRE:STATE RUN");

    double dt = 10*sec_per_div/vals.size();
    double dv = 10*volt_per_div/(256);

    std::cout << "Save trace to \"" << out_file << "\"" << std::endl;
    std::ofstream of;
    of.open(out_file.c_str());
    for(size_t i=0; i<vals.size(); ++i){
      double t = (((double)i) - vals.size()/2.0) * dt + horizontal_pos;
      double v = dv*vals[i] - vertical_pos * volt_per_div;
      of << t << "\t" << v << "\n";
    }
    of.close();

    rc = 0;
  }
  catch(const Exception& e){
    std::cerr << e << std::endl;
  }

  VisaInstrument::FinalizeVisaLibrary();

  return rc;
}

// tds2000.cc ends here
