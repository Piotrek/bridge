#ifndef __STATS_H_
#define __STATS_H_

#include <cstdio>
#include <cmath>
#include <algorithm>

#define STALA_UCB 1.0
#define START_UCB 100.0

class Stats {

private:
  
  float playoutsNum;
  float playoutsScore;
  float ucb;

public:

  Stats () {
    playoutsNum = 0.0;
    playoutsScore = 0.0;
    ucb = 100.0;
  }
  
  float playNum();
  void resetStats ();
  void update (float playoutScore);
  float mean ();
  float updateUcb (float allPlayouts);
  float getPlayoutsScore() {return playoutsScore; };
};

#endif
