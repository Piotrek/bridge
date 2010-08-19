#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>
#include <ctime>

#include "myrandom.h"
#include "stats.h"
#include "uct.h"


using namespace std;


vector<vector< set<int> > > players;

MyRandom a = MyRandom(time(NULL));

int whoWon (int* card) {
  int ret = 0;
  for (int j = 1; j < 4; j++)
    if ((card[j]/13 == card[ret]/13) && (card[j] > card[ret]))
      ret = j;
  return ret;  
}

int playCard1(int who) {
  int s[4], set_num, ret;
  s[0] = players[who][0].size();
  s[1] = players[who][1].size();
  s[2] = players[who][2].size();
  s[3] = players[who][3].size();
  int card = a.getRandomUint(s[0] + s[1] + s[2] + s[3]);
  
  set<int>::iterator iter;
  set_num = 0;
  while (card >= s[set_num]) {
    card -= s[set_num];
    set_num++;
  }
  iter = players[who][set_num].begin();  
  for (; card > 0; card--) iter++;
//   cout<<"gracz "<<who<<" :"<<*(iter)<<endl;
  ret = *(iter);
  players[who][set_num].erase(iter);
  return ret;
}

int playCard2(int who, int c1) {
  int suit = c1 / 13, index, ret;
  set<int>::iterator iter;
  
  if (!players[who][suit].empty()) { /* player has a card in this suit */
    index = a.getRandomUint(players[who][suit].size());
    iter = players[who][suit].begin();  
    for (; index > 0; index--) iter++;
//     cout<<"gracz "<<who<<" :"<<*(iter)<<endl;
    ret = *(iter);
    players[who][suit].erase(iter);
    return ret;
  }
  
  /* player hasn't got a card in this suit */
  return playCard1(who);
}

int playCard3(int c1, int c2) { return 0; }

int playCard4(int c1, int c2, int c3) { return 0; }

void play_randomly() {
  int card[4];
  int lastWinner = 0;
  
  for (int trick = 1; trick <= 13; trick++) {
//     cout<<"lewa "<<trick<<endl;
    card[0] = playCard1(lastWinner);
    card[1] = playCard2((lastWinner + 1) % 4, card[0]);
    card[2] = playCard2((lastWinner + 2) % 4, card[0]);
    card[3] = playCard2((lastWinner + 3) % 4, card[0]);
    //card3 = playCard3(card1, card2);
    //card4 = playCard4(card1, card2, card3);
    lastWinner = (lastWinner + whoWon(card)) % 4;
// cout <<"Winner: "<<lastWinner<<endl;
  }
    
}

int main () {  
  clock_t pocz, koniec;
  
  players.resize(4);
  for (int i=0; i < 4; i++)
    players[i].resize(4);
  
  pocz = clock();
  for (int j = 0; j < 100000; j++) {
    for (int i=0; i < 52; i++)
      players[i%4][i/13].insert(i);

    play_randomly();
  }
  koniec = clock();
  cout<<(double)(koniec-pocz)/CLOCKS_PER_SEC<<endl;
}
