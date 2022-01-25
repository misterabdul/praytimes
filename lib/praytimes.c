#include <math.h>
#include <stdlib.h>

#include "praytimes.h"
#include "praytimes_math.h"

/**
 * PrayTimes's settings struct data type.
 **/
typedef struct private_pt_settings_t
{
  double imsak;
  double fajr;
  double dhuhr;
  PT_ArsJuristic_t asr;
  double maghrib;
  double isha;
  PT_MidnightMethod_t midnight;
  PT_HighLatMethod_t highlats;
} PT_Settings_t;

typedef double PT_Offsets_t[PT_TN_MIDNIGHT + 1];

typedef double PT_Times_t[PT_TN_MIDNIGHT + 1];

/**
 * Real PrayTimes struct data type.
 **/
typedef struct private_pt_t
{
  PT_Method_t method;
  PT_Settings_t settings;
  PT_Offsets_t offsets;

  double offset;
} * PrivatePT;

PT
PT__new(void)
{
  PrivatePT pt = malloc(sizeof(struct private_pt_t));
  pt->method = PT_M_MWL;
  pt->settings.imsak = 10.0f;
  pt->settings.fajr = 18.0f;
  pt->settings.dhuhr = 0.0f;
  pt->settings.asr = PT_AJ_STANDARD;
  pt->settings.maghrib = 0.0f;
  pt->settings.isha = 17.0f;
  pt->settings.midnight = PT_MM_STANDARD;
  pt->settings.highlats = PT_HL_NIGHT_MIDDLE;

  return (PT)pt;
}

void
PT__free(PT* pt)
{
  free(*pt);
  *pt = NULL;
}

void
PT__setMethod(PT pt, const PT_Method_t method)
{
  PrivatePT _pt = (PrivatePT)pt;
  _pt->method = method;
  switch (method) {
    default:
    case PT_M_MWL:
      _pt->settings.fajr = 18.0f;
      _pt->settings.isha = 17.0f;
      break;
    case PT_M_ISNA:
      _pt->settings.fajr = 15.0f;
      _pt->settings.isha = 15.0f;
      break;
    case PT_M_EGYPT:
      _pt->settings.fajr = 19.5f;
      _pt->settings.isha = 17.5f;
      break;
    case PT_M_MAKKAH:
      _pt->settings.fajr = 18.5f;
      _pt->settings.isha = 90.0;
      break;
    case PT_M_KARACHI:
      _pt->settings.fajr = 18.0f;
      _pt->settings.isha = 18.0f;
      break;
    case PT_M_TEHRAN:
      _pt->settings.fajr = 17.7f;
      _pt->settings.isha = 14.0f;
      _pt->settings.maghrib = 4.5f;
      _pt->settings.midnight = PT_MM_JAFARI;
      break;
    case PT_M_JAFARI:
      _pt->settings.fajr = 16.0f;
      _pt->settings.isha = 14.0f;
      _pt->settings.maghrib = 4.5f;
      _pt->settings.midnight = PT_MM_JAFARI;
      break;
    case PT_M_INDONESIA:
      _pt->settings.fajr = 20.0f;
      _pt->settings.isha = 18.0f;
  }
}

void
PT__adjust(PT pt,
           const double imsak,
           const double fajr,
           const double dhuhr,
           const PT_ArsJuristic_t asr,
           const double maghrib,
           const double isha,
           const PT_MidnightMethod_t midnight,
           const PT_HighLatMethod_t highlats)
{
  PrivatePT _pt = (PrivatePT)pt;
  _pt->settings.imsak = imsak;
  _pt->settings.fajr = fajr;
  _pt->settings.dhuhr = dhuhr;
  _pt->settings.asr = asr;
  _pt->settings.maghrib = maghrib;
  _pt->settings.isha = isha;
  _pt->settings.midnight = midnight;
  _pt->settings.highlats = highlats;
}

void
PT__tune(PT pt, const double offsets)
{
  PrivatePT _pt = (PrivatePT)pt;
  _pt->offsets[PT_TN_IMSAK] = offsets;
  _pt->offsets[PT_TN_FAJR] = offsets;
  _pt->offsets[PT_TN_SUNRISE] = offsets;
  _pt->offsets[PT_TN_DHUHR] = offsets;
  _pt->offsets[PT_TN_ASR] = offsets;
  _pt->offsets[PT_TN_SUNSET] = offsets;
  _pt->offsets[PT_TN_MAGHRIB] = offsets;
  _pt->offsets[PT_TN_ISHA] = offsets;
  _pt->offsets[PT_TN_MIDNIGHT] = offsets;
}

PT_Method_t
PT__getMethod(const PT pt)
{
  PrivatePT _pt = (PrivatePT)pt;
  return _pt->method;
}

double
PT__getOffset(const PT pt)
{
  PrivatePT _pt = (PrivatePT)pt;
  return _pt->offset;
}

/**
 * Calculate asr time
 *
 * @param[in]  jDate
 * @param[in]  arsJuristic
 * @param[in]  time
 * @param[in]  lat
 * @return
 **/
static inline double
PT__asrTime(const double jDate,
            const PT_ArsJuristic_t asrJuristic,
            const double time,
            const double lat)
{
  double asrFactor = asrJuristic == PT_AJ_STANDARD ? 1.0f : 2.0f;
  double decl = PTM__sunPositionDeclination(jDate + time);
  double angle = -PTM__arccot(asrFactor + PTM__tan(fabs(lat - decl)));
  return PTM__sunAngleTime(jDate, angle, time, PTM_SD_CW, lat);
}

/**
 * Adjust time for higher lattitude
 *
 * @param[in]  method
 * @param[in]  time
 * @param[in]  base
 * @param[in]  angle
 * @param[in]  night
 * @param[in]  direction
 * @return
 **/
static inline double
PT__adjustHLTime(const PT_HighLatMethod_t method,
                 const double time,
                 const double base,
                 const double angle,
                 const double night,
                 const PTM_SunDirection_t direction)
{
  double portion;
  switch (method) {
    default:
    case PT_HL_NONE:
      portion = (1 / 2.0f) * night;
      break;
    case PT_HL_ANGLE_BASED:
      portion = (1 / 60.0f) * angle * night;
      break;
    case PT_HL_ONE_SEVENTH:
      portion = (1 / 7.0f) * night;
      break;
  }

  double timeDiff = direction == PTM_SD_CCW ? PTM__fixHour(base - time)
                                            : PTM__fixHour(time - base);
  double _time = time;
  if (isnan(time) || timeDiff > portion)
    _time = base + (direction == PTM_SD_CCW ? -portion : portion);
  return _time;
}

/**
 * Adjust for higher latitude
 *
 * @param[in]  pt
 * @param[out]  times
 **/
static inline void
PT__adjustHighLats(const PrivatePT pt, PT_PrayerTimes_t times)
{
  double nightTime = PTM__fixHour(times[PT_TN_SUNRISE] - times[PT_TN_SUNSET]);
  times[PT_TN_IMSAK] = PT__adjustHLTime(pt->settings.highlats,
                                        times[PT_TN_IMSAK],
                                        times[PT_TN_SUNRISE],
                                        pt->settings.imsak,
                                        nightTime,
                                        PTM_SD_CCW);
  times[PT_TN_FAJR] = PT__adjustHLTime(pt->settings.highlats,
                                       times[PT_TN_FAJR],
                                       times[PT_TN_SUNRISE],
                                       pt->settings.fajr,
                                       nightTime,
                                       PTM_SD_CCW);
  times[PT_TN_ISHA] = PT__adjustHLTime(pt->settings.highlats,
                                       times[PT_TN_ISHA],
                                       times[PT_TN_SUNSET],
                                       pt->settings.isha,
                                       nightTime,
                                       PTM_SD_CW);
  times[PT_TN_MAGHRIB] = PT__adjustHLTime(pt->settings.highlats,
                                          times[PT_TN_MAGHRIB],
                                          times[PT_TN_SUNSET],
                                          pt->settings.maghrib,
                                          nightTime,
                                          PTM_SD_CW);
}

/**
 * Compute prayer times
 *
 * @param[in]  pt
 * @param[out]  results
 * @param[in]   lat
 * @param[in]   jDate
 * @param[in]   riseSetAngle
 * @param[in]   timeAdjust
 **/
static inline void
PT__computeTimes(const PrivatePT pt,
                 PT_PrayerTimes_t results,
                 const double lat,
                 const double jDate,
                 const double riseSetAngle,
                 const double timeAdjust)
{
  PT_Times_t defaultTimes = { 5 / 24.0f,    /* Imsak */
                              5 / 24.0f,    /* Fajr */
                              6 / 24.0f,    /* Sunrise */
                              12 / 24.0f,   /* Dhuhr */
                              13 / 24.0f,   /* Asr */
                              18 / 24.0f,   /* Sunset */
                              18 / 24.0f,   /* Maghrib */
                              18 / 24.0f }; /* Isha */

  results[PT_TN_IMSAK] =
    PTM__sunAngleTime(
      jDate, pt->settings.imsak, defaultTimes[PT_TN_IMSAK], PTM_SD_CCW, lat) +
    timeAdjust;
  results[PT_TN_FAJR] =
    PTM__sunAngleTime(
      jDate, pt->settings.fajr, defaultTimes[PT_TN_FAJR], PTM_SD_CCW, lat) +
    timeAdjust;
  results[PT_TN_SUNRISE] =
    PTM__sunAngleTime(
      jDate, riseSetAngle, defaultTimes[PT_TN_SUNRISE], PTM_SD_CCW, lat) +
    timeAdjust;
  results[PT_TN_DHUHR] =
    PTM__midDay(jDate, defaultTimes[PT_TN_DHUHR]) + timeAdjust;
  results[PT_TN_ASR] =
    PT__asrTime(jDate, pt->settings.asr, defaultTimes[PT_TN_ASR], lat) +
    timeAdjust;
  results[PT_TN_SUNSET] =
    PTM__sunAngleTime(
      jDate, riseSetAngle, defaultTimes[PT_TN_SUNSET], PTM_SD_CW, lat) +
    timeAdjust;
  results[PT_TN_MAGHRIB] = PTM__sunAngleTime(jDate,
                                             pt->settings.maghrib,
                                             defaultTimes[PT_TN_MAGHRIB],
                                             PTM_SD_CW,
                                             lat) +
                           timeAdjust;
  results[PT_TN_ISHA] =
    PTM__sunAngleTime(
      jDate, pt->settings.isha, defaultTimes[PT_TN_ISHA], PTM_SD_CW, lat) +
    timeAdjust;
}

/**
 * Adjust prayer times
 *
 * @param[in]   pt
 * @param[out]  results
 **/
static inline void
PT__adjustTimes(const PrivatePT pt, PT_PrayerTimes_t results)
{
  results[PT_TN_IMSAK] = results[PT_TN_FAJR] - (pt->settings.imsak / 60.0f);
  if (pt->method != PT_M_TEHRAN && pt->method != PT_M_JAFARI)
    results[PT_TN_MAGHRIB] =
      results[PT_TN_SUNSET] + (pt->settings.maghrib / 60.0f);
  if (pt->method == PT_M_MAKKAH)
    results[PT_TN_ISHA] = results[PT_TN_MAGHRIB] + (pt->settings.isha / 60.0f);
  results[PT_TN_DHUHR] += (pt->settings.dhuhr / 60.0f);
}

/**
 * Compute midnight time
 *
 * @param[in]   pt
 * @param[out]  results
 **/
static inline void
PT__computeMidnight(const PrivatePT pt, PT_PrayerTimes_t results)
{
  results[PT_TN_MIDNIGHT] =
    pt->settings.midnight == PT_MM_JAFARI
      ? results[PT_TN_SUNSET] +
          (PTM__fixHour(results[PT_TN_FAJR] - results[PT_TN_SUNSET]) / 2.0f)
      : results[PT_TN_SUNSET] +
          (PTM__fixHour(results[PT_TN_SUNRISE] - results[PT_TN_SUNSET]) / 2.0f);
}

/**
 * Tune prayer times
 *
 * @param[in]   pt
 * @param[out]  results
 **/
static inline void
PT__tuneTimes(const PrivatePT pt, PT_PrayerTimes_t results)
{
  results[PT_TN_IMSAK] += (pt->offsets[PT_TN_IMSAK] / 60.0f);
  results[PT_TN_FAJR] += (pt->offsets[PT_TN_FAJR] / 60.0f);
  results[PT_TN_SUNRISE] += (pt->offsets[PT_TN_SUNRISE] / 60.0f);
  results[PT_TN_DHUHR] += (pt->offsets[PT_TN_DHUHR] / 60.0f);
  results[PT_TN_ASR] += (pt->offsets[PT_TN_ASR] / 60.0f);
  results[PT_TN_SUNSET] += (pt->offsets[PT_TN_SUNSET] / 60.0f);
  results[PT_TN_MAGHRIB] += (pt->offsets[PT_TN_MAGHRIB] / 60.0f);
  results[PT_TN_ISHA] += (pt->offsets[PT_TN_ISHA] / 60.0f);
  results[PT_TN_MIDNIGHT] += (pt->offsets[PT_TN_MIDNIGHT] / 60.0f);
}

void
PT__getTimes(const PT pt,
             PT_PrayerTimes_t results,
             const int year,
             const int month,
             const int day,
             const double lat,
             const double lng,
             const double elv,
             const int timezone,
             const int dst)
{
  PrivatePT _pt = (PrivatePT)pt;
  int jDate = PTM__julianDay(year, month, day);
  double riseSetAngle = 0.833f + (0.0347f * sqrt(elv));
  double timeAdjust = (double)(timezone + dst) - (lng / 15.0f);

  PT__computeTimes(_pt, results, lat, jDate, riseSetAngle, timeAdjust);

  if (_pt->settings.highlats != PT_HL_NONE)
    PT__adjustHighLats(_pt, results);

  PT__adjustTimes(_pt, results);

  PT__computeMidnight(_pt, results);

  PT__tuneTimes(_pt, results);
}

char*
PT__formatTime(const PT pt, const double resultTime, const char* format)
{
  const double time = PTM__fixHour(resultTime + (1 / 180.0f));
  const int hours = (int)floor(time);
  const int minutes = (int)floor((time - hours) * 60);
  char* formatted = malloc(6 * sizeof(char));
  formatted[0] = '0' + (hours / 10);
  formatted[1] = '0' + (hours % 10);
  formatted[2] = ':';
  formatted[3] = '0' + (minutes / 10);
  formatted[4] = '0' + (minutes % 10);
  formatted[5] = '\0';

  return formatted;
}
