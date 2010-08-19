#include <cstdio>
#include <cmath>

#include "stats.h"


void Stats::resetStats () {
  playoutsNum = 0.0;
  playoutsScore = 0.0;
  squarePlayoutsScore = 0.0;
  ucb = START_UCB;
}

void Stats::update (float playoutScore) {
  playoutsNum += 1.0;
  playoutsScore += playoutScore;
  squarePlayoutsScore += playoutScore * playoutScore;
}

float Stats::mean () {
  return playoutsScore / playoutsNum;
}

float Stats::variance () {
  float m = mean();
  return squarePlayoutsScore / playoutsNum - m * m;
}

float Stats::updateUcb (float allPlayouts) {
  //tu mozna wstawic jakas stala przed pierwiastkiem
  ucb = mean() + STALA_UCB * sqrt(log(allPlayouts) / playoutsNum);
  return ucb;
}

float Stats::playNum () {
  return playoutsNum;
}
