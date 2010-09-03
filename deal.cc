#include "deal.h"


int Trick::whoWon(int trump) {
  int who = 0;
  
  for (int i = 1; i < 4; i++)
    if (((cards[i] > cards[who]) && (cards[i] / 13 == cards[who] / 13)) ||
        ((cards[i] / 13 == trump) && (cards[who] / 13 != trump)))
      who = i;
  
  return who;
}

void Trick::playCard(int card, int player) {
  cards[player] = card;
}

int Trick::firstCard() {
  return cards[0];
}

int Trick::getCard(int i) {
  return cards[i];
}

void Trick::printTrick() {
  fprintf(stderr, "%d %d %d %d\n", cards[0], cards[1], cards[2], cards[3]);
}

void Trick::printTrickSymbols() {
  char c1[2], c2[2], c3[2], c4[2];
  
  fprintf(stderr, "%s ", changeNumberToCard(cards[0], c1));
  fprintf(stderr, "%s ", changeNumberToCard(cards[1], c2));
  fprintf(stderr, "%s ", changeNumberToCard(cards[2], c3));
  fprintf(stderr, "%s\n", changeNumberToCard(cards[3], c4));
}

int Deal::getLastCard() {
  return currentTrick.getCard((cardsInTrick + 3) % 4);
}


int Deal::getContractLevel() {
  return contractLevel;
}

int Deal::getContractSuit() {
  return contractSuit;
}

Trick Deal::getCurrentTrick() {
  return currentTrick;
}

std::vector < std::vector < std::set < int > > > Deal::getCards() {
  return cards;
}

std::vector < std::set < int > > Deal::getDeclarerCards() {
  return cards[S];
}

std::vector < std::set < int > > Deal::getDummyCards() {
  return cards[N];
}

std::vector < std::set < int > > Deal::getEastCards() {
  return cards[E];
}

std::vector < std::set < int > > Deal::getWestCards() {
  return cards[W];
}

int Deal::getWonTricks() {
  return wonTricks;
}

int Deal::whoNow() {
  return playerNow;
}

int Deal::getCardsInTrick() {
  return cardsInTrick;
}

int Deal::suitOfTrick() {
  return currentTrick.firstCard() / 13;
}


void Deal::incWonTricks() {
  wonTricks++;
}

void Deal::printDeal() {
  char card[2];
  
  for (int i = 0; i < 4; i++) {
    fprintf(stderr, "cards - player no %d\n", i);
    fprintf(stderr, "S   ");
    for(std::set<int>::iterator it = cards[i][Sp].begin(); it != cards[i][Sp].end(); it++) 
      fprintf(stderr, "%s ", changeNumberToCard(*it, card));
    fprintf(stderr, "\n");
    fprintf(stderr, "H   ");
    for(std::set<int>::iterator it = cards[i][H].begin(); it != cards[i][H].end(); it++)
      fprintf(stderr, "%s ", changeNumberToCard(*it, card));
    fprintf(stderr, "\n");
    fprintf(stderr, "D   ");
    for(std::set<int>::iterator it = cards[i][D].begin(); it != cards[i][D].end(); it++)
      fprintf(stderr, "%s ", changeNumberToCard(*it, card));
    fprintf(stderr, "\n");
    fprintf(stderr, "C   ");
    for(std::set<int>::iterator it = cards[i][C].begin(); it != cards[i][C].end(); it++)
      fprintf(stderr, "%s ", changeNumberToCard(*it, card));
    fprintf(stderr, "\n");
  }
}
