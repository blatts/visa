// -*- mode: C++ -*-
// Time-stamp: "2013-09-11 13:00:00 sb"

/*
  file       CommandLine.hh
  copyright  (c) Sebastian Blatt 2011, 2012, 2013

 */


#ifndef COMMANDLINE_HH__92B2F746_4720_4EFE_926A_FA30AF30E713
#define COMMANDLINE_HH__92B2F746_4720_4EFE_926A_FA30AF30E713

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "Exception.hh"
#include "Validator.hh"

template<typename key_t, typename val_t>
bool has_key(const std::map<key_t, val_t>& m, const key_t& k) {
  return (m.find(k) != m.end());
}

class CommandLineException : public Exception {
  public:
    CommandLineException(const std::string& msg_,
                         const std::string& FILE_,
                         const int LINE_,
                         const std::string& FUNCTION_) throw ()
      : Exception(msg_,FILE_,LINE_,FUNCTION_){}
};

#define COMMAND_LINE_EXCEPTION(msg) CommandLineException((msg),(__FILE__),(__LINE__),(__FUNCTION__))


class CommandLine{
    typedef enum {Flag, Data} ArgumentToken;
  private:
    std::string short_option_prefix;
    std::string long_option_prefix;
    bool MatchNoDigitCharacter(char c) const;
    bool MatchDigitCharacter(char c) const;
    bool MatchShortOption(const std::string& s) const;
    bool MatchLongOption(const std::string& s) const;

    std::vector<std::string> args;
    bool parsed;

    size_t new_flag_id;
    std::map<std::string, size_t> flag_ids;
    size_t GetNewFlagId();
    size_t GetFlagIdFromName(const std::string& flag_name) const;

    std::map<size_t, size_t> flag_fingerprint;
    std::vector<ArgumentToken> parsed_tokens;
    std::map<size_t, std::string> parsed_flag_map;
    std::vector<std::string> parsed_free_arguments;
    std::map<size_t, std::string> help_texts;

    std::string program_name;
    std::string program_description;
    std::string program_version;
    std::string program_copyright;


  public:

    CommandLine(int argc_, char** argv_);
    ~CommandLine();
    void Parse();

    void AddFlag(const std::string& help_text,
                 const std::string& long_flag,
                 const std::string& short_flag,
                 size_t required_arguments=0,
                 const std::string& default_argument=""
                );
    bool IsFlagDefined(const std::string& flag) const;
    const std::string& GetFlagData(const std::string& flag, const Validator<std::string>& val = ValidatorTrue<std::string>()) const;

    int GetFlagDataAsInt(const std::string& flag, const Validator<int>& val = ValidatorRange<int>()) const;
    size_t GetFlagDataAsUint(const std::string& flag, const Validator<size_t>& val = ValidatorRange<size_t>()) const;
    double GetFlagDataAsDouble(const std::string& flag, const Validator<double>& val = ValidatorRange<double>()) const;

    size_t CountFreeArguments() const;
    const std::string& GetFreeArgument(size_t n) const;


    std::ostream& PrintHelpTexts(std::ostream& out) const;

    void SetVersionInformation(const std::string& program_name_,
                               const std::string& program_description_,
                               const std::string& program_version_,
                               const std::string& program_copyright_);
    std::ostream& PrintVersionInformation(std::ostream& out) const;


    // Define command line options from string array
    void DefineAndParse(const char** const program_options, size_t length);
};


// Convenience function that just Does What I Mean to handle setting
// up command line parameters. Handles help, version information and
// exits on command line errors using exit().
void DWIM_CommandLine(CommandLine& cl,
                      const std::string& program_name,
                      const std::string& program_description,
                      const std::string& program_version,
                      const std::string& program_copyright,
                      const char** const program_options,
                      size_t length);


#endif // COMMANDLINE_HH__92B2F746_4720_4EFE_926A_FA30AF30E713

// CommandLine.hh ends here
