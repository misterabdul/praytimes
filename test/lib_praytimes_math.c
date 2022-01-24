#include <assert.h>
#include <math.h>
#include <stdio.h>

#include <praytimes_math.h>

int
areSameF(double a, double b)
{
  return fabs(a - b) < __DBL_EPSILON__;
}

int
main(int argc, char* argv[])
{
  assert((int)(PTM__sin(90.0f) * 10000000) == 9999999);
  assert((int)(PTM__cos(90.0f) * 10000000) == 0);
  assert((int)(PTM__tan(45.0f) * 10000000) == 10000000);
  assert((int)(PTM__arcsin(1.0f) * 10000000) == 899999974);
  assert((int)(PTM__arccos(1.0f) * 10000000) == 0);
  assert((int)(PTM__arctan2(1.0f, 0.0f) * 10000000) == 899999974);
  assert((int)(PTM__arctan2(1.0f, 1.0f) * 10000000) == 449999987);
  assert((int)(PTM__arccot(1.0f) * 10000000) == 449999987);
  assert(PTM__fixAngle(90.0f) == 90.0f);
  assert(PTM__fixAngle(361.0f) == 1.0f);
  assert(PTM__fixHour(12.0f) == 12.0f);
  assert(PTM__fixHour(25.0f) == 1.0f);
  assert(PTM__julianDay(2022, 1, 20) == 2459599.5f);
  double jDate = PTM__julianDay(2022, 1, 20);
  assert((int)(PTM__sunPositionDeclination(jDate) * 10000000) == -201697033);
  assert((int)(PTM__sunPositionEquation(jDate) * 10000000) == -1810883);
  assert((int)(PTM__midDay(jDate, 0.5f) * 10000000) == 121835385);
  assert((int)(PTM__sunAngleTime(jDate, 10, 5, PTM_SD_CCW, 3.583333f) *
               10000000) == 55799308);

  printf("All test assertions passed...\n");

  return 0;
}
