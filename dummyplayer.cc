#include "dummyplayer.h"


int Dummyplayer::playCard1 (int who) {
  int s[4], set_num, ret;
  s[0] = cards[who][0].size();
  s[1] = cards[who][1].size();
  s[2] = cards[who][2].size();
  s[3] = cards[who][3].size();
  int card = a.getRandomUint(s[0] + s[1] + s[2] + s[3]);

  std::set < int >::iterator iter;
  set_num = 0;
  while (card >= s[set_num]) {
    card -= s[set_num];
    set_num++;
  }
  iter = cards[who][set_num].begin();
  for (; card > 0; card--) iter++;
  ret = *(iter);
  cards[who][set_num].erase(iter);
  return ret;
}

int Dummyplayer::playCard2(int who, int c1) {
  int suit = c1 / 13, index, ret;
  std::set<int>::iterator iter;

  if (!cards[who][suit].empty()) { /* player has a card in this suit */
    index = a.getRandomUint(cards[who][suit].size());
    iter = cards[who][suit].begin();
    for (; index > 0; index--) iter++;
    ret = *(iter);
    cards[who][suit].erase(iter);
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
int Dummyplayer::play_randomly(int tricksWon, int lastWinner) {
  
  if (cardsInTrick > 0) {
    if (cardsInTrick == 1) {
      currentTrick.playCard(playCard2((lastWinner + 1) % 4, currentTrick.firstCard()), 1);
    }
    if (cardsInTrick <= 2) {
      currentTrick.playCard(playCard2((lastWinner + 2) % 4, currentTrick.firstCard()), 2);
    }
    if (cardsInTrick <= 3) {
      currentTrick.playCard(playCard2((lastWinner + 3) % 4, currentTrick.firstCard()), 3);
    }
    lastWinner = (lastWinner + currentTrick.whoWon(contractSuit)) % 4;
    tricksWon += (lastWinner % 2);
  }
    
  while (!(cards[N][C].empty() && cards[N][D].empty() && cards[N][H].empty() && cards[N][Sp].empty())) {
    currentTrick.playCard(playCard1(lastWinner), 0);
    currentTrick.playCard(playCard2((lastWinner + 1) % 4, currentTrick.firstCard()), 1);
    currentTrick.playCard(playCard2((lastWinner + 2) % 4, currentTrick.firstCard()), 2);
    currentTrick.playCard(playCard2((lastWinner + 3) % 4, currentTrick.firstCard()), 3);
    lastWinner = (lastWinner + currentTrick.whoWon(contractSuit)) % 4;
    tricksWon += (lastWinner % 2);
    //currentTrick.printTrick();
  }
  
  return tricksWon;
}




int Betterplayer::playCard1 (int who) {
  int s[4], set_num, ret;
  s[0] = cards[who][0].size();
  s[1] = cards[who][1].size();
  s[2] = cards[who][2].size();
  s[3] = cards[who][3].size();
  int card = a.getRandomUint(s[0] + s[1] + s[2] + s[3]);

  std::set < int >::iterator iter;
  set_num = 0;
  while (card >= s[set_num]) {
    card -= s[set_num];
    set_num++;
  }
  iter = cards[who][set_num].begin();
  for (; card > 0; card--) iter++;
  ret = *(iter);
  cards[who][set_num].erase(iter);
  return ret;
}

int Betterplayer::playCard2(int who, int c1) {
  int suit = c1 / 13, index, ret;
  std::set<int>::iterator iter;

  if (!cards[who][suit].empty()) { /* player has a card in this suit */
    index = a.getRandomUint(cards[who][suit].size()*2);
    iter = cards[who][suit].begin();
    if (index < (int) cards[who][suit].size()) /* with prob 50% we play lowest card */
       for (; index > 0; index--) iter++;
    ret = *(iter);
    cards[who][suit].erase(iter);
    return ret;
  }
  
  index = a.getRandomUint(2); /* with prob 50% we ruff */
  if ((index == 0) && (contractSuit < 4) && (!cards[who][contractSuit].empty())) {
    ret = *(cards[who][contractSuit].begin());
    cards[who][contractSuit].erase(cards[who][contractSuit].begin());
    return ret;
  }

  /* player hasn't got a card in this suit */
  return playCard1(who);
}

int Betterplayer::playCard3 (int who, int c1, int c2) {
  int suit = c1 / 13, index, ret;
  std::set<int>::iterator iter;

  if (!cards[who][suit].empty()) { /* player has a card in this suit */
    index = a.getRandomUint(cards[who][suit].size()*2);
    iter = cards[who][suit].begin();
    if (index < (int) cards[who][suit].size()) /* with prob 50% we play lowest card */
       for (; index > 0; index--) iter++;
    ret = *(iter);
    cards[who][suit].erase(iter);
    return ret;
  }
  
  index = a.getRandomUint(2); /* with prob 50% we ruff */
  if ((index == 0) && (contractSuit < 4) && (!cards[who][contractSuit].empty())) {
    ret = *(cards[who][contractSuit].begin());
    cards[who][contractSuit].erase(cards[who][contractSuit].begin());
    return ret;
  }

  /* player hasn't got a card in this suit */
  return playCard1(who);
}




int Betterplayer::playRandomSmallCard(int who, int trumps) {
  int i = 0, ret, index = 0, index2, suit = 0;
  std::set<int>::iterator iter;

  for (i = 0; i < 4; i++) if ((!cards[who][i].empty()) && (i != trumps)) index++;
  /* index - how many different suits (non-trumps) a player has */
  if (index == 0) return playLowestCardFromThisSuit(who, trumps);
  index2 = a.getRandomUint(index); /* index - which suit will be played */
  while ((suit < 4) && (cards[who][suit].empty())) suit++;
  for (i = suit; i < 4; i++) { 
    if ((index2 > 0) && ((cards[who][i].empty()) || (i == trumps))) { suit++; }
    else if (index2 > 0) { suit++; index2--; }
    else { while ((suit < 4) && ((cards[who][suit].empty())|| (suit == trumps))) suit++; break; }
  }
    
  iter = cards[who][suit].begin();
  ret = *(iter);
  cards[who][suit].erase(iter);
  return ret;
}

int Betterplayer::playLowestCardFromThisSuit(int who, int suit) {
  int ret;
  std::set<int>::iterator iter;

  iter = cards[who][suit].begin();
  ret = *(iter);
  cards[who][suit].erase(iter);
  return ret; 
}


int Betterplayer::playTryToOverruff(int who, int suit, int card) {  
  int ret;
  std::set<int>::iterator iter;
  
  iter = cards[who][suit].begin();
  while (iter != cards[who][suit].end())
    if ((*iter) < card) iter++;
    else break;
  if (iter != cards[who][suit].end()) { /* play the lowest higer trump */
    ret = *(iter);
    cards[who][contractSuit].erase(iter);
    return ret;
  }
  return playRandomSmallCard(who, suit);
}


int Betterplayer::playCard4(int who, int c1, int c2, int c3) {
  int suit = c1 / 13, index, ret;
  int suit2 = c2 / 13, suit3 = c3 / 13;
  std::set<int>::iterator iter;

  index = a.getRandomUint(10);
  
  if (index == 9) { /* in 10% we play random card */
    fprintf(stderr, "losuje karte\n");
    if (!cards[who][suit].empty()) { /* player has a card in this suit */
      index = a.getRandomUint(cards[who][suit].size());
      iter = cards[who][suit].begin();
      for (; index > 0; index--) iter++;
      ret = *(iter);
      cards[who][suit].erase(iter);
      return ret;
    }
    /* player hasn't got a card in this suit */
    return playCard1(who);
  }
  
  else { /* we try to take the trick */
    if (!cards[who][suit].empty()) { /* player has a card in this suit */
      if ((suit != contractSuit) && ((suit2 == contractSuit) || (suit3 == contractSuit))) /* couldn't take trick */
        return playLowestCardFromThisSuit(who, suit);
      if ((suit == suit2) && (c2 > c1))
        c1 = c2;
      if ((suit == suit3) && (c3 > c1))
        c1 = c3;
      iter = cards[who][suit].begin();
      if (c1 != c2) { /* opps take the trick */ 
        for (; (iter != cards[who][suit].end()) && (*iter < c1); iter++) {}
        if (iter == cards[who][suit].end())
          iter = cards[who][suit].begin();
      }
      ret = *(iter);
      cards[who][suit].erase(iter);
      return ret;
    }
    else { /* player doesn't have a card in this suit */
      /* if there is not a trump suit, we have to throw a small card */
      if (contractSuit == NT)
        return playRandomSmallCard(who, NT);
      /* if there is a trump suit and no trump has been played and a player has a trump, they ruff */
      if ((suit != contractSuit) && (suit2 != contractSuit) && (suit3 != contractSuit) && (!cards[who][contractSuit].empty()))
        return playLowestCardFromThisSuit(who, contractSuit);
      /* if there is a trump suit, opponent has ruffed and partner hasn't */
      if ((suit2 != contractSuit) && ((suit == contractSuit) || (suit3 == contractSuit))) {
        if (((suit == suit3) && (c1 < c3)) || ((suit != contractSuit) && (suit3 == contractSuit)))
          c1 = c3; /* c1 - higher trump */
        return playTryToOverruff(who, contractSuit, c1);  
      }
      /* if there is a trump suit, partner has ruffed and opponents hasn't */
      if ((suit2 == contractSuit) && (suit != contractSuit) && (suit3 != contractSuit))
        return playRandomSmallCard(who, contractSuit);
      /* if there is a trump suit, partner has ruffed and so did opponents */
      if ((suit2 == contractSuit) && ((suit == contractSuit) || (suit3 == contractSuit))) {
        if (((suit == suit3) && (c1 < c3)) || ((suit != contractSuit) && (suit3 == contractSuit)))
          c1 = c3; /* c1 - higher trump */
        if (c2 > c1) /* partner takes this trick */
          return playRandomSmallCard(who, contractSuit);
        /* can you overruff? */
        return playTryToOverruff(who, contractSuit, c1);        
      }
      return playRandomSmallCard(who, contractSuit);
    }
  }
    
  return 0;
}

int Betterplayer::play_randomly(int tricksWon, int lastWinner) {
  
  if (cardsInTrick > 0) {
    if (cardsInTrick == 1) {
      currentTrick.playCard(playCard2((lastWinner + 1) % 4, currentTrick.firstCard()), 1);
    }
    if (cardsInTrick <= 2) {
      currentTrick.playCard(playCard3((lastWinner + 2) % 4, currentTrick.firstCard(), currentTrick.getCard(1)), 2);
    }
    if (cardsInTrick <= 3) {
      currentTrick.playCard(playCard4((lastWinner + 3) % 4, currentTrick.firstCard(), currentTrick.getCard(1), currentTrick.getCard(2)), 3);
    }
    lastWinner = (lastWinner + currentTrick.whoWon(contractSuit)) % 4;
    tricksWon += (lastWinner % 2);
  }
  while (!(cards[N][C].empty() && cards[N][D].empty() && cards[N][H].empty() && cards[N][Sp].empty())) {
    currentTrick.playCard(playCard1(lastWinner), 0);
    currentTrick.playCard(playCard2((lastWinner + 1) % 4, currentTrick.firstCard()), 1);
    currentTrick.playCard(playCard3((lastWinner + 2) % 4, currentTrick.firstCard(), currentTrick.getCard(1)), 2);
    currentTrick.playCard(playCard4((lastWinner + 3) % 4, currentTrick.firstCard(), currentTrick.getCard(1), currentTrick.getCard(2)), 3);
    currentTrick.printTrickSymbols();
    lastWinner = (lastWinner + currentTrick.whoWon(contractSuit)) % 4;
    tricksWon += (lastWinner % 2);
  }
  
  return tricksWon;
}
