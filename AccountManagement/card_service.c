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
		printf("----------����Ϣ�ѳɹ����浽�ļ�����---------\n");
		return TRUE;
	}
		printf("----------����Ϣ���浽�ļ�����ʧ��---------\n");
		return FALSE;
}
Card* queryCard(const char*pName)
{
	lpCardNode p;
	Card* pCard = NULL;

	//����ӿ���Ϣ�ļ��л�ȡ����Ϣʧ�ܣ�����NULL
	if (FALSE == getCard())
	{
		return NULL;
	}
	p = cardList->next;


	//��������
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
	//û���ҵ��򷵻�NULL
	return pCard;
}

//������ queryCards
//���� ģ����ѯ����Ϣ
//���� pName:ָ���û������Ҫ��ѯ�Ŀ��� pIndex:ָ���ѯ���Ŀ���Ϣ������
//����ֵ ָ����Ϣ�ṹ���ָ��
Card* queryCards(const char*pName,int* pIndex)
{
	lpCardNode node = NULL;
	Card* pCard = NULL;//save the information of card 
	
	//����ӿ���Ϣ�ļ��л�ȡ����Ϣʧ�ܣ�����NULL
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
	//��������
	while (node != NULL)
	{
		if (strstr(node->data.aName, pName)  != NULL)
		{
			pCard[*pIndex] = node->data;
			(*pIndex)++;

			pCard = (Card*)realloc(pCard, ((*pIndex) + 1) * sizeof(Card));
		}
		//�Ƶ��������һ�����
		node = node->next;

	}
	//û���ҵ��򷵻�NULL
	return pCard;
}
//������:getCard
//����:�ӿ���Ϣ�ļ��л�ȡ����Ϣ�����浽������
//����:void
//����ֵ:TRUE FALSE
int getCard()
{
	int i = 0;
	Card* pCard = NULL;//��ȡ����һ������Ϣ
	int nCount = 0;//ʵ�ʿ���Ϣ��
	lpCardNode node = NULL;//��ǰβ���ָ��
	lpCardNode cur = NULL;//Ҫ��ӵ������еĽ��ָ��

	//����������Ѵ��ڵ�����
	if (cardList != NULL)
	{
		releaseCardList();
	}
	//��ʼ������
	initCardList();
	//��ȡ�ļ��п���Ϣ����
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
			//������Ϣ���浽�����
			cur->data = pCard[i];
			cur->next = NULL;
			node->next = cur;
			//β���ָ�����
			node = cur;
		}
		//�ͷ��ڴ�
		free(pCard);

		return TRUE;
	}
	
	return FALSE;
}
//��������initCardList
//���ܣ���ʼ������Ϣ����
//����: void
//����ֵ��int�ͣ�TRUE��ʾ�ɹ���FALSE��ʾʧ��
int initCardList()
{
	lpCardNode head = NULL;
	if (cardList == NULL)
	{
		//Ϊ����ͷ�������ڴ�
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
//������:releaseCardList
//����: �ͷſ���Ϣ����
//����:void
//����ֵ:void

void releaseCardList()
{
	lpCardNode cur = cardList;
	lpCardNode next = NULL;

	while (cur != NULL)
	{
		next = cur->next; //�ͷ�cur���ǰ����next�������ĺ�̽��
		free(cur);//�ͷ�cur���
		cur = next;
	}
	cardList = NULL;
}


//[������]checkCard
//[����] ���ļ��϶�ȡ����Ϣ�������������в�ѯ����Ϣ������ȡ���������е�λ��
//[����] pName���ϻ����ţ�pPwd:�ϻ����룻pIndex:���ؿ���������
//[����ֵ] �ϻ����ṹ��
Card* checkCard(const char* pName, const char* pPwd, int* pIndex)
{
	lpCardNode cardNode = NULL;
	int nIndex = 0;

	//������ļ��л�ȡ����Ϣʧ�ܣ����ϻ�ʧ��
	if (FALSE==getCard())
	{
		return FALSE;
	}
	//ָ������ĵ�һ�����
	cardNode = cardList->next;
	//��������
	while (cardNode!=NULL)
	{
		//�����ϻ������жϿ��ź������Ƿ���ȷ
		if ((strcmp(cardNode->data.aName,pName)==0)&&(strcmp(cardNode->data.aPwd,pPwd)==0))
		{
			//���ؿ���Ϣ������ݵĵ�ַ
			*pIndex = nIndex;
			return &cardNode->data;
		}
		cardNode = cardNode->next;
		nIndex++;
	}

	return NULL;
}
