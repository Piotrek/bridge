#include <cstdio>
#include <cmath>

#include "stats.h"


void Stats::resetStats () {
  playouts_num = 0.0;
  playouts_score = 0.0;
  square_playouts_score = 0.0;
  ucb = 100.0;
}

void Stats::update (float playout) {
  playouts_num += 1.0;
  playouts_score += playout;
  square_playouts_score += playout * playout;
}

float Stats::mean () {
  return playouts_score / playouts_num;
}

float Stats::variance () {
  float m = mean();
  return square_playouts_score / playouts_num - m * m;
}

void Stats::updateUcb (float coeff) {
  ucb = mean() + coeff / sqrt(playouts_num);
}