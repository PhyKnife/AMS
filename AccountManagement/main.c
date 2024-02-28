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
		case 1:printf("添加卡\n");
			add();
			break;
		case 2:printf("查询卡\n");
			query();
			break;
		case 3:printf("上机\n");
			logon();
			break;
		case 4:printf("下机\n");
			settle();
			break;
		case 5:printf("充值\n");
			addMoney();
			break;
		case 6:printf("退费\n");
			refundMoney();
			break;
		case 7:printf("注销卡\n");
			annul();
			break;
		case 0:exitApp();
			printf("退出\n"); break;
		default: printf("输入的菜单序号错误"); break;
		}
	} while (nSelection != 0);

	return 0;
}
