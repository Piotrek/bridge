#include <cstdio>
#include <cmath>

#include "stats.h"


void Stats::resetStats () {
  playoutsNum = 0.0;
  playoutsScore = 0.0;
  squarePlayoutsScore = 0.0;
  ucb = 100.0;
}

void Stats::update (float playout) {
  playoutsNum += 1.0;
  playoutsScore += playout;
  squarePlayoutsScore += playout * playout;
}

float Stats::mean () {
  return playoutsScore / playoutsNum;
}

float Stats::variance () {
  float m = mean();
  return squarePlayoutsScore / playoutsNum - m * m;
}

float Stats::updateUcb (float coeff) {
  ucb = mean() + sqrt(log(coeff) / playoutsNum);
	return ucb;
}

float Stats::playNum () {
	return playoutsNum;
}