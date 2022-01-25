#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include <praytimes.h>

int
main(int argc, char* argv[])
{
  int year = 0, month = 1, day = 1, tmz = 0, dst = 0, n = 1;
  int detailed = 0;
  double lat = 0.0f, lng = 0.0f, elv = 0.0f;
  for (int i = 0; i < argc; i++) {
    if (strncmp(argv[i], "--year=", 7) == 0)
      year = str2uint(argv[i], strlen(argv[i]));
    if (strncmp(argv[i], "--month=", 8) == 0)
      month = str2uint(argv[i], strlen(argv[i]));
    if (strncmp(argv[i], "--day=", 6) == 0)
      day = str2uint(argv[i], strlen(argv[i]));
    if (strncmp(argv[i], "--timezone=", 11) == 0)
      tmz = str2int(argv[i], strlen(argv[i]));
    if (strncmp(argv[i], "--dst=", 6) == 0)
      dst = str2int(argv[i], strlen(argv[i]));
    if (strncmp(argv[i], "--lat=", 6) == 0)
      lat = str2float(argv[i], strlen(argv[i]));
    if (strncmp(argv[i], "--long=", 7) == 0)
      lng = str2float(argv[i], strlen(argv[i]));
    if (strncmp(argv[i], "--elevation=", 12) == 0)
      elv = str2float(argv[i], strlen(argv[i]));
    if (strncmp(argv[i], "--n=", 4) == 0)
      n = str2uint(argv[i], strlen(argv[i]));
    if (strncmp(argv[i], "--detailed", 10) == 0)
      detailed = 1;
  }

  PT pt = PT__new();
  PT_PrayerTimes_t results;
  PT__setMethod(pt, PT_M_INDONESIA);
  PT__tune(pt, 2.0f);
  if (detailed)
    printf("Date       "
           "Imsak "
           "Fajr  "
           "Sunrise "
           "Dhuhr "
           "Asr   "
           "Sunset "
           "Maghrib "
           "Isha  "
           "Midnight\n");
  else
    printf("Date       "
           "Imsak "
           "Fajr  "
           "Dhuhr "
           "Asr   "
           "Maghrib "
           "Isha\n");

  for (int i = 0; i < n; i++) {
    PT__getTimes(pt, results, year, month, day, lat, lng, elv, tmz, dst);
    char* imsak = PT__formatTime(pt, results[PT_TN_IMSAK], NULL);
    char* fajr = PT__formatTime(pt, results[PT_TN_FAJR], NULL);
    char* sunrise = PT__formatTime(pt, results[PT_TN_SUNRISE], NULL);
    char* dhuhr = PT__formatTime(pt, results[PT_TN_DHUHR], NULL);
    char* asr = PT__formatTime(pt, results[PT_TN_ASR], NULL);
    char* sunset = PT__formatTime(pt, results[PT_TN_SUNSET], NULL);
    char* maghrib = PT__formatTime(pt, results[PT_TN_MAGHRIB], NULL);
    char* isha = PT__formatTime(pt, results[PT_TN_ISHA], NULL);
    char* midnight = PT__formatTime(pt, results[PT_TN_MIDNIGHT], NULL);

    if (detailed)
      printf("%04d-%02d-%02d %s %s %s   %s %s %s  %s   %s %s\n",
             year,
             month,
             day,
             imsak,
             fajr,
             sunrise,
             dhuhr,
             asr,
             sunset,
             maghrib,
             isha,
             midnight);
    else
      printf("%04d-%02d-%02d %s %s %s %s %s   %s\n",
             year,
             month,
             day,
             imsak,
             fajr,
             dhuhr,
             asr,
             maghrib,
             isha);

    free(imsak);
    free(fajr);
    free(sunrise);
    free(dhuhr);
    free(asr);
    free(sunset);
    free(maghrib);
    free(isha);
    free(midnight);

    dateInc(&year, &month, &day);
  }

  PT__free(&pt);
  return 0;
}
