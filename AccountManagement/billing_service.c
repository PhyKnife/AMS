#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#include"model.h"
#include"billing_file.h"
#include"global.h"

lpBillingNode billingList = NULL;//计费信息链表

//[函数名] addBilling
//[功能] 上机时添加消费信息到文件
//[参数] pBilling:指向消费信息结构体;pName:上机卡号
//[返回值] TRUE保存成功 FALSE保存失败
int addBilling(const char* pName, Billing *pBilling)
{
	//消费信息写入结构体
	strcpy(pBilling->aCardName, pName);
	pBilling->tStart = time(NULL);
	pBilling->tEnd = 0;
	pBilling->fAmount = 0;
	pBilling->nStatus = 0;
	pBilling->nDel = 0;

	//消费信息结构体写入文件、
	if (FALSE==saveBilling(pBilling,BILLINGPATH))
	{
		return FALSE;
	}
	return TRUE;
}

//[函数名] initBillingList
//[功能] 初始化计费信息链表
//[参数] void
//[返回值] void
void initBillingList()
{
	lpBillingNode head = NULL;
	if (billingList==NULL)
	{
		head = (lpBillingNode)malloc(sizeof(BillingNode));
		head->next = NULL;
		billingList = head;
	}
}
//[函数名] releaseBillingList
//[功能] 释放计费信息链表
//[参数] void
//[返回值] void
void releaseBillingList()
{
	lpBillingNode cur = billingList;
	lpBillingNode next = NULL;
	//销毁链表
	while (cur != NULL)
	{
		next = cur->next;//结点内存释放钱，next保存其后继
		free(cur);//释放结点内存
		cur = next;
	}
	billingList = NULL;
}
//[函数名] getBilling
//[功能] 从计费信息文件中，获取计费信息保存到链表中
//[参数] void
//[返回值] TRUE：获取信息成功，FALSE：获取信息失败
int getBilling()
{
	int nCount = 0;
	Billing* pBilling = NULL;//计费信息
	lpBillingNode node = NULL;//
	int i = 0;//实际卡信息数
	lpBillingNode cur = NULL;//

	//清除链表中已存在的数据
	if (billingList != NULL)
	{
		releaseBillingList();
	}
	//初始化链表
	initBillingList();

	//获取计费信息个数
	nCount = getBillingCount(BILLINGPATH);
	//动态分配内存
	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);
	if (pBilling != NULL)
	{
		if (FALSE == readBilling(pBilling, BILLINGPATH))
		{
			free(pBilling);
			return FALSE;
		} 
	
		//将计费信息保存到链表中
	
		for (i = 0, node = billingList; i < nCount; i++)
		{
			cur = (lpBillingNode)malloc(sizeof(BillingNode));

			if (cur == NULL)
			{
				free(pBilling);

				return FALSE;
			}
			memset(cur, 0, sizeof(BillingNode));
			//将卡信息保存到结点中
			cur->data = pBilling[i];
			cur->next = NULL;
			node->next = cur;
			//尾结点指针后移
			node = cur;
		}
		//释放内存
		free(pBilling);
		return TRUE;
	}
	return FALSE;
}
//[函数名] queryBilling
//[功能] 在计费信息链表中，查询对应卡的计费信息，并获取其在链表中的索引号
//[参数] pName:要查询的计费信息的卡号；pIndex：查询到的计费信息在链表当中的索引号
//[返回值] 查询到的计费信息指针
Billing* queryBilling(const char* pName, int* pIndex)
{
	lpBillingNode node = NULL;
	int nIndex = 0;

	
	if (FALSE == getBilling())
	{
		return NULL;
	}
	//指向链表的第一个结点
	node = billingList->next;
	//遍历链表
	while (node != NULL)
	{
		//查询到卡号相同，并且未结算的计费信息
		if (strcmp(node->data.aCardName, pName) == 0 && node->data.nStatus == 0)
		{
			//返回结点数据的地址
		
			return &node->data;
		}
		node = node->next;
		nIndex++;
		*pIndex = nIndex;
	}

	return NULL;
}
