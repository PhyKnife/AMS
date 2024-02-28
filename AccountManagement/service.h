#pragma once
#include"model.h"

int addCardInfo(Card card); 
Card* queryCardInfo(const char* pName);
Card* queryCardsInfo(const char* pName,int* pIndex);
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo);
void releaseList();
int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo);
double getAmount(time_t tStart);
int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo);
int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo);
int annulCard(Card* pCard);