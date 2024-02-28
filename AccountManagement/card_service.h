#ifndef CARD_SERVICE_H
#define CARD_SERVICE_H

#include"model.h"

int addCard(Card crd);
Card* queryCards(const char* pName, int* pIndex);
Card* queryCard(const char* pName);
int getCard();
int initCardList();
void releaseCardList();
Card* checkCard(const char* pName, const char* pPwd, int* pIndex);
#endif
