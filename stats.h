#ifndef __STATS_H_
#define __STATS_H_

class Stats {

private:
  
  float playouts_num;
  float playouts_score;
  float square_playouts_score;
  float ucb;

public:

  Stats () {
    playouts_num = 0.0;
    playouts_score = 0.0;
    square_playouts_score = 0.0;
    ucb = 100.0;
  }
  
  void resetStats ();
  void update (float playout);
  float mean ();
  float variance ();
  void updateUcb (float coeff);
  
};

#endif
