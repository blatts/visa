// -*- mode: C++ -*-
// Time-stamp: "2013-05-20 18:19:59 sb"

/*
  file       Visa.cc
  copyright  (c) Sebastian Blatt 2011, 2012, 2013

 */

#include <sstream>
#include <iomanip>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "Visa.hh"
#include "Exception.hh"
#include "StringVector.hh"
#include "Representable.hh"


bool VisaInstrument::is_visa_initialized = false;
ViSession VisaInstrument::default_resource_manager = VI_NULL;

void VisaInstrument::InitializeVisaLibrary(){
  if(!is_visa_initialized){
    ViStatus status = viOpenDefaultRM(&VisaInstrument::default_resource_manager);
    if(status != VI_SUCCESS){
      throw EXCEPTION("Failed initializing VISA library with viOpenDefaultRM()");
    }
  }
}

ViSession VisaInstrument::GetDefaultRM(){
  return default_resource_manager;
}

std::string VisaInstrument::GetDefaultRMStatusDescription(ViStatus status){
  static ViChar buffer[1024];
  std::string rc = "";
  ViStatus s = viStatusDesc(VisaInstrument::GetDefaultRM(), status, buffer);
  if(s == VI_SUCCESS){
    rc = buffer;
  }
  return rc;
}

void VisaInstrument::FinalizeVisaLibrary(){
  if(is_visa_initialized){
    viClose(VisaInstrument::default_resource_manager);
  }
}

VisaInstrument::VisaInstrument()
  : instrument_session(VI_NULL),
    debug_protocol(false),
    timeout(0) // will be automatically set on first call to Read()
{
}

VisaInstrument::~VisaInstrument(){
  Close();
}

std::string VisaInstrument::GetStatusDescription(ViStatus status){
  ViChar buffer[1024];
  std::string rc = "";
  ViStatus s = viStatusDesc(instrument_session, status, buffer);
  if(s == VI_SUCCESS){
    rc = buffer;
  }
  return rc;
}

void VisaInstrument::Open(const std::string& descriptor){
  if(debug_protocol){
    std::cout << TimeNow() << ": Open(\"" << descriptor << "\")" << std::endl;
  }
  ViStatus status = viOpen(VisaInstrument::GetDefaultRM(),
                           (ViChar*)descriptor.c_str(),
                           VI_NULL, VI_NULL, &instrument_session);
  if(status != VI_SUCCESS){
    std::ostringstream os;
    os << "viOpen("+descriptor+") failed with status code "
       << std::hex << status << ".\n" << GetStatusDescription(status);
    throw EXCEPTION(os.str());
  }
}

void VisaInstrument::Clear(){
  if(debug_protocol){
    std::cout << TimeNow() << ": Clear()" << std::endl;
  }
  ViStatus status = viClear(instrument_session);
  if(status != VI_SUCCESS){
    std::ostringstream os;
    os << "viClear() failed with status code "
       << std::hex << status << ".\n" << GetStatusDescription(status);
    throw EXCEPTION(os.str());
  }
}

void VisaInstrument::Close(){
  if(debug_protocol){
    std::cout << TimeNow() << ": Close()" << std::endl;
  }
  viClose(instrument_session);
  instrument_session = VI_NULL;
}

void VisaInstrument::Write(const std::string& cmd){
  ViUInt32 write_count = 0;
  if(debug_protocol){
    std::cout << TimeNow() << ": Write(\"" << cmd << "\")" << std::endl;
  }
  ViStatus status = viWrite(instrument_session, (ViBuf)cmd.c_str(), (ViUInt32)cmd.size(),
                            &write_count);
  if(status != VI_SUCCESS){
    std::ostringstream os;
    os << "viWrite(" << cmd << ") failed with status code "
       << std::hex << status << "\n." << GetStatusDescription(status);
    throw EXCEPTION(os.str());
  }
}

void VisaInstrument::SetTimeout(size_t timeout_){
  if(timeout_ != timeout){

    ViStatus status = viSetAttribute(instrument_session, VI_ATTR_TMO_VALUE, timeout_);

    if(status != VI_SUCCESS){
      std::ostringstream os;
      os << "viSetAttribute() failed with status code " << std::hex << status
         << ".\n" << GetStatusDescription(status);
      throw EXCEPTION(os.str());
    }

    timeout = timeout_;
  }
}

std::string VisaInstrument::Read(size_t buf_size, size_t timeout){
  if(debug_protocol){
    std::cout << TimeNow() << ": Read()" << std::endl;
  }

  SetTimeout(timeout);

  char* buf = new char[buf_size];
  memset((void*)buf, (char)0, buf_size);

  ViUInt32 read_count = 0;
  ViStatus status = viRead(instrument_session, (ViByte*)buf, buf_size, &read_count);
  if(status != VI_SUCCESS){
    delete[] buf;
    std::ostringstream os;
    os << "viRead() failed with status code " << std::hex << status
       << ".\n" << GetStatusDescription(status);
    throw EXCEPTION(os.str());
  }

  if(debug_protocol){
    std::cout << TimeNow() << ": Read() read " << read_count << " bytes." << std::endl;
  }

  buf[read_count] = '\0';
  std::string rc = std::string((char*)buf);
  delete[] buf;

  return rc;
}

void VisaInstrument::FindResourceList(std::vector<std::string>& descriptors,
                                      const std::string& mask)
{
  char descriptor[VI_FIND_BUFLEN];
  ViUInt32 number_of_instruments = 0;
  ViFindList find_list;
  if(debug_protocol){
    std::cout << TimeNow() << ": FindResourceList(\"" << mask << "\")" << std::endl;
  }
  ViStatus status = viFindRsrc(GetDefaultRM(), (ViChar*)mask.c_str(),
                               &find_list, &number_of_instruments,
                               descriptor);
  if(status != VI_SUCCESS){
    std::ostringstream os;
    os << "viFindRsrc(\"" << mask << "\") failed with status code "
       << std::hex << status << ".\n"
       << GetDefaultRMStatusDescription(status);
    throw EXCEPTION(os.str());
  }

  descriptors.clear();
  descriptors.push_back(descriptor);
  for(size_t i=1; i<number_of_instruments; ++i){
    status = viFindNext(find_list, descriptor);
    if(status != VI_SUCCESS){
      std::ostringstream os;
      os << "viFindNext(\"" << mask << "\") failed with status code "
         << std::hex << status << ".\n"
         << GetDefaultRMStatusDescription(status);
      throw EXCEPTION(os.str());
    }
    descriptors.push_back(descriptor);
  }
}

std::string VisaInstrument::Query(const std::string& cmd, size_t buf_size, size_t timeout){
  Write(cmd);
  std::string rc = Read(buf_size, timeout);
  boost::algorithm::trim(rc);
  return rc;
}


void VisaInstrument::OpenFirstByIDN(const std::string& idn_string){
  std::vector<std::string> rs;
  FindResourceList(rs);
  for(size_t i=0; i<rs.size(); ++i){
    Open(rs[i]);
    Write("*IDN?");
    std::string rc = Read();
    if(rc.substr(0,idn_string.size()) == idn_string){
      break;
    }
    Close();
  }
}

// Visa.cc ends here
