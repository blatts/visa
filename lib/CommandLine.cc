// -*- mode: C++ -*-
// Time-stamp: "2013-01-27 11:43:16 sb"

/*
  file       CommandLine.cc
  copyright  (c) Sebastian Blatt 2011, 2012, 2013

 */

#include "CommandLine.hh"
#include "Representable.hh"
#include "StringVector.hh"

#include <locale>
#include <sstream>
#include <cstdlib>

// Static const definitions:
//
// The below calls default ctor automatically
// = assignment calls copy ctor
// () ctor tries to call function named ValidatorTrueString().
const ValidatorTrue<std::string> CommandLine::ValidatorTrueString;
const ValidatorRange<int> CommandLine::ValidatorRangeInt;
const ValidatorRange<size_t> CommandLine::ValidatorRangeSizeT;
const ValidatorRange<double> CommandLine::ValidatorRangeDouble;


size_t CommandLine::GetNewFlagId(){
  return new_flag_id++;
}

size_t CommandLine::GetFlagIdFromName(const std::string& flag_name) const {
  std::map<std::string, size_t>::const_iterator it = flag_ids.find(flag_name);
  if(it == flag_ids.end()){
    throw COMMAND_LINE_EXCEPTION("Unknown command line flag \"" + flag_name + "\".");
  }
  return it->second;
}


CommandLine::CommandLine(int argc_, char** argv_)
  : short_option_prefix("-"),
    long_option_prefix("--"),
    args(argc_-1),
    parsed(false),
    new_flag_id(0),
    flag_ids(),
    flag_fingerprint(),
    parsed_tokens(),
    parsed_flag_map(),
    parsed_free_arguments(),
    help_texts(),
    program_name(""),
    program_description(""),
    program_version(""),
    program_copyright("")
{
  std::copy(argv_+1,argv_+argc_,args.begin());
}

CommandLine::~CommandLine(){
}

bool CommandLine::MatchNoDigitCharacter(char c) const {
  return std::isalnum(c) && !MatchDigitCharacter(c);
}

bool CommandLine::MatchDigitCharacter(char c) const {
  return std::isdigit(c);
}

bool CommandLine::MatchShortOption(const std::string& s) const {
  const size_t n = short_option_prefix.size();
  if ((s.size() >= n) && (s.substr(0, n) == short_option_prefix)){
    if(s.size() == n){
      throw COMMAND_LINE_EXCEPTION("Invalid short option \"" + s + "\".");
    }
    else if(MatchNoDigitCharacter(s[n])){
      return true;
    }
  }
  return false;
}

bool CommandLine::MatchLongOption(const std::string& s) const {
  const size_t n = long_option_prefix.size();
  if((s.size() >= n) && (s.substr(0, n) == long_option_prefix)){
    if(s.size() == n){
      throw COMMAND_LINE_EXCEPTION("Invalid long option \"" + s + "\".");
    }
    for(size_t j=n; j<s.size(); ++j){
      if(!MatchNoDigitCharacter(s[j])){
        throw COMMAND_LINE_EXCEPTION("Invalid long option \"" + s + "\".");
      }
    }
    return true;
  }
  return false;
}

void CommandLine::Parse(){
  parsed_tokens.resize(args.size());
  for(size_t i=0; i<args.size(); ++i){
    std::string t = args[i];
    if(MatchShortOption(t) || MatchLongOption(t)){
      parsed_tokens[i] = Flag;
    }
    else{
      parsed_tokens[i] = Data;
    }
  }

  for(size_t i=0; i<parsed_tokens.size(); ++i){
    //std::cout << "\"" << args[i] << "\" -> " << parsed_tokens[i] << "\n";
    if(parsed_tokens[i] == Flag){
      const size_t id = GetFlagIdFromName(args[i]);
      const size_t required_data = flag_fingerprint[id];
      for(size_t j=1; j<=required_data; ++j){
        if(parsed_tokens[i+j] != Data){
          std::ostringstream os;
          os << "Flag \"" << args[i] << "\" requires "
             << EnglishNumeral(required_data) << " "
             << EnglishPluralize(BasicRepresentable<std::string>("argument"),
                                 required_data)
             << ". "
             << Alternative(BasicRepresentable<std::string>("None"),
                            Combination(BasicRepresentable<std::string>("Only "),
                                        EnglishNumeral(j-1)),
                            (j-1==0))
             << " given.";
          throw COMMAND_LINE_EXCEPTION(os.str());
        }
      }
      std::vector<std::string> flag_args(required_data);
      std::copy(&(args[i+1]), &(args[i+required_data+1]), flag_args.begin());
      parsed_flag_map[id] = join(flag_args," ");
      i += required_data;
    }
    else{
      parsed_free_arguments.push_back(args[i]);
    }
  }
  parsed = true;
}

void CommandLine::AddFlag(const std::string& help_text,
                          const std::string& long_flag,
                          const std::string& short_flag,
                          size_t required_arguments,
                          const std::string& default_argument)
{
  if(MatchDigitCharacter(short_flag[0])){
    throw COMMAND_LINE_EXCEPTION("AddFlag: invalid short flag \"" + short_flag + "\"");
  }
  const std::string lf = long_option_prefix + long_flag;
  const std::string sf = short_option_prefix + short_flag;
  const size_t id = GetNewFlagId();
  flag_ids[lf] = id;
  flag_ids[sf] = id;
  help_texts[id] = help_text;
  flag_fingerprint[id] = required_arguments;
  if(default_argument != ""){
    parsed_flag_map[id] = default_argument;
  }
}

bool CommandLine::IsFlagDefined(const std::string& flag) const{
  if(!parsed){
    return false;
  }
  size_t id = GetFlagIdFromName(flag);
  std::map<size_t, std::string>::const_iterator it = parsed_flag_map.find(id);
  return it != parsed_flag_map.end();
}

const std::string& CommandLine::GetFlagData(const std::string& flag, const Validator<std::string>& val) const{
  size_t id = GetFlagIdFromName(flag);
  std::map<size_t, std::string>::const_iterator it = parsed_flag_map.find(id);
  if(it == parsed_flag_map.end()){
    throw COMMAND_LINE_EXCEPTION("GetFlagData: Requested flag \""+flag+"\" undefined.");
  }
  const std::string& t = it->second;
  if(!val(t)){
    std::ostringstream os;
    os << "Argument of flag \"" << flag << "\": ";
    val.InvalidMessage(t, os);
    throw COMMAND_LINE_EXCEPTION(os.str());
  }
  return t;
}

int CommandLine::GetFlagDataAsInt(const std::string& flag, const Validator<int>& val) const {
  int t = string_to_int(GetFlagData(flag));
  if(!val(t)){
    std::ostringstream os;
    os << "Argument of flag \"" << flag << "\": ";
    val.InvalidMessage(t, os);
    throw COMMAND_LINE_EXCEPTION(os.str());
  }
  return t;
}

size_t CommandLine::GetFlagDataAsUint(const std::string& flag, const Validator<size_t>& val) const {
  size_t t = string_to_uint(GetFlagData(flag));
  if(!val(t)){
    std::ostringstream os;
    os << "Argument of flag \"" << flag << "\": ";
    val.InvalidMessage(t, os);
    throw COMMAND_LINE_EXCEPTION(os.str());
  }
  return t;
}


double CommandLine::GetFlagDataAsDouble(const std::string& flag, const Validator<double>& val) const {
  double t = string_to_double(GetFlagData(flag));
  if(!val(t)){
    std::ostringstream os;
    os << "Argument of flag \"" << flag << "\": ";
    val.InvalidMessage(t, os);
    throw COMMAND_LINE_EXCEPTION(os.str());
  }
  return t;
}

size_t CommandLine::CountFreeArguments() const{
  return parsed_free_arguments.size();
}

const std::string& CommandLine::GetFreeArgument(size_t n) const{
  const size_t m = CountFreeArguments();
  if(n > m-1){
    std::ostringstream os;
    os << "GetFreeArgument: "
       << Alternative(BasicRepresentable<std::string>("No"),
                      Combination(BasicRepresentable<std::string>("Only "),
                                  EnglishNumeral(m)), m==0)
       << " " << EnglishPluralize(BasicRepresentable<std::string>("argument"),m)
       << " given. Requested argument " << EnglishNumeral(n) << ".";
    throw COMMAND_LINE_EXCEPTION(os.str());
  }
  return parsed_free_arguments[n];
}

std::ostream& CommandLine::PrintHelpTexts(std::ostream& out) const {
  for(std::map<size_t, std::string>::const_iterator it = help_texts.begin();
      it != help_texts.end(); ++it)
  {
    const size_t id = it->first;
    std::vector<std::string> fs;
    for(std::map<std::string, size_t>::const_iterator j = flag_ids.begin();
        j != flag_ids.end(); ++j)
    {
      if(j->second == id){
        fs.push_back(j->first);
      }
    }
    out << join(fs, ", ")
        << " : " << help_texts.at(id) << "\n";
  }
  return out;
}

void CommandLine::SetVersionInformation(const std::string& program_name_,
                                        const std::string& program_description_,
                                        const std::string& program_version_,
                                        const std::string& program_copyright_)
{
  program_name = program_name_;
  program_description = program_description_;
  program_version = program_version_;
  program_copyright = program_copyright_;
}


std::ostream& CommandLine::PrintVersionInformation(std::ostream& out) const {
  out << program_name << " version " << program_version << "\n"
      << program_description << "\n"
      << program_copyright;
  return out;
}


void CommandLine::DefineAndParse(const char** const program_options, size_t length){
  // Add standard options
  AddFlag("Print this text", "help", "h");
  AddFlag("Print version information", "version", "v");

  for(size_t i=0; i<length; ++i){
    AddFlag(program_options[4*i + 0],
            program_options[4*i + 1],
            program_options[4*i + 2],
            (std::string(program_options[4*i + 3]) != "") ? 1 : 0,
            program_options[4*i + 3]
           );
  }
  Parse();
}


// -----------------------------------------------------------------------------

void DWIM_CommandLine(CommandLine& cl,
                      const std::string& program_name,
                      const std::string& program_description,
                      const std::string& program_version,
                      const std::string& program_copyright,
                      const char** const program_options,
                      size_t length)
{
  try{
    cl.SetVersionInformation(program_name, program_description,
                             program_version, program_copyright);
    cl.DefineAndParse(program_options, length);
    //cl.Parse();
  }
  catch(const CommandLineException& e){
    std::cerr << "Command line error:\n" << e << std::endl;
    exit(1);
  }

  if(cl.IsFlagDefined("-h")){
    std::cout << "Command line options:\n";
    cl.PrintHelpTexts(std::cout);
    std::cout.flush();
    exit(0);
  }

  if(cl.IsFlagDefined("-v")){
    cl.PrintVersionInformation(std::cout);
    std::cout << std::endl;
    exit(0);
  }

}


// CommandLine.cc ends here
