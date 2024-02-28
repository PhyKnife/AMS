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
	printf("----------�˵�----------\n");
	printf("1.��ӿ�               *\n");
	printf("2.��ѯ��               *\n");
	printf("3.�ϻ�                 *\n");
	printf("4.�»�                 *\n");
	printf("5.��ֵ                 *\n");
	printf("6.�˷�                 *\n");
	printf("7.ע����               *\n");
	printf("0.�˳�                 *\n");
	printf("��ѡ��˵������(0-8): *\n");
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

	printf("�����뿨��(����Ϊ1~18):");
	scanf("%s", name);
	if (getSize(name) >= 18)
	{
		printf("����Ŀ��ų��ȳ������ֵ\n");
		return;
	}
	if (queryCardInfo(name) != NULL)
	{
		printf("���뿨���Ѵ��ڣ�����������\n");
		return;
	}
	strcpy(card.aName, name);
	printf("���������룺");
	scanf("%s", pwd);
	if (getSize(pwd) >= 8)
	{
		printf("��������볤�ȳ������ֵ\n");
		return;
	}
	strcpy(card.aPwd, pwd);
	printf("�����뿪����RMB����");
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

	printf("----------��ӵĿ���Ϣ����----------\n");
	printf("����\n����\t״̬\t�������\n");
	printf("%s\t%s\t%d\t%0.1f\n\n", card.aName, card.aPwd, card.nStatus, card.fBalance);
	if (FALSE == addCardInfo(card))
	{
		printf("---------��ӿ���Ϣ���ļ�ʧ��---------\n");
	}
	else
	{
		printf("----------��ӿ���Ϣ���ļ��ɹ�----------\n");
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

	printf("������Ҫ��ѯ�Ŀ���(����Ϊ1~18):");
	scanf("%s", name);
	printf("1.��ȷ��ѯ��2.ģ����ѯ������1��2����");
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
		printf("----------û�в�ѯ���ÿ�����Ϣ----------\n");
	}
	else
	{
		printf("----------��ѯ���ÿ���Ϣ����----------\n");
		printf("����\t״̬\t���\t�ۼƽ��\tʹ�ô���\t�ϴ�ʹ��ʱ��\n");

		if (icha == 1)//��ȷ��ѯ
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

//[������] logon
//[����] �����ϻ�����Ϣ����ѯ��������ϻ�����Ϣ
//[����] void
//[����ֵ] void

void logon()
{
	char aName[18] = { 0 };//�ϻ�����
	char aPwd[8] = { 0 };//�ϻ�������
	char aLastTime[30];//���ָ����ʽ�ַ�����ʱ��
	LogonInfo *pInfo = NULL;
	int nResult = 0;

	//��ʾ�������ϻ�����
	printf("�������ϻ�����(����Ϊ1~18):");
	scanf("%s",aName);
	//�ж�����Ŀ����Ƿ����Ҫ��
	if (getSize(aName)>=18)
	{
		printf("����Ŀ��ų��ȳ������ֵ\n");
		return;
	}
	//��ʾ�������ϻ�����
	printf("�������ϻ�����(����Ϊ1~8):");
	scanf("%s", aPwd);
	//�ж�����������Ƿ����Ҫ��
	if (getSize(aPwd)>= 8)
	{
		printf("��������볤�ȳ������ֵ\n");
		return;
	}
	//��ʼ�ϻ� ��ȡ�ϻ����
	pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));
	nResult = doLogon(aName, aPwd, pInfo);
	switch (nResult)
	{
	case 0:
		printf("----------�ϻ�ʧ��----------\n");
		break;
	case 1:
		printf("----------�ϻ��Ŀ���Ϣ����----------\n");
		printf("����\t���\t�ϻ�ʱ��\n");
		timeToString(pInfo->tLogon, aLastTime);
		printf("%s\t%0.1f\t%s\n\n",pInfo->aCardName,pInfo->fBalance,aLastTime);
		printf("----------�ϻ��ɹ�----------\n");
		break;
	case 2:
		printf("----------�ÿ�����ʹ��----------\n");
		break;
	case 3:
		printf("----------����----------\n");
		break;
	}
	//�ͷ��ϻ���Ϣ
	free(pInfo);
}
//��������exitApp
//����:�˳�Ӧ�ó���
//������void
//����ֵ��void

void exitApp()
{
	releaseList();
}

//������ settle
//���� �����»�����Ϣ����ѯ��������ϻ�����Ϣ
//���� void
//����ֵ void
void settle() 
{
	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	int nResult = -1;
	SettleInfo* pInfo = NULL;
	char aStartTime[30] = { 0 };
	char aEndTime[30] = { 0 };

	pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

	printf("�������»�����(����Ϊ1~18):");
	scanf("%s", aName);

	printf("�������»�����(����Ϊ1~8):");
	scanf("%s", aPwd);

	//�����»�
	nResult = doSettle(aName, aPwd, pInfo);

	printf("\n----------�»���Ϣ����----------\n");
	switch (nResult)
	{
	case 0:
		printf("----------�»�ʧ��----------\n");
		break;
	case 1:
		printf("����\t����\t���\t�ϻ�ʱ��\t\t�»�ʱ��\n");
		timeToString(pInfo->tStart, aStartTime);
		timeToString(pInfo->tEnd, aEndTime);
		printf("%s\t%0.1f\t%0.1f\t%s\t%s\n\n", 
			pInfo->aCardName,pInfo->fAmount,pInfo->fBalance, aStartTime,aEndTime);
		printf("----------�»��ɹ�----------\n");
		break;
	case 2:
		printf("----------�ÿ�δ��ʹ��----------\n");
		break;
	case 3:
		printf("----------����----------\n");
		break;
	default:
			break;
	}

	free(pInfo);
}

//[������] addMoney
//[����] ��ֵ
//[����] void
//[����ֵ] void
void addMoney()
{
	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	float fAmount = 0;
	MoneyInfo sMoneyInfo;

	printf("�������ֵ����(����Ϊ1~18):");
	scanf("%s",aName);
	printf("�������ֵ����(����Ϊ1~8):");
	scanf("%s",aPwd);
	printf("�������ֵ���(RMB):");
	scanf("%f",&fAmount);

	//�����ֵ���
	sMoneyInfo.fMoney = fAmount;

	//�жϳ�ֵ�Ƿ�ɹ�
	if (TRUE==doAddMoney(aName,aPwd,&sMoneyInfo))
	{
		//��ʾ��ֵ��Ϣ
		printf("----------��ֵ��Ϣ����----------\n");
		//�����ͷ
		printf("����\t��ֵ���\t���\n:");
		printf("%s\t%0.1f\t\t%0.1f\n",sMoneyInfo.aCardName,sMoneyInfo.fMoney,sMoneyInfo.fBalance);
	}
	else
	{
		printf("----------��ֵʧ��-----------\n");
	}

}

//[������] refundMoney
//[����] �˷�
//[����] void
//[����ֵ] void

void refundMoney()
{
	char aName[18] = { 0 };//����
	char aPwd[8] = { 0 }; //����
	int nResult = -1;
	MoneyInfo sMoneyInfo; // �˷���Ϣ

	printf("�������˷ѿ���(����Ϊ1~18):");

	scanf("%s",aName);

	printf("�������˷�����(����Ϊ1 ^8):");

	scanf("%s",aPwd);

			// �����˷�

	nResult = doRefundMoney(aName, aPwd, &sMoneyInfo);

			// �����˷ѽ������ʾ��ͬ��Ϣ

		switch (nResult) 
		{
			case 0://�˷�ʧ��
				printf("---**--- �˷�ʧ��!---**---\n");
				break;
			case 1 : //�˷ѳɹ�
				printf("---**---�˷���Ϣ����- **-- - \n");
				//�����ͷ
				printf("����\t�˷ѽ��\t���\n");
				//����˷���Ϣ
				printf("%s\t%0.1f\t\t%0.1f\n",sMoneyInfo.aCardName, sMoneyInfo.fMoney, sMoneyInfo.fBalance);
				break;
			case 2:
				printf("----------�ÿ�����ʹ�û���ע��----------\n");
				break;
			case 3: //������
				printf("**-*-- - ������!----\n"); 
				break;
			default:
				break;
		}
}
//[������] annual
//[����] ע����
//[����] void
//[����ֵ] void
void annul()
{
	Card card;

	printf("������Ҫע���Ŀ���:");
	scanf("%s",card.aName);

	printf("����������:");
	scanf("%s", card.aPwd);
	//��ʾע������Ϣ
	if (FALSE==annulCard(&card))
	{
		printf("----------ע����ʧ��,�ÿ������ѱ�ע��----------\n");
		return;
	}
	else
	{
		printf("----------ע����Ϣ����:----------\n");
		printf("����\t�˿���\n");
		printf("%s\t%0.1f\n",card.aName,card.fBalance);
		return;
	}
}

