#ifndef __MYRANDOM_H__
#define __MYRANDOM_H__

typedef unsigned int uint;
 
class MyRandom {
  private:
    uint seed;
    
  public:
 
    MyRandom (uint _seed) : seed(_seed) { };
    void setSeed (uint seed);
    uint getSeed ();
 
    uint getRandomUint ();
    uint getRandomUint (uint n);
};

#endif