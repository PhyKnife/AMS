#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

#include "money_file.h"
#include"model.h"
#include"global.h"

//[函数名] saveMoney
//[功能] 将充值退费信息保存到文件中
//[参数] pMoney:充值退费结构体；pPath:充值退费信息保存路径
//[返回值] TURE ：保存成功；FALSE：保存失败
int saveMoney(const Money * pMoney, const char * pPath)
{
	FILE* fp = NULL;

	//以追加方式打开文件，如果打开失败，则以只写的模式打开文件
	if ((fp=fopen(pPath,"ab"))==NULL)
	{
		if ((fp = fopen(pPath, "wb")) == NULL)
		{
			printf("----------写入充值退费信息打开文件失败----------\n");
			return FALSE;
		}
	}
	//将充值退费信息保存到文件
	fwrite(pMoney, sizeof(Money), 1, fp);

	fclose(fp);
	printf("----------充值退费信息成功保存到文件----------\n\n");

	return TRUE;
}
