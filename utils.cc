#include "utils.h"

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

