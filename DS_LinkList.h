#pragma once
#ifndef _DS_LINKLIST_H_
#define _DS_LINKLIST_H_

/////////////////一些返回状态/////////////////// 

#define DS_TRUE 1
#define DS_FALSE 0
#define DS_OK 1
#define DS_ERROR -1
#define DS_OVERFLOW -2 

#define ABOVE 1
#define EQUAL 0
#define BELOW -1

typedef int Status;

//////////////////////引入头文件/////////////////////// 

#include<stddef.h>
#include"Raindrop.h"

/*以下两行为结点数据域的关键宏，若要更换数据域类型，
请将  Raindrop  和  PRaindrop  替换成其他类型的数据
即可，结点结构和链表结构皆无需修改。

提示：前面带 P 的标识符表示该类型为指向不带 P 的标
	识符类型的指针。*/
#define ElemType Raindrop
#define PElemType PRaindrop

/////////////////////结点结构 ///////////////////////
typedef struct tagNode
{
	ElemType m_data;
	struct tagNode* m_prior;
	struct tagNode* m_next;
} Node,*PNode;

////////////////////链表结构///////////////////////// 
typedef struct tagLinkList
{
	Node* m_head;	//链表头指针，指向头结点 
	Node* m_tail;	//链表尾指针，指向尾结点 
	int m_length;	//链表长度 
	int haveUsed;	//该标志指示该链表是否被使用过，若使用过则值为0xFFFF
					//若未使用过，其值为随机值。 
} LinkList,*PLinkList;

//以下是链表相关的操作函数 

////////////////基本维护系列函数/////////////////// 
Status InitList(LinkList* pList);//
Status DestroyList(LinkList* pList);//
Status ClearList(LinkList* pList);//
Status IsEmpty(const LinkList* pList);//
int GetLength(LinkList* pList);//
void TraverseList(LinkList* pList,void (*visit)(Node* pElem));//

/////////////////查找系列函数///////////////////////// 

Node* Locate(const LinkList* pList,int i);//
Node* GetFirst(const LinkList* pList);//
Node* GetLast(const LinkList* pList);//
Node* GetPrior(const Node* pElem);//
Node* GetNext(const Node* pElem);//
Node* Search(const LinkList* pList,
			ElemType* pCond,
			Status (*compare)(ElemType* pCondElem,ElemType* pCmpElem));//

///////////////插入系列函数////////////////////////// 

Status InsertFirst(LinkList* pList,ElemType* pInElem);// 
Status InsertLast(LinkList* pList,ElemType* pInElem);//
Status InsertBefore(LinkList* pList,int i,ElemType* pInElem);//
Status InsertAfter(LinkList* pList,int i,ElemType* pInElem);//

///////////////删除系列函数//////////////////////// 

Status Delete(LinkList* pList,Node* pDelElem);//
Status DeleteFirst(LinkList* pList,ElemType* pOutElem);//
Status DeleteLast(LinkList* pList,ElemType* pOutElem);//
Status DeleteByOrder(LinkList* pList,int i,ElemType* pOutElem);//

#endif
