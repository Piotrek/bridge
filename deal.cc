#include "deal.h"


void Deal::decEastCards() {
    numEastCards--;
}

void Deal::decWestCards() {
    numWestCards--;
}

int Deal::getContractLevel() {
    return contractLevel;
}

int Deal::getContractSuit() {
    return contractSuit;
}

std::vector<int> Deal::getCurrentTrick() {
    return currentTrick;
}

std::set<int> Deal::getDeclarerCards() {
    return declarerCards;
}

std::set<int> Deal::getDummyCards() {
    return dummyCards;
}

std::set<int> Deal::getEastCards() {
    return eastCards;
}

int Deal::getNumEastCards() {
    return numEastCards;
}

int Deal::getNumWestCards() {
    return numWestCards;
}

std::set<int> Deal::getWestCards() {
    return westCards;
}

int Deal::getWonTricks() {
  return wonTricks;
}
