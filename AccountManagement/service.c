#define _CRT_SECURE_NO_WARNINGS
#include<string.h>
#include<stdio.h>

#include"model.h"
#include"global.h"
#include"card_file.h"
#include"card_service.h"
#include"billing_file.h"
#include"billing_service.h"
#include "service.h"
#include "money_file.h"


//[函数名] addCardInfo
//[功能] 添加卡信息
//[参数] 卡信息结构体
//[返回值] TURE保存成功；FALSE保存失败

int addCardInfo(Card card)
{
	if (TRUE==saveCard(&card,CARDPATH))
	{
		return TRUE;
	}

	return FALSE;
}

//[函数名] queryCardInfo
//[功能] 精确查询卡信息
//[参数] pName:指向用户要查询的卡号
//[返回值] 指向卡信息结构体指针
Card* queryCardInfo(const char* pName)
{
	Card* pCard = NULL;
	pCard = queryCard(pName);
	return pCard;
}

//[函数名] queryCardsInfo
//[功能] 模糊查询卡信息
//[参数] pName:指向用户要查询的卡号,pIndex:指向查到的卡信息数变量
//[返回值] 指向卡信息结构体指针
Card* queryCardsInfo(const char* pName,int* pIndex)
{
	Card* pCard = NULL;
	pCard = queryCards(pName,pIndex);
	return pCard;
}
//[函数名] doLogon
//[功能] 上机
//[参数] pName:上机卡号;pPwd:上机密码;pInfo:指向上机信息结构体
//[返回值] TRUE:上机成功，FALSE:上机失败

int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo)
{
	Card* pCard = NULL;//
	int nIndex = 0;//卡信息在链表中的索引，用于更新卡信息
	Billing billing;//计费信息

	//根据卡号和密码，从链表中获取卡信息和卡信息在链表中的索引
	pCard = checkCard(pName, pPwd, &nIndex);

	//如果卡信息为空，表示没有该卡信息，上机失败
	if (pCard == NULL)
	{
		return FALSE;
	}
	//如果卡状态不为0，表示该卡不能上机
	if (pCard->nStatus != 0)
	{
		return UNUSE;
	}
	//如果卡的余额为0，不能上机
	if (pCard->fBalance <= 0)
	{
		return NOENOUGHMONEY;
	}
	//如果可以上机更新卡信息
	pCard->nStatus = 1;//状态为正在使用
	pCard->nUseCount++;//使用次数+1
	pCard->tLastTime = time(NULL);//更新最后使用时间为当前时间
	//更新文件中的卡信息
	if (FALSE == updateCard(pCard, CARDPATH, nIndex))
	{
		return FALSE;
	}
	//添加消费记录
	strcpy(billing.aCardName, pName);//上机卡号
	billing.tStart = time(NULL);//上机时间
	billing.tEnd = 0;//下机时间
	billing.nStatus = 0;//消费状态
	billing.fAmount = 0;//消费金额
	billing.nDel = 0;//删除标识

	//将计费信息保存到文件中
	if (TRUE == saveBilling(&billing, BILLINGPATH))
	{
		//组装上机信息
		strcpy(pInfo->aCardName, pName);
		pInfo->fBalance = pCard->fBalance;
		pInfo->tLogon = billing.tStart;

		return TRUE;
	}

	return FALSE;
}
//[函数名] releaseList
//[功能] 退出应用程序时，释放链表内存
//[参数] void
//[返回值] void
void releaseList()
{
	releaseCardList(); //释放卡信息链表内存
	releaseBillingList();

}

//[函数名] doSettle
//[功能] 下机
//[参数] pName:下机卡号；pPwd:下机密码；pInfo:指向下机信息结构体
//[返回值] 下机信息值，不同情况输出不同信息
int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo)
{
	Card* pCard = NULL;
	Billing* pBilling = NULL;
	int nIndex = 0;
	int nPosition = 0;
	double dbAmount = 0.0;
	float fBalance = 0.0;

	//查询上机卡
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard==NULL)
	{
		printf("----------无该卡信息，请重新输入----------\n");
		return FALSE;

	}
	if (pCard->nStatus!=1)
	{
		printf("该卡未上机\n");
		return UNUSE;
	}
	pBilling = queryBilling(pName, &nPosition);
	//如果为空，表示没有该卡信息，返回FALSE
	if (pBilling==NULL)
	{
		printf("----------无该卡信息，请重新输入----------\n");
		return FALSE;
	}
	dbAmount = getAmount(pBilling->tStart);

	fBalance = pCard->fBalance - (float)dbAmount;
	if (fBalance<0)
	{
		return NOENOUGHMONEY;
	}
	//更新卡信息
	pCard->fBalance = fBalance;
	pCard->nStatus = 0;
	pCard->tLastTime = time(NULL);

	if (FALSE==updateCard(pCard,CARDPATH,nIndex))
	{
		return FALSE;
	}
	pBilling->fAmount = (float)dbAmount;
	pBilling->nStatus = 1;
	pBilling->tEnd = time(NULL);
	if (FALSE == updateBilling(pBilling, BILLINGPATH, nPosition))
	{
		return FALSE;
	}

	//组装下机信息
	strcpy(pInfo->aCardName, pName);
	pInfo->fAmount = (float)dbAmount;
	pInfo->fBalance = fBalance;
	pInfo->tStart = pBilling->tStart;
	pInfo->tEnd = time(NULL);

	return TRUE;
}
//[函数名] getAmount
//[功能] 根据上机时间，计算消费金额
//[参数] 上机时间
//[返回值] 消费金额
double getAmount(time_t tStart)
{
	double dbAmount = 0;
	int nCount = 0;
	int nSec = 0;
	int nMinutes = 0;
	time_t tEnd = time(NULL);

	nSec = (int)(tEnd - tStart);
	nMinutes = nSec / 60;
	if (nMinutes % UNIT==0)
	{
		nCount = nMinutes / UNIT;
	}
	else
	{
		nCount = nMinutes / UNIT + 1;
	}
	dbAmount = nCount * CHARGE;

	return dbAmount;
}
//[函数名] doAddMoney
//[功能] 进行充值操作
//[参数] pName:充值卡的卡号；pPwd:充值卡密码；pMoneyInfo:充值信息
//[返回值] TURE：充值成功；FALSE：充值失败

int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo)
{
	Card* pCard = NULL;
	int nIndex = 0;
	Money sMoney;

	//查询充值卡
	pCard = checkCard(pName, pPwd, &nIndex);
	

	if (pCard==NULL)
	{
		printf("----------无该卡信息，不能充值----------\n");
		return FALSE;
	}
	if (pCard->nStatus!=0&&pCard->nStatus!=1)
	{
		return FALSE;
	}

	//如果可以充值，更新卡信息
	pCard->fBalance += pMoneyInfo->fMoney;
	pCard->fTotalUse += pMoneyInfo->fMoney;
	//更新文件中的卡信息
	if (FALSE==updateCard(pCard,CARDPATH,nIndex))
	{
		return FALSE;
	}
	//组装充值信息
	strcpy(sMoney.aCardName, pCard->aName);
	sMoney.tTime = time(NULL);
	sMoney.nStatus = 0;
	sMoney.fMoney = pMoneyInfo->fMoney;
	sMoney.nDel = 0;
	//将充值记录保存到文件中
	if (TRUE==saveMoney(&sMoney,MONEYPATH))
	{
		strcpy(pMoneyInfo->aCardName,sMoney.aCardName);
		pMoneyInfo->fBalance = pCard->fBalance;

		return TRUE;
	}


	return FALSE;
}

//[函数名] doRefundMoney
//[功能] 进行退费操作
//[参数]pName 退费卡号; pPwd 退费卡密码; pMoneyInfo 充值退费信息结构体
//[返回值] TRUE 退费成功; FALSE退费失败
int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo)
{
	Card* pCard = NULL;
	int nIndex = 0; // 卡信息在链表中的索引号
	float fBalance = 0.0;// 卡的余额
	Money sMoney;
	//查询退费卡
	pCard = checkCard(pName, pPwd, &nIndex);
	// 如果为空，表示没有该卡信息，返回FALSE

	if (pCard == NULL) {


		printf("---***---无该卡信息，不能退费!---***---\n");

		return FALSE;
	}
	//判断该卡是否未使用，只有未使用的卡才能进行退费操作
	if (pCard->nStatus != 0)
	{
		return	UNUSE;
	}

	//如果余额等于0，则不能退费
	fBalance = pCard->fBalance;
	if (fBalance <= 0)
	{
		return NOENOUGHMONEY;
	}
	//更新卡信息
	pCard->fBalance = 0;//余额
	pCard->fTotalUse -= fBalance;  //累计金额
//更新文件中的卡信息
	if (FALSE == updateCard(pCard, CARDPATH, nIndex))
	{

		return FALSE;
	}

	//组合退费信息
	strcpy(sMoney.aCardName, pCard->aName);
	sMoney.tTime = time(NULL);
	sMoney.nStatus = 1;
	sMoney.fMoney = fBalance;
	sMoney.nDel = 0;

	//更新文件中的充值退费信息
	if (TRUE==saveMoney(&sMoney,MONEYPATH))
	{
		strcpy(pMoneyInfo->aCardName, sMoney.aCardName);
		pMoneyInfo->fMoney = sMoney.fMoney;
		pMoneyInfo->fBalance = pCard->fBalance;

		return TRUE;
	}

	return TRUE;
}
//[函数名] annulCard
//[功能] 注销卡
//[参数] pCard 卡信息结构体
//[返回值] TRUE 注销成功；FALSE 注销失败
int annulCard(Card * pCard)
{
	Card* pCurCard = NULL;
	int nIndex = -1;

	if (pCard==NULL)
	{
		return FALSE;
	}

	pCurCard = checkCard(pCard->aName, pCard->aPwd, &nIndex);

	if (pCurCard==NULL)
	{
		return FALSE;
	}
	//只有未上机的卡才能注销
	if (pCurCard->nStatus!=0)
	{
		return FALSE;
	}
	pCard->fBalance = pCurCard->fBalance;

	pCurCard->nStatus = 2;
	pCurCard->nDel = 1;
	pCurCard->fBalance = 0;
	pCurCard->tLastTime = time(NULL);

	if (FALSE==updateCard(pCurCard,CARDPATH,nIndex))
	{
		return FALSE;
	}
	return TRUE;
}
