// -*- mode: C++ -*-
// Time-stamp: "2012-10-21 18:01:23 sb"

/*
  file       OutputManipulator.hh
  copyright  (c) Sebastian Blatt 2012

  std::ostream manipulators that restore the original stream state
  after emitting their argument in a particular way.

 */


#ifndef OUTPUTMANIPULATOR_HH__F059345E_2A18_4FD8_85C8_5D78AA02D4FE
#define OUTPUTMANIPULATOR_HH__F059345E_2A18_4FD8_85C8_5D78AA02D4FE

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdint.h>

// Base class for custom output manipulators, implements
// UNWIND-PROTECT for stream format state.
class OutputManipulator {
  public:
    virtual std::ostream& output(std::ostream& out) const = 0;
    std::ostream& output_wrapper(std::ostream& out) const {
      if(out.good()){
        std::ios_base::fmtflags flags = out.flags();
        char f = out.fill();
        std::streamsize sz = out.precision();
        output(out);
        out.precision(sz);
        out.fill(f);
        out.flags(flags);
      }
      return out;
    }
};
inline std::ostream& operator<<(std::ostream& out, const OutputManipulator& o) {
  return o.output_wrapper(out);
}

// Print a hexadecimal number in 0x0001ABCD format, padded with zeros
// according to byte width of type T.
template <typename T>
class hex_form : public OutputManipulator {
  private:
    const T t;
    const size_t n;
  public:
    hex_form(T t_, size_t n_ = sizeof(T)) : t(t_), n(n_) {}
    std::ostream& output(std::ostream& out) const {
      out << "0x" << std::setw(2 * n) << std::setfill('0')
          << std::hex
      // need to convert uint8_t, otherwise will print as a character
      // instead of a number.
          << ((n > sizeof(uint8_t)) ? t : static_cast<unsigned int>(t));
      return out;
    }
};

// Print an unsigned integer in spaced hexadecimal form (12 ab 32 0c),
// padded with zeros according to byte width of type T.
template <typename T>
class hex_split_form : public OutputManipulator {
  private:
    const T t;
    const size_t n;
  public:
    hex_split_form(T t_, size_t n_ = sizeof(T)) : t(t_), n(n_) {}
    std::ostream& output(std::ostream& out) const {
      out.fill('0');
      for(size_t j=0; j<n; ++j){
        uint16_t byte = (t >> 8*(n-1-j)) & 0x00ff;
        out << std::setw(2) << std::hex << byte;
        if(j < n-1){
          out << " ";
        }
      }
      return out;
    }
};

// Print boolean as "y" or "n"
class bool_yn_form : public OutputManipulator {
  private:
    const bool b;
    const bool capitalize;
  public:
    bool_yn_form(bool b_, bool capitalize_=false)
      : b(b_), capitalize(capitalize_) {}
    std::ostream& output(std::ostream& out) const {
      out << (b ? (capitalize ? "Y" : "y") : (capitalize ? "N" : "n"));
      return out;
    }
};

// Print boolean as 1 or 0
class bool_numeric_form : public OutputManipulator {
  private:
    const bool b;
  public:
    bool_numeric_form(bool b_) : b(b_) {}
    std::ostream& output(std::ostream& out) const {
      out << (b ? "1" : "0");
      return out;
    }
};

// Right-justify type T using field width N and fill character FILL.
template <typename T>
class right_justified : public OutputManipulator {
  private:
    const T t;
    const size_t n;
    const char fill;
  public:
    right_justified(T t_, size_t n_, char fill_ = ' ')
      : t(t_), n(n_), fill(fill_)
    {}
    std::ostream& output(std::ostream& out) const {
      out.fill(fill);
      out << std::setw(n) << t;
      return out;
    }
};

// Print any iterable container with size() member function as a
// comma-separated list.
template <typename IterableContainer>
class list_form : public OutputManipulator {
  private:
    const IterableContainer& ic;
    const std::string& separator;
  public:
    list_form(const IterableContainer& ic_,
              const std::string& separator_ = ", ")
      : ic(ic_), separator(separator_)
    {}
    std::ostream& output(std::ostream& out) const {
      size_t j = 0;
      const size_t n = ic.size();
      for(typename IterableContainer::const_iterator i = ic.begin();
          i != ic.end(); ++i, ++j)
      {
        out << *i;
        if(j < n - 1){
          out << separator;
        }
      }
      return out;
    }
};

// Print std::vector as list
template <typename T>
class vector_form : public OutputManipulator {
  private:
    const std::vector<T>& v;
  public:
    vector_form(const std::vector<T>& v_)
      : v(v_)
    {}
    std::ostream& output(std::ostream& out) const {
      out << "(" << list_form< std::vector<T> >(v, ", ") << ")";
      return out;
    }
};


// Print std::vector of unsigned integers as list of hex numbers with addresses
template <typename T>
class hex_vector_form : public OutputManipulator {
  private:
    const std::vector<T>& v;
    const size_t w;
  public:
    hex_vector_form(const std::vector<T>& v_, size_t w_=20/sizeof(T))
      : v(v_), w(w_) {}
    std::ostream& output(std::ostream& out) const {
      for(size_t i=0; i<v.size(); ++i){
        if(i % w == 0){
          out << "  " << right_justified<size_t>(i, 6) << "  ";
        }
        out << hex_split_form<T>(v[i]);
        if(i % w == w-1){
          out << "\n";
        }
        else{
          out << ((sizeof(T)>1) ? "  " : " ");
        }
      }
      return out;
    }
};

// If numerator % denominator == 0, print (numerator/denominator),
// else print numerator << "/" << denominator.
template <typename T>
class fractional_form : public OutputManipulator {
  private:
    const T numerator;
    const T denominator;
  public:
    fractional_form(const T& numerator_,
                    const T& denominator_)
      : numerator(numerator_),
        denominator(denominator_)
    {}
    std::ostream& output(std::ostream& out) const {
      if(numerator % denominator == 0){
        out << numerator / denominator;
      }
      else{
        out << numerator << "/" << denominator;
      }
      return out;
    }
};

#endif // OUTPUTMANIPULATOR_HH__F059345E_2A18_4FD8_85C8_5D78AA02D4FE

// OutputManipulator.hh ends here
