#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"model.h"
#include"global.h"
#include"tool.h"
#include"card_file.h"

//函数名 getCardConut
//功能 获取卡信息文件中卡信息数量
//参数 文件路径
//返回值 卡信息数量

int getCardCount(const char* pPath)
{
	FILE*fp = NULL;//文件指针
	int nIndex = 0;//卡信息数量
	Card* pCard = (Card*)malloc(sizeof(Card));

	//以只读形式打开文件
	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		printf("-----读取卡信息数量打开文件失败-----");
		return 0;
	}
	//逐行读取文件
	while (!feof(fp))
	{
		if (fread(pCard, sizeof(Card), 1, fp) != 0)
		{

			nIndex++;
		}
		
	}	//关闭文件
		fclose(fp);
		free(pCard);

		return nIndex;
}


//函数名：readCard
//[功能]：从文件中读取卡信息到结构体数组
//[参数]：pCard：指向结构体数组,pPath：文件路径
//[返回值]:正确读取返回TRUE，读取错误返回FALSE

int readCard(Card* pCard, const char* pPath)
{
	FILE* fp = NULL;//文件结构体指针
	int nIndex = 0;//卡信息数量
	

	//以只读形式打开二进制文件
	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		printf("-----读取卡信息打开文件失败-----");
		return FALSE;
	}
	//从文件中逐行读取卡信息
	while (!feof(fp))
	{

		if (fread(&pCard[nIndex], sizeof(Card), 1,fp) != 0)
		{
			
			nIndex++;
		}
	}	
		//关闭文件
		fclose(fp);

		return TRUE;
	}

//函数名:praseCard[已删除]
//[功能]:将一条卡信息字符串解析后放入一个卡结构体
//[参数]:卡信息字符串
//[返回值]:卡结构体



//函数名：saveCard
//[功能]：将卡信息保存到文件中
//[参数]：pCard：指向要保存的卡信息结构体,pPath：保存的文件路径
//[返回值]:保存成功返回TRUE，保存失败返回FALSE
int saveCard(const Card* pCard, const char* pPath)
{
	FILE* fp = NULL;//文件结构体指针
	
	//以只读的方式打开一个二进制文件
	if ((fp = fopen(pPath, "ab")) == NULL)
	{
		//如果以追加方式失败，则以只写方式创建一个文件并打开
		if ((fp = fopen(pPath, "wb")) == NULL)
		{
			printf("-----添加卡信息打开文件失败-----\n");
			return FALSE;
		}
	}
	//将卡信息保存到文件中
	fwrite(pCard, sizeof(Card), 1, fp);
		//关闭文件
		fclose(fp);
		printf("----------卡信息写入文件成功----------\n");

		return TRUE;
		
}

//[函数名] updateCard
//[功能] 更新卡信息文件中对应的一条卡信息
//[参数] pCard:更新后的卡内容;pPath:卡信息文件的路径;nIndex:需要更新的卡信息在文件中的卡序号
//[返回值] TRUE更新成功，FALSE更新失败
int updateCard(const Card* pCard, const char* pPath, int nIndex)
{
	FILE* fp = NULL;//文件指针
	int  nLine = 0;//文件中当前卡序号
	long lPosition = 0;//文件位置标记
	Card bBuf;


	//以读写模式打开文件，如果失败，返回FALSE
	if ((fp=fopen(pPath,"rb+"))==NULL)
	{
		printf("----------更新卡信息打开文件失败-----------\n");
		return FALSE;
	}
	//遍历文件，获取卡在文件中位置
	while ((!feof(fp))&&(nLine<nIndex))
	{
		
		//逐行读取文件内容
		if (fread(&bBuf,sizeof(Card),1,fp)!=0)
		{
			//获取文件标识位置,循环的最后一次是找到的位置
			//在要更新的卡信息开始位置 
			lPosition = ftell(fp);
			nLine++;
		}
	}
	//移到文件标识位置
	fseek(fp, lPosition, 0);
	//将信息更新到文件
	fwrite(pCard, sizeof(Card), 1, fp);
		printf("----------卡信息更新到文件成功----------\n");
		//关闭文件
	fclose(fp);
	return TRUE;
}