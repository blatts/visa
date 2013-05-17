// -*- mode: C++ -*-
// Time-stamp: "2012-01-10 23:55:43 sb"

/*
  file       Representable.hh
  copyright  (c) Sebastian Blatt 2011, 2012

 */


#ifndef REPRESENTABLE_HH__631117C1_588D_4C64_99A4_6710708744CF
#define REPRESENTABLE_HH__631117C1_588D_4C64_99A4_6710708744CF

#include <iostream>

class Representable {
  public:
    virtual std::ostream& Represent(std::ostream& out) const = 0;
};

std::ostream& operator<<(std::ostream& out, const Representable& e);

template<typename T>
class BasicRepresentable : public Representable{
  private:
    const T& t;
  public:
    BasicRepresentable(const T& t_) : t(t_) {};
    std::ostream& Represent(std::ostream& out) const {
      out << t;
      return out;
    }
};

class Combination : public Representable {
  private:
    const Representable& a;
    const Representable& b;
  public:
    Combination(const Representable& a_, const Representable& b_)
      : a(a_), b(b_)
    {}
    std::ostream& Represent(std::ostream& out) const;
};

class Alternative : public Representable {
  private:
    const Representable& a;
    const Representable& b;
    const bool c;
  public:
    Alternative(const Representable& a_, const Representable& b_, bool c_)
      : a(a_), b(b_), c(c_)
    {}
    std::ostream& Represent(std::ostream& out) const;
};

class EnglishNumeral : public Representable{
  private:
    const size_t x;
  public:
    EnglishNumeral(const size_t x_) : x(x_){}
    std::ostream& Represent(std::ostream& out) const;
};

class EnglishPluralize : public Representable{
  private:
    const Representable& s;
    const size_t m;
  public:
    EnglishPluralize(const Representable& s_, size_t m_) : s(s_), m(m_){}
    std::ostream& Represent(std::ostream& out) const;
};

class TimeNow : public Representable {
  public:
    TimeNow() {}
    std::ostream& Represent(std::ostream& out) const;
};



#endif // REPRESENTABLE_HH__631117C1_588D_4C64_99A4_6710708744CF

// Representable.hh ends here
