#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"model.h"
#include"menu.h"
#include"card_file.h"
#include"card_service.h"

void exitApp();

int main(void)
{
	int nSelection = -1;

	do
	{
		outputMenu();
		scanf_s("%d", &nSelection);
		getchar();
		switch (nSelection)
		{
		case 1:printf("��ӿ�\n");
			add();
			break;
		case 2:printf("��ѯ��\n");
			query();
			break;
		case 3:printf("�ϻ�\n");
			logon();
			break;
		case 4:printf("�»�\n");
			settle();
			break;
		case 5:printf("��ֵ\n");
			addMoney();
			break;
		case 6:printf("�˷�\n");
			refundMoney();
			break;
		case 7:printf("ע����\n");
			annul();
			break;
		case 0:exitApp();
			printf("�˳�\n"); break;
		default: printf("����Ĳ˵���Ŵ���"); break;
		}
	} while (nSelection != 0);

	return 0;
}
