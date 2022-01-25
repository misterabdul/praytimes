#include <math.h>
#include <stdio.h>

#ifndef __UTILS_H
#define __UTILS_H

/**
 * Convert string to number (unsigned).
 *
 * @param[in]  str     The string
 * @param[in]  strlen  The length of string
 * @return             Converted number
 **/
static inline int
str2uint(const char* str, const int strlen)
{
  int result = 0, started = 0;
  for (int i = 0; i < strlen; i++)
    if (str[i] >= '0' && str[i] <= '9') {
      result = (result * 10) + (str[i] - '0');
      started = 1;
    } else if (started)
      break;
  return result;
}

/**
 * Convert string to number (signed).
 *
 * @param[in]  str     The string
 * @param[in]  strlen  The length of string
 * @return             Converted number
 **/
static inline int
str2int(const char* str, const int strlen)
{
  int result = 0, started = 0, sign = 1;
  for (int i = 0; i < strlen; i++)
    if (str[i] >= '0' && str[i] <= '9') {
      result = (result * 10) + (str[i] - '0');
      if (!started && i > 0 && str[i - 1] == '-')
        sign = -1;
      started = 1;
    } else if (started)
      break;
  return result * sign;
}


/**
 * Convert string to float number (signed).
 *
 * @param[in]  str     The string
 * @param[in]  strlen  The length of string
 * @return             Converted float number
 **/
static inline double
str2float(const char* str, const int strlen)
{
  int started = 0, floated = 0, sign = 1;
  double result = 0.0f;
  for (int i = 0; i < strlen; i++) {
    if (str[i] >= '0' && str[i] <= '9') {
      if (floated) {
        result = result + ((str[i] - '0') / pow(10, floated));
        floated++;
      } else
        result = (result * 10) + (str[i] - '0');
      if (!started && i > 0 && str[i - 1] == '-')
        sign = -1;
      started = 1;
    } else if (str[i] == '.') {
      if (floated)
        break;
      floated = 1;
    } else if (started)
      break;
  }
  return result * sign;
}

/**
 * Date increment
 *
 * @param[out]  year   Year value
 * @param[out]  month  Month value
 * @param[out]  day    Day value
 **/
static inline void
dateInc(int* year, int* month, int* day)
{
  if (*day == 28 && *month == 2) {
    if (*year % 4) {
      *day = 1;
      *month = *month + 1;
      return;
    }
    if (*year % 100) {
      *day = *day + 1;
      return;
    }
    if (*year % 400) {
      *day = 1;
      *month = *month + 1;
      return;
    }
  } else if (*day == 30) {
    switch (*month) {
      case 2:
      case 4:
      case 6:
      case 9:
      case 11:
        *month = *month + 1;
        *day = 1;
        return;
      default:
        break;
    }
  } else if (*day == 31) {
    if (*month != 12) {
      *month = *month + 1;
      *day = 1;
      return;
    }
    *year = *year + 1;
    *month = 1;
    *day = 1;
    return;
  }
  *day = *day + 1;
}

#endif
