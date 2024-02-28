#ifndef MODEL_H
#define MODEL_H

#include<time.h>

typedef struct Card
{
	char aName[18];
	char aPwd[8];
	int nStatus;
	time_t tStart;
	time_t tEnd;
	float fTotalUse;
	time_t tLastTime;
	int nUseCount;
	float fBalance;
	int nDel;
}Card;

typedef struct CardNode
{
	Card data;
	struct CardNode* next;//指向下一结点的指针
}CardNode, *lpCardNode;

//消费信息结构体
typedef struct Billing
{
	char aCardName[18];
	time_t tStart;
	time_t tEnd;
	float fAmount;
	int nStatus;
	int nDel;
}Billing;

//上机信息结构体
typedef struct LogonInfo
{
	char aCardName[18];
	time_t  tLogon;
	float fBalance;
}LogonInfo;


//计费信息结点
typedef struct BillingNode
{
	Billing data;
	struct BillingNode *next;
}BillingNode, *lpBillingNode;

//下机信息结构体
typedef struct SettleInfo
{
	char aCardName[18]; //卡号
	time_t tStart;   //上机时间
	time_t tEnd; //下机时间
	float fAmount; //消费金额
	float fBalance; //余额
}SettleInfo;

//充值退费结构体
typedef struct Money
{
	char aCardName[18];//卡号
	time_t tTime; //充值消费的时间
	int nStatus; //状态：0-充值；1-退费
	float fMoney; //充值退费金额
	int nDel;//删除标识，0-未删除，1-已删除
}Money;

//充值退费信息结构体
typedef struct MoneyInfo
{
	char aCardName[18]; //卡号
	float fMoney; //充值退费金额
	float fBalance; //余额
}MoneyInfo;

#endif

