// Test definitions for the Julian day calculations
#include "Julian.hh"
// Google Test
#include <gtest/gtest.h>

using namespace TC;


TEST(Julian, UnixEpoch)
{
  int jd = ToJulianDay(1, 1, 1970);
  EXPECT_EQ(jd, 2440588);

  int day = 0, month = 0, year = 0;
  FromJulianDay(jd, day, month, year);
  EXPECT_EQ(day,   1);
  EXPECT_EQ(month, 1);
  EXPECT_EQ(year,  1970);
}

TEST(Julian, J2000)
{
  EXPECT_EQ(ToJulianDay(1, 1, 2000), 2451545);
}

TEST(Julian, LeapDay)
{
  // Feb 29 and Mar 1 must be consecutive
  EXPECT_EQ(ToJulianDay(29, 2, 2000), 2451604);
  EXPECT_EQ(ToJulianDay( 1, 3, 2000), 2451605);
}

TEST(Julian, YearBoundary)
{
  // Dec 31 and Jan 1 must be consecutive
  EXPECT_EQ(ToJulianDay(31, 12, 1999), 2451544);
  EXPECT_EQ(ToJulianDay( 1,  1, 2000), 2451545);
}

TEST(Julian, RoundTrip)
{
  // FromJulianDay must invert ToJulianDay for a sample of dates
  int dates[][3] = {
    {15,  6, 1582},  // near Gregorian adoption
    { 1,  3, 1900},
    {28,  2, 2100},  // century non-leap year
  };
  for (auto& d : dates) {
    int jd = ToJulianDay(d[0], d[1], d[2]);
    int day = 0, month = 0, year = 0;
    FromJulianDay(jd, day, month, year);
    EXPECT_EQ(day,   d[0]);
    EXPECT_EQ(month, d[1]);
    EXPECT_EQ(year,  d[2]);
  }
}
