#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

#include "money_file.h"
#include"model.h"
#include"global.h"

//[������] saveMoney
//[����] ����ֵ�˷���Ϣ���浽�ļ���
//[����] pMoney:��ֵ�˷ѽṹ�壻pPath:��ֵ�˷���Ϣ����·��
//[����ֵ] TURE ������ɹ���FALSE������ʧ��
int saveMoney(const Money * pMoney, const char * pPath)
{
	FILE* fp = NULL;

	//��׷�ӷ�ʽ���ļ��������ʧ�ܣ�����ֻд��ģʽ���ļ�
	if ((fp=fopen(pPath,"ab"))==NULL)
	{
		if ((fp = fopen(pPath, "wb")) == NULL)
		{
			printf("----------д���ֵ�˷���Ϣ���ļ�ʧ��----------\n");
			return FALSE;
		}
	}
	//����ֵ�˷���Ϣ���浽�ļ�
	fwrite(pMoney, sizeof(Money), 1, fp);

	fclose(fp);
	printf("----------��ֵ�˷���Ϣ�ɹ����浽�ļ�----------\n\n");

	return TRUE;
}
