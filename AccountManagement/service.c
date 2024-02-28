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


//[������] addCardInfo
//[����] ��ӿ���Ϣ
//[����] ����Ϣ�ṹ��
//[����ֵ] TURE����ɹ���FALSE����ʧ��

int addCardInfo(Card card)
{
	if (TRUE==saveCard(&card,CARDPATH))
	{
		return TRUE;
	}

	return FALSE;
}

//[������] queryCardInfo
//[����] ��ȷ��ѯ����Ϣ
//[����] pName:ָ���û�Ҫ��ѯ�Ŀ���
//[����ֵ] ָ����Ϣ�ṹ��ָ��
Card* queryCardInfo(const char* pName)
{
	Card* pCard = NULL;
	pCard = queryCard(pName);
	return pCard;
}

//[������] queryCardsInfo
//[����] ģ����ѯ����Ϣ
//[����] pName:ָ���û�Ҫ��ѯ�Ŀ���,pIndex:ָ��鵽�Ŀ���Ϣ������
//[����ֵ] ָ����Ϣ�ṹ��ָ��
Card* queryCardsInfo(const char* pName,int* pIndex)
{
	Card* pCard = NULL;
	pCard = queryCards(pName,pIndex);
	return pCard;
}
//[������] doLogon
//[����] �ϻ�
//[����] pName:�ϻ�����;pPwd:�ϻ�����;pInfo:ָ���ϻ���Ϣ�ṹ��
//[����ֵ] TRUE:�ϻ��ɹ���FALSE:�ϻ�ʧ��

int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo)
{
	Card* pCard = NULL;//
	int nIndex = 0;//����Ϣ�������е����������ڸ��¿���Ϣ
	Billing billing;//�Ʒ���Ϣ

	//���ݿ��ź����룬�������л�ȡ����Ϣ�Ϳ���Ϣ�������е�����
	pCard = checkCard(pName, pPwd, &nIndex);

	//�������ϢΪ�գ���ʾû�иÿ���Ϣ���ϻ�ʧ��
	if (pCard == NULL)
	{
		return FALSE;
	}
	//�����״̬��Ϊ0����ʾ�ÿ������ϻ�
	if (pCard->nStatus != 0)
	{
		return UNUSE;
	}
	//����������Ϊ0�������ϻ�
	if (pCard->fBalance <= 0)
	{
		return NOENOUGHMONEY;
	}
	//��������ϻ����¿���Ϣ
	pCard->nStatus = 1;//״̬Ϊ����ʹ��
	pCard->nUseCount++;//ʹ�ô���+1
	pCard->tLastTime = time(NULL);//�������ʹ��ʱ��Ϊ��ǰʱ��
	//�����ļ��еĿ���Ϣ
	if (FALSE == updateCard(pCard, CARDPATH, nIndex))
	{
		return FALSE;
	}
	//������Ѽ�¼
	strcpy(billing.aCardName, pName);//�ϻ�����
	billing.tStart = time(NULL);//�ϻ�ʱ��
	billing.tEnd = 0;//�»�ʱ��
	billing.nStatus = 0;//����״̬
	billing.fAmount = 0;//���ѽ��
	billing.nDel = 0;//ɾ����ʶ

	//���Ʒ���Ϣ���浽�ļ���
	if (TRUE == saveBilling(&billing, BILLINGPATH))
	{
		//��װ�ϻ���Ϣ
		strcpy(pInfo->aCardName, pName);
		pInfo->fBalance = pCard->fBalance;
		pInfo->tLogon = billing.tStart;

		return TRUE;
	}

	return FALSE;
}
//[������] releaseList
//[����] �˳�Ӧ�ó���ʱ���ͷ������ڴ�
//[����] void
//[����ֵ] void
void releaseList()
{
	releaseCardList(); //�ͷſ���Ϣ�����ڴ�
	releaseBillingList();

}

//[������] doSettle
//[����] �»�
//[����] pName:�»����ţ�pPwd:�»����룻pInfo:ָ���»���Ϣ�ṹ��
//[����ֵ] �»���Ϣֵ����ͬ��������ͬ��Ϣ
int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo)
{
	Card* pCard = NULL;
	Billing* pBilling = NULL;
	int nIndex = 0;
	int nPosition = 0;
	double dbAmount = 0.0;
	float fBalance = 0.0;

	//��ѯ�ϻ���
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard==NULL)
	{
		printf("----------�޸ÿ���Ϣ������������----------\n");
		return FALSE;

	}
	if (pCard->nStatus!=1)
	{
		printf("�ÿ�δ�ϻ�\n");
		return UNUSE;
	}
	pBilling = queryBilling(pName, &nPosition);
	//���Ϊ�գ���ʾû�иÿ���Ϣ������FALSE
	if (pBilling==NULL)
	{
		printf("----------�޸ÿ���Ϣ������������----------\n");
		return FALSE;
	}
	dbAmount = getAmount(pBilling->tStart);

	fBalance = pCard->fBalance - (float)dbAmount;
	if (fBalance<0)
	{
		return NOENOUGHMONEY;
	}
	//���¿���Ϣ
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

	//��װ�»���Ϣ
	strcpy(pInfo->aCardName, pName);
	pInfo->fAmount = (float)dbAmount;
	pInfo->fBalance = fBalance;
	pInfo->tStart = pBilling->tStart;
	pInfo->tEnd = time(NULL);

	return TRUE;
}
//[������] getAmount
//[����] �����ϻ�ʱ�䣬�������ѽ��
//[����] �ϻ�ʱ��
//[����ֵ] ���ѽ��
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
//[������] doAddMoney
//[����] ���г�ֵ����
//[����] pName:��ֵ���Ŀ��ţ�pPwd:��ֵ�����룻pMoneyInfo:��ֵ��Ϣ
//[����ֵ] TURE����ֵ�ɹ���FALSE����ֵʧ��

int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo)
{
	Card* pCard = NULL;
	int nIndex = 0;
	Money sMoney;

	//��ѯ��ֵ��
	pCard = checkCard(pName, pPwd, &nIndex);
	

	if (pCard==NULL)
	{
		printf("----------�޸ÿ���Ϣ�����ܳ�ֵ----------\n");
		return FALSE;
	}
	if (pCard->nStatus!=0&&pCard->nStatus!=1)
	{
		return FALSE;
	}

	//������Գ�ֵ�����¿���Ϣ
	pCard->fBalance += pMoneyInfo->fMoney;
	pCard->fTotalUse += pMoneyInfo->fMoney;
	//�����ļ��еĿ���Ϣ
	if (FALSE==updateCard(pCard,CARDPATH,nIndex))
	{
		return FALSE;
	}
	//��װ��ֵ��Ϣ
	strcpy(sMoney.aCardName, pCard->aName);
	sMoney.tTime = time(NULL);
	sMoney.nStatus = 0;
	sMoney.fMoney = pMoneyInfo->fMoney;
	sMoney.nDel = 0;
	//����ֵ��¼���浽�ļ���
	if (TRUE==saveMoney(&sMoney,MONEYPATH))
	{
		strcpy(pMoneyInfo->aCardName,sMoney.aCardName);
		pMoneyInfo->fBalance = pCard->fBalance;

		return TRUE;
	}


	return FALSE;
}

//[������] doRefundMoney
//[����] �����˷Ѳ���
//[����]pName �˷ѿ���; pPwd �˷ѿ�����; pMoneyInfo ��ֵ�˷���Ϣ�ṹ��
//[����ֵ] TRUE �˷ѳɹ�; FALSE�˷�ʧ��
int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo)
{
	Card* pCard = NULL;
	int nIndex = 0; // ����Ϣ�������е�������
	float fBalance = 0.0;// �������
	Money sMoney;
	//��ѯ�˷ѿ�
	pCard = checkCard(pName, pPwd, &nIndex);
	// ���Ϊ�գ���ʾû�иÿ���Ϣ������FALSE

	if (pCard == NULL) {


		printf("---***---�޸ÿ���Ϣ�������˷�!---***---\n");

		return FALSE;
	}
	//�жϸÿ��Ƿ�δʹ�ã�ֻ��δʹ�õĿ����ܽ����˷Ѳ���
	if (pCard->nStatus != 0)
	{
		return	UNUSE;
	}

	//���������0�������˷�
	fBalance = pCard->fBalance;
	if (fBalance <= 0)
	{
		return NOENOUGHMONEY;
	}
	//���¿���Ϣ
	pCard->fBalance = 0;//���
	pCard->fTotalUse -= fBalance;  //�ۼƽ��
//�����ļ��еĿ���Ϣ
	if (FALSE == updateCard(pCard, CARDPATH, nIndex))
	{

		return FALSE;
	}

	//����˷���Ϣ
	strcpy(sMoney.aCardName, pCard->aName);
	sMoney.tTime = time(NULL);
	sMoney.nStatus = 1;
	sMoney.fMoney = fBalance;
	sMoney.nDel = 0;

	//�����ļ��еĳ�ֵ�˷���Ϣ
	if (TRUE==saveMoney(&sMoney,MONEYPATH))
	{
		strcpy(pMoneyInfo->aCardName, sMoney.aCardName);
		pMoneyInfo->fMoney = sMoney.fMoney;
		pMoneyInfo->fBalance = pCard->fBalance;

		return TRUE;
	}

	return TRUE;
}
//[������] annulCard
//[����] ע����
//[����] pCard ����Ϣ�ṹ��
//[����ֵ] TRUE ע���ɹ���FALSE ע��ʧ��
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
	//ֻ��δ�ϻ��Ŀ�����ע��
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
