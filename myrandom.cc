#include <cstdio>

#include "myrandom.h"

void MyRandom::setSeed (uint seed) {
  this->seed = seed;
}

uint MyRandom::getSeed () {
  return seed;
}

uint MyRandom::getRandomUint () {
  uint hi, lo;
  
  lo = 16807 * (seed & 0xffff);
  hi = 16807 * (seed >> 16);
  lo += (hi & 0x7fff) << 16;
  lo += hi >> 15;
  seed = (lo & 0x7FFFFFFF) + (lo >> 31);
  return seed;
}

uint MyRandom::getRandomUint (uint n) {
  return ((getRandomUint() & 0xffff) * n) >> 16;
}