// -*- mode: C++ -*-
// Time-stamp: "2012-03-29 17:56:27 sb"

/*
  file       tds2000.cc
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
    cl.AddFlag("output","o",1);
    cl.AddFlag("channel","c",1);
    cl.AddFlag("timebase","t",1);
    cl.AddFlag("scale","s",1);
    cl.Parse();

    std::string out_file = "trace.dat";
    if(cl.IsFlagDefined("-o")){
      out_file = cl.GetFlagData("-o");
    }

    int channel = 1;
    if(cl.IsFlagDefined("-c")){
      int c = string_to_int(cl.GetFlagData("-c"));
      if(c>=1 && c <=4){
        channel = c;
      }
    }
    std::ostringstream os;
    os << "CH" << channel;
    std::string channel_string = os.str();

    double timebase = 1e-3; // in s/DIV
    if(cl.IsFlagDefined("-t")){
      timebase = string_to_double(cl.GetFlagData("-t"));
    }
    double scale = 1.0; // in V/DIV
    if(cl.IsFlagDefined("-s")){
      scale = string_to_double(cl.GetFlagData("-s"));
    }

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


    VisaInstrument::FinalizeVisaLibrary();
  }
  catch(const Exception& e){
    std::cerr << "Exception caught from " << e.FILE << ":" << e.LINE
              << " " << e.FUNCTION << "()\n" << e.msg << std::endl;
  }

  return 0;
}

// tds2000.cc ends here
