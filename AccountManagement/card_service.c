#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"model.h"
#include"global.h"
#include"card_file.h"
#include"card_service.h"
#include"billing_service.h"

lpCardNode cardList = NULL;

int addCard(Card crd)
{
	if (TRUE == saveCard(&crd, CARDPATH))
	{
		printf("----------卡信息已成功保存到文件夹中---------\n");
		return TRUE;
	}
		printf("----------卡信息保存到文件夹中失败---------\n");
		return FALSE;
}
Card* queryCard(const char*pName)
{
	lpCardNode p;
	Card* pCard = NULL;

	//如果从卡信息文件中获取卡信息失败，返回NULL
	if (FALSE == getCard())
	{
		return NULL;
	}
	p = cardList->next;


	//遍历链表
	while (p != NULL)
	{
		if (strcmp(p->data.aName, pName) == 0)
		{
			pCard = &(p->data);
			return pCard;
		}
		else
		{
			p = p->next;
		}

	}
	//没有找到则返回NULL
	return pCard;
}

//函数名 queryCards
//功能 模糊查询卡信息
//参数 pName:指向用户输入的要查询的卡号 pIndex:指向查询到的卡信息数变量
//返回值 指向卡信息结构体的指针
Card* queryCards(const char*pName,int* pIndex)
{
	lpCardNode node = NULL;
	Card* pCard = NULL;//save the information of card 
	
	//如果从卡信息文件中获取卡信息失败，返回NULL
	if (FALSE==getCard())
	{
		return NULL;
	}
	pCard = (Card*)malloc(sizeof(Card));

	if (pCard==NULL)
	{
		return NULL;
	}
	node = cardList->next;
	//遍历链表
	while (node != NULL)
	{
		if (strstr(node->data.aName, pName)  != NULL)
		{
			pCard[*pIndex] = node->data;
			(*pIndex)++;

			pCard = (Card*)realloc(pCard, ((*pIndex) + 1) * sizeof(Card));
		}
		//移到链表的下一个结点
		node = node->next;

	}
	//没有找到则返回NULL
	return pCard;
}
//函数名:getCard
//功能:从卡信息文件中获取卡信息，保存到链表中
//参数:void
//返回值:TRUE FALSE
int getCard()
{
	int i = 0;
	Card* pCard = NULL;//读取到的一条卡信息
	int nCount = 0;//实际卡信息数
	lpCardNode node = NULL;//当前尾结点指针
	lpCardNode cur = NULL;//要添加到链表中的结点指针

	//清除链表中已存在的数据
	if (cardList != NULL)
	{
		releaseCardList();
	}
	//初始化链表
	initCardList();
	//获取文件中卡信息个数
	nCount = getCardCount(CARDPATH);
	if (nCount == 0)
	{
		return FALSE;
	}
	 
	pCard = (Card*)malloc(sizeof(Card)*nCount);
	if (pCard != NULL)
	{
		if(0==readCard(pCard,CARDPATH))
		{
			free(pCard);
			return FALSE;
		}
		for (i = 0, node = cardList; i < nCount; i++)
		{
			cur=(lpCardNode)malloc(sizeof(CardNode));

				if (cur==NULL)
				{
					free(pCard);

					return FALSE;
				}
			memset(cur, 0,  sizeof(CardNode));
			//将卡信息保存到结点中
			cur->data = pCard[i];
			cur->next = NULL;
			node->next = cur;
			//尾结点指针后移
			node = cur;
		}
		//释放内存
		free(pCard);

		return TRUE;
	}
	
	return FALSE;
}
//函数名：initCardList
//功能：初始化卡信息链表
//参数: void
//返回值：int型，TRUE表示成功，FALSE表示失败
int initCardList()
{
	lpCardNode head = NULL;
	if (cardList == NULL)
	{
		//为链表头结点分配内存
		head = (lpCardNode)malloc(sizeof(CardNode));
		//if it successed
		if (head != NULL)
		{
			head->next = NULL;
			cardList = head;
			return TRUE;
		}
	}
	return FALSE;
}
//函数名:releaseCardList
//功能: 释放卡信息链表
//参数:void
//返回值:void

void releaseCardList()
{
	lpCardNode cur = cardList;
	lpCardNode next = NULL;

	while (cur != NULL)
	{
		next = cur->next; //释放cur结点前，用next保存它的后继结点
		free(cur);//释放cur结点
		cur = next;
	}
	cardList = NULL;
}


//[函数名]checkCard
//[功能] 从文件上读取卡信息到链表，在链表中查询卡信息，并获取其在链表中的位置
//[参数] pName：上机卡号；pPwd:上机密码；pIndex:返回卡的索引号
//[返回值] 上机卡结构体
Card* checkCard(const char* pName, const char* pPwd, int* pIndex)
{
	lpCardNode cardNode = NULL;
	int nIndex = 0;

	//如果从文件中获取卡信息失败，则上机失败
	if (FALSE==getCard())
	{
		return FALSE;
	}
	//指向链表的第一个结点
	cardNode = cardList->next;
	//遍历链表
	while (cardNode!=NULL)
	{
		//查找上机卡，判断卡号和密码是否正确
		if ((strcmp(cardNode->data.aName,pName)==0)&&(strcmp(cardNode->data.aPwd,pPwd)==0))
		{
			//返回卡信息结点数据的地址
			*pIndex = nIndex;
			return &cardNode->data;
		}
		cardNode = cardNode->next;
		nIndex++;
	}

	return NULL;
}
