#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"model.h"
#include"global.h"

//[函数名] saveBilling
//[功能] 保存计费信息
//[参数] pBilling:计算信息结构体指针;pPath:保存计费信息文件路径
//[返回值] TRUE保存成功 FALSE保存失败

int saveBilling(const Billing* pBilling, const char* pPath)
{
	FILE* fp = NULL;

	//以追加方式打开一个二进制文件
	if ((fp=fopen(pPath,"ab"))==NULL)
	{
		//如果以追加方式失败，则以只写方式创建一个文件并打开
		if ((fp = fopen(pPath, "wb")) == NULL)
		{
			printf("----------添加计费信息打开文件失败----------\n");
			return FALSE;
		}
	}
	//将计费信息保存到文件中
	fwrite(pBilling,sizeof(Billing),1,fp);
	//close file
	fclose(fp);
	printf("---------计费信息成功存入文件----------\n");
	return TRUE;
}
//[函数名] readBilling
//[功能] 读取计费信息
//[参数] pBilling：计费信息结构体指针；pPath：计费信息文件路径
//[返回值] TURE：读取成功，FALSE：读取失败
int readBilling(Billing* pBilling, const char* pPath)
{
	FILE* fp = NULL;//文件结构体指针
	int nIndex = 0;
	//以只读方式打开一个二进制文件
	if ((fp = fopen(pPath, "rb")) == NULL)
	{
			printf("----------打开文件失败，不能读取计费信息 ----------\n");
			
			return FALSE;
	}
	//读取文件内容
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

//[函数名] getBillingCount
//[功能] 获取文件中计费信息的量
//[参数] 计费信息文件路径
//[返回值] 计费信息数量
int getBillingCount(const char* pPath)
{
	FILE*fp = NULL;//文件指针
	int nCount = 0;
	Billing* pBilling = (Billing*)malloc(sizeof(Billing));

	//以只读形式打开文件
	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		printf("-----打开文件失败,不能统计计费信息数量-----");
		return 0;
	}
	//逐条读取文件，计数
	while (!feof(fp))
	{
		if (fread(pBilling, sizeof(Billing), 1, fp) !=0)
		{
			nCount++;
		}
	}	
	//关闭文件
	fclose(fp);
	free(pBilling);
	return nCount;

}
//[函数名] updateBilling
//[功能] 更新计费信息文件中的一条计费信息
//[参数] pBilling:指向计费信息结构体；pPath:计费信息文件路径；nIndex:计费信息序号
//[返回值] TURE：更新成功；FALSE：更新失败
int updateBilling(const Billing* pBilling, const char* pPath, int nIndex)
{
	FILE* fp = NULL;
	int nCount = 0;
	long lPosition = 0;
	Billing pbBuf;

	if ((fp=fopen(pPath,"rb+"))==NULL)
	{
		printf("----------文件无法打开，不能更新计费信息----------");
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
	printf("----------计费信息成功更新到文件----------");
	fclose(fp);

	return TRUE;
}