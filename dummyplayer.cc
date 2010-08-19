#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "dummyplayer.h"
#include "myrandom.h"


//TODO: dorobic gre w kolor
int Dummyplayer::whoWon (int* card) {
  int ret = 0;
  for (int j = 1; j < 4; j++)
    if ((card[j]/13 == card[ret]/13) && (card[j] > card[ret]))
      ret = j;
  return ret;
}

int Dummyplayer::playCard1 (int who) {
  int s[4], set_num, ret;
  s[0] = players[who][0].size();
  s[1] = players[who][1].size();
  s[2] = players[who][2].size();
  s[3] = players[who][3].size();
  int card = a.getRandomUint(s[0] + s[1] + s[2] + s[3]);

  std::set < int >::iterator iter;
  set_num = 0;
  while (card >= s[set_num]) {
    card -= s[set_num];
    set_num++;
  }
  iter = players[who][set_num].begin();
  for (; card > 0; card--) iter++;
  ret = *(iter);
  players[who][set_num].erase(iter);
  return ret;
}

int Dummyplayer::playCard2(int who, int c1) {
  int suit = c1 / 13, index, ret;
  std::set<int>::iterator iter;

  if (!players[who][suit].empty()) { /* player has a card in this suit */
    index = a.getRandomUint(players[who][suit].size());
    iter = players[who][suit].begin();
    for (; index > 0; index--) iter++;
    ret = *(iter);
    players[who][suit].erase(iter);
    return ret;
  }

  /* player hasn't got a card in this suit */
  return playCard1(who);
}

//TODO
int Dummyplayer::playCard3(int who, int c1, int c2) { return 0; }

//TODO
int Dummyplayer::playCard4(int who, int c1, int c2, int c3) { return 0; }

//TODO
float Dummyplayer::play_randomly() {
  int card[4];
  int lastWinner = 0;

  for (int trick = 1; trick <= 13; trick++) {
    card[0] = playCard1(lastWinner);
    card[1] = playCard2((lastWinner + 1) % 4, card[0]);
    card[2] = playCard2((lastWinner + 2) % 4, card[0]);
    card[3] = playCard2((lastWinner + 3) % 4, card[0]);
    lastWinner = (lastWinner + whoWon(card)) % 4;
  }
  return 1.0;

}
