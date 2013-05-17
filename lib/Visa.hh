// -*- mode: C++ -*-
// Time-stamp: "2013-05-17 15:51:59 sb"

/*
  file       Visa.hh
  copyright  (c) Sebastian Blatt 2011, 2012, 2013

 */


#ifndef VISA_HH__731B623E_D697_4AA6_8816_3B797DF85DBE
#define VISA_HH__731B623E_D697_4AA6_8816_3B797DF85DBE

#include <string>
#include <visa.h>

#define VISA_DEVICE_DESCRIPTOR_MASK "(GPIB|USB)[0-9]::?*::INSTR"
//#define VISA_DEVICE_DESCRIPTOR_MASK "(GPIB|USB)[0-9]::?*"


class VisaInstrument{
  private:
    static bool is_visa_initialized;
    static ViSession default_resource_manager;
    ViSession instrument_session;

    bool debug_protocol;

  public:
    static void InitializeVisaLibrary();
    static void FinalizeVisaLibrary();
    static ViSession GetDefaultRM();
    static std::string GetDefaultRMStatusDescription(ViStatus status);

    VisaInstrument();
    ~VisaInstrument();

    std::string GetStatusDescription(ViStatus status);
    void Open(const std::string& descriptor);
    void Clear();
    void Close();
    void Write(const std::string& cmd);
    std::string Read(size_t buf_size = 1024, size_t timeout = 2000);
    std::string Query(const std::string& cmd, size_t buf_size = 1024, size_t timeout = 2000);
    void FindResourceList(std::vector<std::string>& descriptors,
                          const std::string& mask = VISA_DEVICE_DESCRIPTOR_MASK);

    void OpenFirstByIDN(const std::string& idn_string);

    void DebugProtocol(bool debug_protocol_){
      debug_protocol = debug_protocol_;
    }

};

#endif // VISA_HH__731B623E_D697_4AA6_8816_3B797DF85DBE

// Visa.hh ends here
