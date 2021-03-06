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
  if (playoutsNum > 0)
    ucb = mean() + STALA_UCB * sqrt(log(allPlayouts) / playoutsNum);
  else
    ucb = START_UCB;
  return ucb;
}

float Stats::playNum () {
  return playoutsNum;
}
