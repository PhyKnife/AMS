#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#include"model.h"
#include"billing_file.h"
#include"global.h"

lpBillingNode billingList = NULL;//�Ʒ���Ϣ����

//[������] addBilling
//[����] �ϻ�ʱ���������Ϣ���ļ�
//[����] pBilling:ָ��������Ϣ�ṹ��;pName:�ϻ�����
//[����ֵ] TRUE����ɹ� FALSE����ʧ��
int addBilling(const char* pName, Billing *pBilling)
{
	//������Ϣд��ṹ��
	strcpy(pBilling->aCardName, pName);
	pBilling->tStart = time(NULL);
	pBilling->tEnd = 0;
	pBilling->fAmount = 0;
	pBilling->nStatus = 0;
	pBilling->nDel = 0;

	//������Ϣ�ṹ��д���ļ���
	if (FALSE==saveBilling(pBilling,BILLINGPATH))
	{
		return FALSE;
	}
	return TRUE;
}

//[������] initBillingList
//[����] ��ʼ���Ʒ���Ϣ����
//[����] void
//[����ֵ] void
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
//[������] releaseBillingList
//[����] �ͷżƷ���Ϣ����
//[����] void
//[����ֵ] void
void releaseBillingList()
{
	lpBillingNode cur = billingList;
	lpBillingNode next = NULL;
	//��������
	while (cur != NULL)
	{
		next = cur->next;//����ڴ��ͷ�Ǯ��next��������
		free(cur);//�ͷŽ���ڴ�
		cur = next;
	}
	billingList = NULL;
}
//[������] getBilling
//[����] �ӼƷ���Ϣ�ļ��У���ȡ�Ʒ���Ϣ���浽������
//[����] void
//[����ֵ] TRUE����ȡ��Ϣ�ɹ���FALSE����ȡ��Ϣʧ��
int getBilling()
{
	int nCount = 0;
	Billing* pBilling = NULL;//�Ʒ���Ϣ
	lpBillingNode node = NULL;//
	int i = 0;//ʵ�ʿ���Ϣ��
	lpBillingNode cur = NULL;//

	//����������Ѵ��ڵ�����
	if (billingList != NULL)
	{
		releaseBillingList();
	}
	//��ʼ������
	initBillingList();

	//��ȡ�Ʒ���Ϣ����
	nCount = getBillingCount(BILLINGPATH);
	//��̬�����ڴ�
	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);
	if (pBilling != NULL)
	{
		if (FALSE == readBilling(pBilling, BILLINGPATH))
		{
			free(pBilling);
			return FALSE;
		} 
	
		//���Ʒ���Ϣ���浽������
	
		for (i = 0, node = billingList; i < nCount; i++)
		{
			cur = (lpBillingNode)malloc(sizeof(BillingNode));

			if (cur == NULL)
			{
				free(pBilling);

				return FALSE;
			}
			memset(cur, 0, sizeof(BillingNode));
			//������Ϣ���浽�����
			cur->data = pBilling[i];
			cur->next = NULL;
			node->next = cur;
			//β���ָ�����
			node = cur;
		}
		//�ͷ��ڴ�
		free(pBilling);
		return TRUE;
	}
	return FALSE;
}
//[������] queryBilling
//[����] �ڼƷ���Ϣ�����У���ѯ��Ӧ���ļƷ���Ϣ������ȡ���������е�������
//[����] pName:Ҫ��ѯ�ļƷ���Ϣ�Ŀ��ţ�pIndex����ѯ���ļƷ���Ϣ�������е�������
//[����ֵ] ��ѯ���ļƷ���Ϣָ��
Billing* queryBilling(const char* pName, int* pIndex)
{
	lpBillingNode node = NULL;
	int nIndex = 0;

	
	if (FALSE == getBilling())
	{
		return NULL;
	}
	//ָ������ĵ�һ�����
	node = billingList->next;
	//��������
	while (node != NULL)
	{
		//��ѯ��������ͬ������δ����ļƷ���Ϣ
		if (strcmp(node->data.aCardName, pName) == 0 && node->data.nStatus == 0)
		{
			//���ؽ�����ݵĵ�ַ
		
			return &node->data;
		}
		node = node->next;
		nIndex++;
		*pIndex = nIndex;
	}

	return NULL;
}
