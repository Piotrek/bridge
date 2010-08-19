#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>

#include "uct.h"

using namespace std;

int changeCardToNumber (char *c) {
  int num = 0;

  if ((c[0] == 'D') || (c[0] == 'd')) num += 13;
  if ((c[0] == 'H') || (c[0] == 'h')) num += 26;
  if ((c[0] == 'S') || (c[0] == 's')) num += 39;

  if ((c[1] > '1') && (c[1] <= '9')) num = num + c[1] - '2';
  if (c[1] == 'T') num += 8;
  if (c[1] == 'J') num += 9;
  if (c[1] == 'Q') num += 10;
  if (c[1] == 'K') num += 11;
  if (c[1] == 'A') num += 12;

  return num;
}

char* changeNumberToCard (int num, char* c) {
  int suit, value;

  suit = num / 13;
  value = num % 13;

  if (suit == 0) c[0] = 'C';
  if (suit == 1) c[0] = 'D';
  if (suit == 2) c[0] = 'H';
  if (suit == 3) c[0] = 'S';

  if (value < 8) c[1] = '2' + value;
  if (value == 8) c[1] = 'T'; 
  if (value == 9) c[1] = 'J'; 
  if (value == 10) c[1] = 'Q'; 
  if (value == 11) c[1] = 'K'; 
  if (value == 12) c[1] = 'A';
  
  c[2]=0;
  
  return c; 
}


int main()
{	
  set < int > dummyCards, declarerCards, defendersCards;
  vector < set < int > > cards;
  int count;
  
  cards.resize(3);
  while (true) {
    char cmd[16];
    scanf("%s", cmd);
    if (!strcmp(cmd, "set_dummy_cards")) {
      dummyCards.clear();
      for (int i = 0; i < count; i++) {
        char buf[4];
        scanf("%s", buf);
        dummyCards.insert(changeCardToNumber(buf));
      }
      cards[0] = dummyCards;
      printf("=\n\n");
    } 
    else if (!strcmp(cmd, "set_declarer_cards")) {
      declarerCards.clear();
      for (int i = 0; i < count; i++) {
        char buf[4];
        scanf("%s", buf);
        declarerCards.insert(changeCardToNumber(buf));
      }
      cards[2] = declarerCards;
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "set_defenders_cards")) {
      defendersCards.clear();
      for (int i = 0; i < 2*count; i++) {
        char buf[4];
        scanf("%s", buf);
        defendersCards.insert(changeCardToNumber(buf));
      }
      cards[1] = defendersCards;
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "auto_set_defenders_cards")) {
      vector < int > possibleCards;
      set < int >::iterator it;
      
      possibleCards.resize(52);
      for (it = dummyCards.begin(); it != dummyCards.end(); it++)
        possibleCards[*it] = 1;  
      for (it = declarerCards.begin(); it != declarerCards.end(); it++)
        possibleCards[*it] = 1;
        
      defendersCards.clear();
      for (int i = 0; i < 52; i++) {
        if (possibleCards[i] != 1)
          defendersCards.insert(i);
      }
      cards[1] = defendersCards;
      printf("=\n\n");
    } // TODO
    else if (!strcmp(cmd, "gen_move")) {
      char c[2];
      int card;
      UctTree tree = UctTree(0, dummyCards, declarerCards); //TODO
      card = tree.genMove().getCard();
      printf("%s ", changeNumberToCard(card,c));
      printf("\n");
    } // TODO
    else if (!strcmp(cmd, "play")) {
      char player[2], card[3];
      scanf("%s %s", player, card);
      int c = changeCardToNumber(card);
      if (player[0] == 'N')
        dummyCards.erase(c);
      else {
        if (player[0] == 'S')
          declarerCards.erase(c);
        else 
          defendersCards.erase(c);
      }
    }
    else if (!strcmp(cmd, "set_board_size")) {
      scanf("%d", &count);
      printf("\n");
    } 
    else if (!strcmp(cmd, "print_dummy_cards")) {
      char c[3];
      set < int >::iterator it;
      for (it = dummyCards.begin(); it != dummyCards.end(); it++)
        printf("%s ", changeNumberToCard(*it, c));
    }
    else if (!strcmp(cmd, "print_declarer_cards")) {
      char c[3];
      set < int >::iterator it;
      for (it = declarerCards.begin(); it != declarerCards.end(); it++)
        printf("%s ", changeNumberToCard(*it, c));
    }
    else if (!strcmp(cmd, "print_defenders_cards")) {
      char c[3];
      set < int >::iterator it;
      for (it = defendersCards.begin(); it != defendersCards.end(); it++)
        printf("%s ", changeNumberToCard(*it, c));
    }
    else if (!strcmp(cmd, "quit")) {
      break;
    }
    else { // wrong command
      scanf("%*[^\n]");
      printf("? wrong command `%s'\n\n", cmd);
    }
    fflush(stdout);  
  }
  return 0;
}
