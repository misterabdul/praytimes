#ifndef __PRAYTIMES_H
#define __PRAYTIMES_H

/**
 * PrayTimes struct data type.
 **/
typedef struct pt_t
{
} * PT;

/**
 * Time names
 **/
typedef enum PT_TimeNames
{
  PT_TN_IMSAK,
  PT_TN_FAJR,
  PT_TN_SUNRISE,
  PT_TN_DHUHR,
  PT_TN_ASR,
  PT_TN_SUNSET,
  PT_TN_MAGHRIB,
  PT_TN_ISHA,
  PT_TN_MIDNIGHT,
} PT_TimeName_t;

typedef double PT_PrayerTimes_t[PT_TN_MIDNIGHT + 1];

/**
 * Calculation methods
 **/
typedef enum PT_Methods
{
  PT_M_MWL,       /* Muslim World League */
  PT_M_ISNA,      /* Islamic Society of North America */
  PT_M_EGYPT,     /* Egyptian General Authority of Survey */
  PT_M_MAKKAH,    /* Umm Al-Qura University, Makkah */
  PT_M_KARACHI,   /* University of Islamic Science, Karachi */
  PT_M_TEHRAN,    /* Institue of Geophysics, University of Tehran */
  PT_M_JAFARI,    /* Shia Ithna-Ashari, Leva Institute, Qum */
  PT_M_INDONESIA, /* Kementrian Agama Republik Indonesia */
} PT_Method_t;

typedef enum PT_AsrJuristics
{
  PT_AJ_STANDARD, /* Shafi'i, Maliki, Ja'fari, Hanbali */
  PT_AJ_HANAFI,   /* Hanafi */
} PT_AsrJuristic_t;

typedef enum PT_MidnightMethods
{
  PT_MM_STANDARD, /* Mid Sunset to Sunrise */
  PT_MM_JAFARI,   /* Mid Sunset to Fajr */
} PT_MidnightMethod_t;

typedef enum PT_HighLatMethods
{
  PT_HL_NONE,         /* no adjustment */
  PT_HL_NIGHT_MIDDLE, /* middle of night */
  PT_HL_ANGLE_BASED,  /* angle/60th of night */
  PT_HL_ONE_SEVENTH,  /* 1/7 of the night */
} PT_HighLatMethod_t;

/**
 * Create new PrayTimes instance
 *
 * @return  PrayTimes instance
 **/
PT
PT__new(void);

/**
 * Free the memory containing PrayTimes instance
 *
 * @param[out]  pt  PrayTimes instance
 **/
void
PT__free(PT* pt);

/**
 * Set calculation method
 *
 * @param[out] pt      PrayTimes instance
 * @param[in]  method  Calculation method
 **/
void
PT__setMethod(PT pt, const PT_Method_t method);

/**
 * Set calculating parameters
 *
 * @param[out] pt        PrayTimes instance
 * @param[in]  imsak     Sun angle for imsak
 * @param[in]  fajr      Sun angle for fajr
 * @param[in]  asr       Asr juristics
 * @param[in]  dhuhr     Dhuhr time offset
 * @param[in]  maghrib   Sun angle for maghrib
 * @param[in]  isha      Sun angle for isha
 * @param[in]  midnight  Midnight mode
 * @param[in]  highlats  Method for higher latitutes
 **/
void
PT__adjust(PT pt,
           const double imsak,
           const double fajr,
           const double dhuhr,
           const PT_AsrJuristic_t asr,
           const double maghrib,
           const double isha,
           const PT_MidnightMethod_t midnight,
           const PT_HighLatMethod_t highlats);

/**
 * Set time offsets
 *
 * @param[out] pt       PrayTimes instance
 * @param[in]  offsets  Time offsets (in minutes)
 **/
void
PT__tune(PT pt, const double offsets);

/**
 * Get current calculation method
 *
 * @param[in]  pt  PrayTimes instance
 * @return          Current calculation method
 **/
PT_Method_t
PT__getMethod(const PT pt);

/**
 * Get current offsets
 *
 * @param[in]  pt  PrayTimes instance
 * @return          Current offsets
 **/
double
PT__getOffset(const PT pt);

/**
 * Get default calculation parameters
 *
 * @param[in]  pt  PrayTimes instance
 * @return          Default calculation parameters
 **/
int
PT__getDefaults(const PT pt);

/**
 * Return prayer times for a given date
 *
 * @param[in]  pt        PrayTimes instance
 * @param[out]  result    Prayer times result
 * @param[in]   date      Date
 * @param[in]   coords    Coordinate
 * @param[in]   timezone  Timezone
 * @param[in]   dst       Daylight saving time
 **/
void
PT__getTimes(const PT pt,
             PT_PrayerTimes_t result,
             const int year,
             const int month,
             const int day,
             const double lat,
             const double lng,
             const double elv,
             const int timezone,
             const int dst);

/**
 * Format the result time
 *
 * @param[in]   pt          PrayTimes instance
 * @param[in]   resultTime  Result time
 * @param[in]   format      Time format
 **/
char*
PT__formatTime(const PT pt, const double time, const char* format);

#endif
