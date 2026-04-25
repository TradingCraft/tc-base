#pragma once

#include <ctime>


namespace TC {

// Julian Day calculation source (Fliegel & Van Flandern, 1968):
// https://aa.usno.navy.mil/faq/JD_formula

inline int ToJulianDay(int day, int month, int year)
{
  int I = year;
  int J = month;
  int K = day;
  int jd = K-32075+1461*(I+4800+(J-14)/12)/4+367*(J-2-(J-14)/12*12)/12-3*((I+4900+(J-14)/12)/100)/4;
  return jd;
}


inline void FromJulianDay(int jd, int& day, int& month, int& year)
{
  int L= jd+68569;
  int N= 4*L/146097;
  L = L-(146097*N+3)/4;
  int I= 4000*(L+1)/1461001;
  L = L-1461*I/4+31;
  int J= 80*L/2447;
  int K= L-2447*J/80;
  L= J/11;
  J= J+2-12*L;
  I= 100*(N-49)+I+L;

  year = I;
  month = J;
  day = K;
}


inline int CurrentJulianDay()
{
  std::time_t currTime = std::time(nullptr);
  std::tm buf{};
#if defined(_WIN32)
  localtime_s(&buf, &currTime);
#else
  localtime_r(&currTime, &buf);
#endif
  return ToJulianDay(buf.tm_mday, buf.tm_mon+1, buf.tm_year+1900);
}


//! Calculate day of the week for a given Julian day
inline int DayOfWeek(int julianDay)
{
  return (julianDay + 1) % 7;
}

} // namespace
