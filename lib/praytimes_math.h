#include <math.h>

#ifndef __PRAYTIMES_MATH_
#define __PRAYTIMES_MATH_

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

typedef enum PTM_SunDirection
{
  PTM_SD_CW,
  PTM_SD_CCW
} PTM_SunDirection_t;

/**
 * Get fixed angle value
 *
 * @param[in]  a  angle
 * @return        fixed angle
 **/
static inline double
PTM__fixAngle(double a)
{
  a = a - (360.0f * floor(a / 360.0f));
  return a < 0 ? a + 360.0f : a;
}

/**
 * Get fixed hour value
 *
 * @param[in]  a  hour
 * @return        fixed hour
 **/
static inline double
PTM__fixHour(double a)
{
  a = a - (24.0f * floor(a / 24.0f));
  return a < 0 ? a + 24.0f : a;
}

/**
 * Degree-based sin
 *
 * @param[in]  a
 * @return
 **/
static inline double
PTM__sin(double a)
{
  return sin((a * M_PI) / 180.0f);
}

/**
 * Degree-based cos
 *
 * @param[in]  a
 * @return
 **/
static inline double
PTM__cos(double a)
{
  return cos((a * M_PI) / 180.0f);
}

/**
 * Degree-based cos
 *
 * @param[in]  a
 * @return
 **/
static inline double
PTM__tan(double a)
{
  return tan((a * M_PI) / 180.f);
}

/**
 * Degree-based arcsin
 *
 * @param[in]  a
 * @return
 **/
static inline double
PTM__arcsin(double a)
{
  return (asin(a) * 180.0f) / M_PI;
}

/**
 * Degree-based arccos
 *
 * @param[in]  a
 * @return
 **/
static inline double
PTM__arccos(double a)
{
  return (acos(a) * 180.0f) / M_PI;
}

/**
 * Degree-based arccot
 *
 * @param[in]  a
 * @return
 **/
static inline double
PTM__arccot(double a)
{
  return (atan(1 / a) * 180.0f) / M_PI;
}

/**
 * Degree-based arctan2
 *
 * @param[in]  a
 * @param[in]  b
 * @return
 **/
static inline double
PTM__arctan2(double a, double b)
{
  return (atan2(a, b) * 180.0f) / M_PI;
}

/**
 * compute declination angle of sun
 *
 * Ref: http://aa.usno.navy.mil/faq/docs/SunApprox.php
 *
 * @param[in]  jd  Julian date
 * @return         Declination angle of sun
 **/
static inline double
PTM__sunPositionDeclination(const double jd)
{
  double D = jd - 2451545.0f;
  double g = PTM__fixAngle(357.529f + 0.98560028f * D);
  double q = PTM__fixAngle(280.459f + 0.98564736f * D);
  double L =
    PTM__fixAngle(q + (1.915f * PTM__sin(g)) + (0.020f * PTM__sin(2.0f * g)));

  double e = 23.439f - 0.00000036f * D;

  return PTM__arcsin(PTM__sin(e) * PTM__sin(L));
}

/**
 * compute equation of time
 *
 * Ref: http://aa.usno.navy.mil/faq/docs/SunApprox.php
 *
 * @param[in]  jd  Julian date
 * @return         Equation of time
 **/
static inline double
PTM__sunPositionEquation(const double jd)
{
  double D = jd - 2451545.0f;
  double g = PTM__fixAngle(357.529f + 0.98560028f * D);
  double q = PTM__fixAngle(280.459f + 0.98564736f * D);
  double L =
    PTM__fixAngle(q + (1.915f * PTM__sin(g)) + (0.020f * PTM__sin(2.0f * g)));

  double e = 23.439f - 0.00000036f * D;

  double RA = PTM__arctan2(PTM__cos(e) * PTM__sin(L), PTM__cos(L)) / 15.0f;

  return (q / 15.0) - PTM__fixHour(RA);
}

/**
 * compute mid-day time
 *
 * @param[in]  jDate
 * @param[in]  time
 * @return
 **/
static inline double
PTM__midDay(const double jDate, const double time)
{
  double eqt = PTM__sunPositionEquation(jDate + time);
  double noon = PTM__fixHour(12.0f - eqt);
  return noon;
}

/**
 * compute the time of given angle of sun
 *
 * @param[in]  jDate
 * @param[in]  angle
 * @param[in]  time
 * @param[in]  direction
 * @param[in]  lat
 * @return
 **/
static inline double
PTM__sunAngleTime(const double jDate,
                  const double angle,
                  const double time,
                  const PTM_SunDirection_t direction,
                  const double lat)
{
  double decl = PTM__sunPositionDeclination(jDate + time);
  double noon = PTM__midDay(jDate, time);
  double t = (1 / 15.0f) *
             PTM__arccos((-PTM__sin(angle) - (PTM__sin(decl) * PTM__sin(lat))) /
                         (PTM__cos(decl) * PTM__cos(lat)));
  return noon + (direction == PTM_SD_CCW ? -t : t);
}

/**
 * convert Gregorian date to Julian day
 * Ref: Astronomical Algorithms by Jean Meeus
 *
 * @param[in]  year
 * @param[in]  month
 * @param[in]  day
 * @return
 **/
static inline double
PTM__julianDay(const int year, const int month, const int day)
{
  int _year = year, _month = month;
  if (month <= 2) {
    _year -= 1;
    _month += 12;
  }

  double A = floor(_year / 100.0f);
  double B = 2 - A + floor(A / 4.0f);

  return floor(365.25f * (_year + 4716.0f)) + floor(30.6001f * (_month + 1)) +
         day + B - 1524.5f;
}

#endif
