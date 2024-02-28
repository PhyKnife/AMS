#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"menu.h"
#include"model.h"
#include"card_service.h"
#include"global.h"
#include"tool.h"
#include"card_file.h"
#include"billing_file.h"
#include"billing_service.h"
#include"service.h"

void outputMenu()
{
	printf("----------菜单----------\n");
	printf("1.添加卡               *\n");
	printf("2.查询卡               *\n");
	printf("3.上机                 *\n");
	printf("4.下机                 *\n");
	printf("5.充值                 *\n");
	printf("6.退费                 *\n");
	printf("7.注销卡               *\n");
	printf("0.退出                 *\n");
	printf("请选择菜单栏编号(0-8): *\n");
}
int getSize(const char*pString)
{
	int nSize = 0;
	while (*(pString + nSize) != '\0')
	{
		nSize++;
	}

	return nSize;
}
void add()
{
	Card card;
	char name[30];
	char pwd[20];
	struct tm* endTime;
	struct tm* startTime;

	printf("请输入卡号(长度为1~18):");
	scanf("%s", name);
	if (getSize(name) >= 18)
	{
		printf("输入的卡号长度超出最大值\n");
		return;
	}
	if (queryCardInfo(name) != NULL)
	{
		printf("输入卡号已存在，请重新输入\n");
		return;
	}
	strcpy(card.aName, name);
	printf("请输入密码：");
	scanf("%s", pwd);
	if (getSize(pwd) >= 8)
	{
		printf("输入的密码长度超出最大值\n");
		return;
	}
	strcpy(card.aPwd, pwd);
	printf("请输入开卡金额（RMB）：");
	scanf("%f", &card.fBalance);
	card.fTotalUse = card.fBalance;
	card.nDel = 0;
	card.nStatus = 0;
	card.nUseCount = 0;
	card.tStart = card.tEnd = card.tLastTime = time(NULL);
	startTime = localtime(&card.tStart);
	endTime = localtime(&card.tEnd);
	endTime->tm_year = startTime->tm_year + 1;
	card.tEnd = mktime(endTime);

	printf("----------添加的卡信息如下----------\n");
	printf("卡号\n密码\t状态\t开卡金额\n");
	printf("%s\t%s\t%d\t%0.1f\n\n", card.aName, card.aPwd, card.nStatus, card.fBalance);
	if (FALSE == addCardInfo(card))
	{
		printf("---------添加卡信息到文件失败---------\n");
	}
	else
	{
		printf("----------添加卡信息到文件成功----------\n");
	}
}
void query()
{
	char name[18] = { 0 };
	Card* pCard = NULL;
	char aLastTime[30];

	int icha = 0;
	int nIndex = 0;
	int i;

	printf("请输入要查询的卡号(长度为1~18):");
	scanf("%s", name);
	printf("1.精确查询，2.模糊查询（输入1或2）：");
	scanf("%d", &icha);

	if (icha==1)
	{
		pCard = queryCardInfo(name);
	}
	else
	{
		pCard = queryCardsInfo(name,&nIndex);
	}

	if (pCard == NULL)
	{
		printf("----------没有查询到该卡的信息----------\n");
	}
	else
	{
		printf("----------查询到该卡信息如下----------\n");
		printf("卡号\t状态\t余额\t累计金额\t使用次数\t上次使用时间\n");

		if (icha == 1)//精确查询
		{
			timeToString(pCard->tLastTime, aLastTime);
			printf("%s\t%d\t%0.1f\t%0.1f\t\t%d\t%s\n\n", pCard->aName, pCard->nStatus, pCard->fBalance, pCard->fTotalUse, pCard->nUseCount, aLastTime);
		}
		else
		{
			for ( i = 0; i < nIndex; i++)
			{
				timeToString(pCard[i].tLastTime, aLastTime);
				printf("%s\t%d\t%0.1f\t%0.1f\t\t%d\t%s\n\n", pCard[i].aName, pCard[i].nStatus, pCard[i].fBalance, pCard[i].fTotalUse, pCard[i].nUseCount, aLastTime);
			
			}
			free(pCard);
		}
		printf("--------------------\n");
		pCard = NULL;
	}
}

//[函数名] logon
//[功能] 输入上机卡信息，查询到，输出上机卡信息
//[参数] void
//[返回值] void

void logon()
{
	char aName[18] = { 0 };//上机卡号
	char aPwd[8] = { 0 };//上机卡密码
	char aLastTime[30];//存放指定格式字符串的时间
	LogonInfo *pInfo = NULL;
	int nResult = 0;

	//提示并接受上机卡号
	printf("请输入上机卡号(长度为1~18):");
	scanf("%s",aName);
	//判断输入的卡号是否符合要求
	if (getSize(aName)>=18)
	{
		printf("输入的卡号长度超过最大值\n");
		return;
	}
	//提示并接收上机密码
	printf("请输入上机密码(长度为1~8):");
	scanf("%s", aPwd);
	//判断输入的密码是否符合要求
	if (getSize(aPwd)>= 8)
	{
		printf("输入的密码长度超过最大值\n");
		return;
	}
	//开始上机 获取上机结果
	pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));
	nResult = doLogon(aName, aPwd, pInfo);
	switch (nResult)
	{
	case 0:
		printf("----------上机失败----------\n");
		break;
	case 1:
		printf("----------上机的卡信息如下----------\n");
		printf("卡号\t余额\t上机时间\n");
		timeToString(pInfo->tLogon, aLastTime);
		printf("%s\t%0.1f\t%s\n\n",pInfo->aCardName,pInfo->fBalance,aLastTime);
		printf("----------上机成功----------\n");
		break;
	case 2:
		printf("----------该卡不能使用----------\n");
		break;
	case 3:
		printf("----------余额不足----------\n");
		break;
	}
	//释放上机信息
	free(pInfo);
}
//函数名：exitApp
//功能:退出应用程序
//参数：void
//返回值：void

void exitApp()
{
	releaseList();
}

//函数名 settle
//功能 输入下机卡信息，查询到，输出上机卡信息
//参数 void
//返回值 void
void settle() 
{
	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	int nResult = -1;
	SettleInfo* pInfo = NULL;
	char aStartTime[30] = { 0 };
	char aEndTime[30] = { 0 };

	pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

	printf("请输入下机卡号(长度为1~18):");
	scanf("%s", aName);

	printf("请输入下机密码(长度为1~8):");
	scanf("%s", aPwd);

	//进行下机
	nResult = doSettle(aName, aPwd, pInfo);

	printf("\n----------下机信息如下----------\n");
	switch (nResult)
	{
	case 0:
		printf("----------下机失败----------\n");
		break;
	case 1:
		printf("卡号\t消费\t余额\t上机时间\t\t下机时间\n");
		timeToString(pInfo->tStart, aStartTime);
		timeToString(pInfo->tEnd, aEndTime);
		printf("%s\t%0.1f\t%0.1f\t%s\t%s\n\n", 
			pInfo->aCardName,pInfo->fAmount,pInfo->fBalance, aStartTime,aEndTime);
		printf("----------下机成功----------\n");
		break;
	case 2:
		printf("----------该卡未在使用----------\n");
		break;
	case 3:
		printf("----------余额不足----------\n");
		break;
	default:
			break;
	}

	free(pInfo);
}

//[函数名] addMoney
//[功能] 充值
//[参数] void
//[返回值] void
void addMoney()
{
	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	float fAmount = 0;
	MoneyInfo sMoneyInfo;

	printf("请输入充值卡号(长度为1~18):");
	scanf("%s",aName);
	printf("请输入充值密码(长度为1~8):");
	scanf("%s",aPwd);
	printf("请输入充值金额(RMB):");
	scanf("%f",&fAmount);

	//保存充值金额
	sMoneyInfo.fMoney = fAmount;

	//判断充值是否成功
	if (TRUE==doAddMoney(aName,aPwd,&sMoneyInfo))
	{
		//提示充值信息
		printf("----------充值信息如下----------\n");
		//输出表头
		printf("卡号\t充值金额\t余额\n:");
		printf("%s\t%0.1f\t\t%0.1f\n",sMoneyInfo.aCardName,sMoneyInfo.fMoney,sMoneyInfo.fBalance);
	}
	else
	{
		printf("----------充值失败-----------\n");
	}

}

//[函数名] refundMoney
//[功能] 退费
//[参数] void
//[返回值] void

void refundMoney()
{
	char aName[18] = { 0 };//卡号
	char aPwd[8] = { 0 }; //密码
	int nResult = -1;
	MoneyInfo sMoneyInfo; // 退费信息

	printf("请输入退费卡号(长度为1~18):");

	scanf("%s",aName);

	printf("请输入退费密码(长度为1 ^8):");

	scanf("%s",aPwd);

			// 进行退费

	nResult = doRefundMoney(aName, aPwd, &sMoneyInfo);

			// 根据退费结果，提示不同信息

		switch (nResult) 
		{
			case 0://退费失败
				printf("---**--- 退费失败!---**---\n");
				break;
			case 1 : //退费成功
				printf("---**---退费信息如下- **-- - \n");
				//输出表头
				printf("卡号\t退费金额\t余额\n");
				//输出退费信息
				printf("%s\t%0.1f\t\t%0.1f\n",sMoneyInfo.aCardName, sMoneyInfo.fMoney, sMoneyInfo.fBalance);
				break;
			case 2:
				printf("----------该卡正在使用或已注销----------\n");
				break;
			case 3: //卡余额不足
				printf("**-*-- - 卡余额不足!----\n"); 
				break;
			default:
				break;
		}
}
//[函数名] annual
//[功能] 注销卡
//[参数] void
//[返回值] void
void annul()
{
	Card card;

	printf("请输入要注销的卡号:");
	scanf("%s",card.aName);

	printf("请输入密码:");
	scanf("%s", card.aPwd);
	//提示注销的信息
	if (FALSE==annulCard(&card))
	{
		printf("----------注销卡失败,该卡可能已被注销----------\n");
		return;
	}
	else
	{
		printf("----------注销信息如下:----------\n");
		printf("卡号\t退款金额\n");
		printf("%s\t%0.1f\n",card.aName,card.fBalance);
		return;
	}
}

