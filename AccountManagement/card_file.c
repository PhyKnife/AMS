#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"model.h"
#include"global.h"
#include"tool.h"
#include"card_file.h"

//������ getCardConut
//���� ��ȡ����Ϣ�ļ��п���Ϣ����
//���� �ļ�·��
//����ֵ ����Ϣ����

int getCardCount(const char* pPath)
{
	FILE*fp = NULL;//�ļ�ָ��
	int nIndex = 0;//����Ϣ����
	Card* pCard = (Card*)malloc(sizeof(Card));

	//��ֻ����ʽ���ļ�
	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		printf("-----��ȡ����Ϣ�������ļ�ʧ��-----");
		return 0;
	}
	//���ж�ȡ�ļ�
	while (!feof(fp))
	{
		if (fread(pCard, sizeof(Card), 1, fp) != 0)
		{

			nIndex++;
		}
		
	}	//�ر��ļ�
		fclose(fp);
		free(pCard);

		return nIndex;
}


//��������readCard
//[����]�����ļ��ж�ȡ����Ϣ���ṹ������
//[����]��pCard��ָ��ṹ������,pPath���ļ�·��
//[����ֵ]:��ȷ��ȡ����TRUE����ȡ���󷵻�FALSE

int readCard(Card* pCard, const char* pPath)
{
	FILE* fp = NULL;//�ļ��ṹ��ָ��
	int nIndex = 0;//����Ϣ����
	

	//��ֻ����ʽ�򿪶������ļ�
	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		printf("-----��ȡ����Ϣ���ļ�ʧ��-----");
		return FALSE;
	}
	//���ļ������ж�ȡ����Ϣ
	while (!feof(fp))
	{

		if (fread(&pCard[nIndex], sizeof(Card), 1,fp) != 0)
		{
			
			nIndex++;
		}
	}	
		//�ر��ļ�
		fclose(fp);

		return TRUE;
	}

//������:praseCard[��ɾ��]
//[����]:��һ������Ϣ�ַ������������һ�����ṹ��
//[����]:����Ϣ�ַ���
//[����ֵ]:���ṹ��



//��������saveCard
//[����]��������Ϣ���浽�ļ���
//[����]��pCard��ָ��Ҫ����Ŀ���Ϣ�ṹ��,pPath��������ļ�·��
//[����ֵ]:����ɹ�����TRUE������ʧ�ܷ���FALSE
int saveCard(const Card* pCard, const char* pPath)
{
	FILE* fp = NULL;//�ļ��ṹ��ָ��
	
	//��ֻ���ķ�ʽ��һ���������ļ�
	if ((fp = fopen(pPath, "ab")) == NULL)
	{
		//�����׷�ӷ�ʽʧ�ܣ�����ֻд��ʽ����һ���ļ�����
		if ((fp = fopen(pPath, "wb")) == NULL)
		{
			printf("-----��ӿ���Ϣ���ļ�ʧ��-----\n");
			return FALSE;
		}
	}
	//������Ϣ���浽�ļ���
	fwrite(pCard, sizeof(Card), 1, fp);
		//�ر��ļ�
		fclose(fp);
		printf("----------����Ϣд���ļ��ɹ�----------\n");

		return TRUE;
		
}

//[������] updateCard
//[����] ���¿���Ϣ�ļ��ж�Ӧ��һ������Ϣ
//[����] pCard:���º�Ŀ�����;pPath:����Ϣ�ļ���·��;nIndex:��Ҫ���µĿ���Ϣ���ļ��еĿ����
//[����ֵ] TRUE���³ɹ���FALSE����ʧ��
int updateCard(const Card* pCard, const char* pPath, int nIndex)
{
	FILE* fp = NULL;//�ļ�ָ��
	int  nLine = 0;//�ļ��е�ǰ�����
	long lPosition = 0;//�ļ�λ�ñ��
	Card bBuf;


	//�Զ�дģʽ���ļ������ʧ�ܣ�����FALSE
	if ((fp=fopen(pPath,"rb+"))==NULL)
	{
		printf("----------���¿���Ϣ���ļ�ʧ��-----------\n");
		return FALSE;
	}
	//�����ļ�����ȡ�����ļ���λ��
	while ((!feof(fp))&&(nLine<nIndex))
	{
		
		//���ж�ȡ�ļ�����
		if (fread(&bBuf,sizeof(Card),1,fp)!=0)
		{
			//��ȡ�ļ���ʶλ��,ѭ�������һ�����ҵ���λ��
			//��Ҫ���µĿ���Ϣ��ʼλ�� 
			lPosition = ftell(fp);
			nLine++;
		}
	}
	//�Ƶ��ļ���ʶλ��
	fseek(fp, lPosition, 0);
	//����Ϣ���µ��ļ�
	fwrite(pCard, sizeof(Card), 1, fp);
		printf("----------����Ϣ���µ��ļ��ɹ�----------\n");
		//�ر��ļ�
	fclose(fp);
	return TRUE;
}