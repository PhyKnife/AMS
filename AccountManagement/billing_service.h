#pragma once

#include"model.h"

int addBilling(const char* pName, Billing *pBilling);
void initBillingList();
int getBilling();
Billing* queryBilling(const char* pName, int* pIndex);
void releaseBillingList();