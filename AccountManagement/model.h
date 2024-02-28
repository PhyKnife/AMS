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
	struct CardNode* next;//ָ����һ����ָ��
}CardNode, *lpCardNode;

//������Ϣ�ṹ��
typedef struct Billing
{
	char aCardName[18];
	time_t tStart;
	time_t tEnd;
	float fAmount;
	int nStatus;
	int nDel;
}Billing;

//�ϻ���Ϣ�ṹ��
typedef struct LogonInfo
{
	char aCardName[18];
	time_t  tLogon;
	float fBalance;
}LogonInfo;


//�Ʒ���Ϣ���
typedef struct BillingNode
{
	Billing data;
	struct BillingNode *next;
}BillingNode, *lpBillingNode;

//�»���Ϣ�ṹ��
typedef struct SettleInfo
{
	char aCardName[18]; //����
	time_t tStart;   //�ϻ�ʱ��
	time_t tEnd; //�»�ʱ��
	float fAmount; //���ѽ��
	float fBalance; //���
}SettleInfo;

//��ֵ�˷ѽṹ��
typedef struct Money
{
	char aCardName[18];//����
	time_t tTime; //��ֵ���ѵ�ʱ��
	int nStatus; //״̬��0-��ֵ��1-�˷�
	float fMoney; //��ֵ�˷ѽ��
	int nDel;//ɾ����ʶ��0-δɾ����1-��ɾ��
}Money;

//��ֵ�˷���Ϣ�ṹ��
typedef struct MoneyInfo
{
	char aCardName[18]; //����
	float fMoney; //��ֵ�˷ѽ��
	float fBalance; //���
}MoneyInfo;

#endif

