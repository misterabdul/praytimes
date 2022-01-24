#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <praytimes.h>

int
main(int argc, char* argv[])
{
  PT pt = PT__new();
  PT_PrayerTimes_t results = {};
  PT__getTimes(pt, results, 2022, 1, 21, 3.583333, 97.666667, 0, 7, 0);
  char* imsak = PT__formatTime(pt, results[PT_TN_IMSAK], NULL);
  char* fajr = PT__formatTime(pt, results[PT_TN_FAJR], NULL);
  char* sunrise = PT__formatTime(pt, results[PT_TN_SUNRISE], NULL);
  char* dhuhr = PT__formatTime(pt, results[PT_TN_DHUHR], NULL);
  char* asr = PT__formatTime(pt, results[PT_TN_ASR], NULL);
  char* sunset = PT__formatTime(pt, results[PT_TN_SUNSET], NULL);
  char* maghrib = PT__formatTime(pt, results[PT_TN_MAGHRIB], NULL);
  char* isha = PT__formatTime(pt, results[PT_TN_ISHA], NULL);
  char* midnight = PT__formatTime(pt, results[PT_TN_MIDNIGHT], NULL);

  assert(strcmp(imsak, "05:19") == 0);
  assert(strcmp(fajr, "05:29") == 0);
  assert(strcmp(sunrise, "06:42") == 0);
  assert(strcmp(dhuhr, "12:40") == 0);
  assert(strcmp(asr, "16:04") == 0);
  assert(strcmp(sunset, "18:39") == 0);
  assert(strcmp(maghrib, "18:39") == 0);
  assert(strcmp(isha, "19:48") == 0);
  assert(strcmp(midnight, "00:40") == 0);

  printf("All test assertions passed...\n");

  /*
  printf("Imsak:    %s\n", imsak);
  printf("Fajr:     %s\n", fajr);
  printf("Sunrise:  %s\n", sunrise);
  printf("Dhuhr:    %s\n", dhuhr);
  printf("Asr:      %s\n", asr);
  printf("Sunset:   %s\n", sunset);
  printf("Magrib:   %s\n", maghrib);
  printf("Isha:     %s\n", isha);
  printf("Midnight: %s\n", midnight);
  */

  free(imsak);
  free(fajr);
  free(sunrise);
  free(dhuhr);
  free(asr);
  free(sunset);
  free(maghrib);
  free(isha);
  free(midnight);
  PT__free(&pt);

  return 0;
}
