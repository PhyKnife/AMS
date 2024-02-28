#pragma once
#include"model.h"

int saveBilling(const Billing* pBilling, const char* pPath);
int readBilling(Billing* pBilling, const char* pPath);
int getBillingCount(const char* pPath);
int updateBilling(const Billing* pBilling, const char* pPath, int nIndex);