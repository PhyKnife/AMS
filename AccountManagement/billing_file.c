#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"model.h"
#include"global.h"

//[������] saveBilling
//[����] ����Ʒ���Ϣ
//[����] pBilling:������Ϣ�ṹ��ָ��;pPath:����Ʒ���Ϣ�ļ�·��
//[����ֵ] TRUE����ɹ� FALSE����ʧ��

int saveBilling(const Billing* pBilling, const char* pPath)
{
	FILE* fp = NULL;

	//��׷�ӷ�ʽ��һ���������ļ�
	if ((fp=fopen(pPath,"ab"))==NULL)
	{
		//�����׷�ӷ�ʽʧ�ܣ�����ֻд��ʽ����һ���ļ�����
		if ((fp = fopen(pPath, "wb")) == NULL)
		{
			printf("----------��ӼƷ���Ϣ���ļ�ʧ��----------\n");
			return FALSE;
		}
	}
	//���Ʒ���Ϣ���浽�ļ���
	fwrite(pBilling,sizeof(Billing),1,fp);
	//close file
	fclose(fp);
	printf("---------�Ʒ���Ϣ�ɹ������ļ�----------\n");
	return TRUE;
}
//[������] readBilling
//[����] ��ȡ�Ʒ���Ϣ
//[����] pBilling���Ʒ���Ϣ�ṹ��ָ�룻pPath���Ʒ���Ϣ�ļ�·��
//[����ֵ] TURE����ȡ�ɹ���FALSE����ȡʧ��
int readBilling(Billing* pBilling, const char* pPath)
{
	FILE* fp = NULL;//�ļ��ṹ��ָ��
	int nIndex = 0;
	//��ֻ����ʽ��һ���������ļ�
	if ((fp = fopen(pPath, "rb")) == NULL)
	{
			printf("----------���ļ�ʧ�ܣ����ܶ�ȡ�Ʒ���Ϣ ----------\n");
			
			return FALSE;
	}
	//��ȡ�ļ�����
	while (!feof(fp))
	{
		if (fread(&pBilling[nIndex], sizeof(Billing), 1, fp)!=0)
		{
			nIndex++;
		}
	}
	//close file
	fclose(fp);
	
	return TRUE;

}

//[������] getBillingCount
//[����] ��ȡ�ļ��мƷ���Ϣ����
//[����] �Ʒ���Ϣ�ļ�·��
//[����ֵ] �Ʒ���Ϣ����
int getBillingCount(const char* pPath)
{
	FILE*fp = NULL;//�ļ�ָ��
	int nCount = 0;
	Billing* pBilling = (Billing*)malloc(sizeof(Billing));

	//��ֻ����ʽ���ļ�
	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		printf("-----���ļ�ʧ��,����ͳ�ƼƷ���Ϣ����-----");
		return 0;
	}
	//������ȡ�ļ�������
	while (!feof(fp))
	{
		if (fread(pBilling, sizeof(Billing), 1, fp) !=0)
		{
			nCount++;
		}
	}	
	//�ر��ļ�
	fclose(fp);
	free(pBilling);
	return nCount;

}
//[������] updateBilling
//[����] ���¼Ʒ���Ϣ�ļ��е�һ���Ʒ���Ϣ
//[����] pBilling:ָ��Ʒ���Ϣ�ṹ�壻pPath:�Ʒ���Ϣ�ļ�·����nIndex:�Ʒ���Ϣ���
//[����ֵ] TURE�����³ɹ���FALSE������ʧ��
int updateBilling(const Billing* pBilling, const char* pPath, int nIndex)
{
	FILE* fp = NULL;
	int nCount = 0;
	long lPosition = 0;
	Billing pbBuf;

	if ((fp=fopen(pPath,"rb+"))==NULL)
	{
		printf("----------�ļ��޷��򿪣����ܸ��¼Ʒ���Ϣ----------");
		return FALSE;
	}
	while ((!feof(fp))&&(nCount<nIndex))
	{
		if (fread(&pbBuf, sizeof(Billing), 1, fp) != 0) 
		{
			lPosition = ftell(fp);
		}
		nCount++;
	}
	fseek(fp, lPosition, 0);
	fwrite(pBilling, sizeof(Billing), 1, fp);
	printf("----------�Ʒ���Ϣ�ɹ����µ��ļ�----------");
	fclose(fp);

	return TRUE;
}