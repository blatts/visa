// -*- mode: C++ -*-
// Time-stamp: "2013-05-17 14:21:20 sb"

/*
  file       agilent33410A.cc
  copyright  (c) Sebastian Blatt 2011, 2012, 2013

 */

#define PROGRAM_NAME        "agilent33410A"
#define PROGRAM_DESCRIPTION "Communicate with Agilent 33410A via VISA."
#define PROGRAM_COPYRIGHT   "(C) Sebastian Blatt 2013"
#define PROGRAM_VERSION     "20130517"


#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include <windows.h>

#include "Visa.hh"
#include "CommandLine.hh"


class Agilent33410A : public VisaInstrument{
  private:
    std::list<std::string> error_queue;
    bool check_errors;

  public:
    Agilent33410A();
    virtual ~Agilent33410A();

    void OpenFirst();

    void CheckErrors(bool check_errors_){
      check_errors = check_errors_;
    }

    void ReadErrorQueue();
    bool ErrorOccurred();
    void HandleError();
    void ClearStatus();
    void ResetDevice();
    void SetBeep(bool beep);

    void SetupTrigger();
    void SetupDCMeasurement();
};

Agilent33410A::Agilent33410A()
: VisaInstrument(),
  error_queue(),
  check_errors(true)
{
}

Agilent33410A::~Agilent33410A() {
}

void Agilent33410A::OpenFirst() {
  OpenFirstByIDN("Agilent Technologies,34410A,");
}

void Agilent33410A::ReadErrorQueue(){
  while(true){
    std::string rc = Query("SYST:ERR:NEXT?");
    if(rc != "+0,\"No error\""){
      error_queue.push_back(rc);
    }
    else{
      break;
    }
  }
}

bool Agilent33410A::ErrorOccurred(){
  std::string rc = Query("*STB?");
  unsigned char i = (unsigned char)atoi(rc.c_str());
  std::cout << "*STB? = \"" << rc << "\" = " << (int)i << std::endl;
  return (i & 0x04) != 0;
}

void Agilent33410A::HandleError(){
  if(check_errors && ErrorOccurred()){
    ReadErrorQueue();
    for(std::list<std::string>::const_iterator i = error_queue.begin();
        i != error_queue.end(); ++i)
    {
      std::cout << "ERROR: " << *i << "\n";
    }
    error_queue.clear();
    throw EXCEPTION("Instrument error detected, see ERROR messages above.");
  }
}

void Agilent33410A::ClearStatus(){
  Write("*CLS");
  HandleError();
}

void Agilent33410A::ResetDevice(){
  Write("*RST");
  HandleError();
}

void Agilent33410A::SetBeep(bool beep){
  std::ostringstream os;
  os << "SYST:BEEP:STAT " << (beep ? "1" : "0");
  Write(os.str());
  HandleError();
}

void Agilent33410A::SetupTrigger(){
  Write("TRIG:SOUR IMM");
  HandleError();
  Write("TRIG:COUN 1");
  HandleError();
}

void Agilent33410A::SetupDCMeasurement(){
  Write("SENS:VOLT:DC:RANG:AUTO 1");
  HandleError();

  std::ostringstream os;
  os << "SENS:VOLT:DC:APER " << 0.1;
  Write(os.str());
  HandleError();

  std::cout << "Integration time = " << Query("SENS:VOLT:DC:APER?") << " s\n";
  HandleError();
}

class PerformanceCounterWrapper {
  private:
    double frequency;
    __int64 counter_start;
    time_t time_start;

  public:
    PerformanceCounterWrapper();
    double GetRelativeTime();
    double GetAbsoluteTime();
    unsigned GetStartTime();
};

PerformanceCounterWrapper::PerformanceCounterWrapper()
  : frequency(0),
    counter_start(0)
{
  LARGE_INTEGER tmp;
  QueryPerformanceFrequency(&tmp);
  frequency = ((double)tmp.QuadPart);

  QueryPerformanceCounter(&tmp);
  counter_start = tmp.QuadPart;

  time_start = time(0);
}

double PerformanceCounterWrapper::GetRelativeTime(){
  LARGE_INTEGER tmp;
  QueryPerformanceCounter(&tmp);
  return double(tmp.QuadPart - counter_start) / frequency;
}

double PerformanceCounterWrapper::GetAbsoluteTime(){
  return ((unsigned long)time_start) + GetRelativeTime();
}

unsigned PerformanceCounterWrapper::GetStartTime(){
  return (unsigned)time_start;
}

static bool __global_sigint_status = false;

BOOL control_handler(DWORD type){
  BOOL rc = FALSE;
  switch(type){
    case CTRL_C_EVENT:
      __global_sigint_status = true;
      rc = TRUE;
      //std::cerr << "Caught SIGINT, aborting." << std::endl;
      break;
    default:
      break;
  }
  return rc;
}

static const char* __command_line_options[] =
{
 "Output file", "output", "o", "voltage_data.txt",
  };




int main(int argc, char** argv){
  // Setup sigint handler on Windows
  if(! SetConsoleCtrlHandler((PHANDLER_ROUTINE) control_handler, TRUE)){
    std::cerr << "Could not install SIGINT handler." << std::endl;
    return 1;
  }

  CommandLine cl(argc, argv);
  DWIM_CommandLine(cl,
                   PROGRAM_NAME,
                   PROGRAM_DESCRIPTION,
                   PROGRAM_VERSION,
                   PROGRAM_COPYRIGHT,
                   __command_line_options,
                   sizeof(__command_line_options)/sizeof(char*)/4);

  try{

    std::string output_file = cl.GetFlagData("-o");
    std::ofstream of;
    of.open(output_file.c_str());

    VisaInstrument::InitializeVisaLibrary();
    Agilent33410A v;
    v.DebugProtocol(false);
    v.CheckErrors(false);

    v.OpenFirst();
    v.Clear();

    std::cout << "Connected to " << v.Query("*IDN?") << std::endl;

    v.ClearStatus();
    v.ResetDevice();
    v.SetBeep(false);

    v.SetupTrigger();
    v.SetupDCMeasurement();

    PerformanceCounterWrapper pcw;
    std::cout << "Clock starts at = " << pcw.GetStartTime() << "\n"
              << "\n"
              << "---\n";

    //for(size_t i=0; i<100000 && !__global_sigint_status; ++i){
    while(!__global_sigint_status) {
      double t0 = pcw.GetRelativeTime();
      std::string rc = v.Query("READ?");
      double t1 = pcw.GetRelativeTime();
      v.HandleError();

      std::cout << t0 << "\t" << t1 << "\t" << rc << "\n";
      of << pcw.GetStartTime() << "\t" << t0 << "\t" << t1 << "\t" << rc << "\n";
    }

    v.ResetDevice();
    VisaInstrument::FinalizeVisaLibrary();
  }
  catch(const Exception& e){
    std::cerr << "Exception caught from " << e.FILE << ":" << e.LINE
              << " " << e.FUNCTION << "()\n" << e.msg << std::endl;
  }

  return 0;
}

// agilent33410A.cc ends here
