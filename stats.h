#ifndef __STATS_H_
#define __STATS_H_

class Stats {

private:
  
  float playoutsNum;
  float playoutsScore;
  float squarePlayoutsScore;
  float ucb;

public:

  Stats () {
    playoutsNum = 0.0;
    playoutsScore = 0.0;
    squarePlayoutsScore = 0.0;
    ucb = 100.0;
  }
  
	float playNum();
  void resetStats ();
  void update (float playout);
  float mean ();
  float variance ();
  float updateUcb (float coeff);
  
};

#endif
