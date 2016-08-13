#pragma once
#ifndef _DS_LINKLIST_H_
#define _DS_LINKLIST_H_

/////////////////һЩ����״̬/////////////////// 

#define DS_TRUE 1
#define DS_FALSE 0
#define DS_OK 1
#define DS_ERROR -1
#define DS_OVERFLOW -2 

#define ABOVE 1
#define EQUAL 0
#define BELOW -1

typedef int Status;

//////////////////////����ͷ�ļ�/////////////////////// 

#include<stddef.h>
#include"Raindrop.h"

/*��������Ϊ���������Ĺؼ��꣬��Ҫ�������������ͣ�
�뽫  Raindrop  ��  PRaindrop  �滻���������͵�����
���ɣ����ṹ������ṹ�������޸ġ�

��ʾ��ǰ��� P �ı�ʶ����ʾ������Ϊָ�򲻴� P �ı�
	ʶ�����͵�ָ�롣*/
#define ElemType Raindrop
#define PElemType PRaindrop

/////////////////////���ṹ ///////////////////////
typedef struct tagNode
{
	ElemType m_data;
	struct tagNode* m_prior;
	struct tagNode* m_next;
} Node,*PNode;

////////////////////����ṹ///////////////////////// 
typedef struct tagLinkList
{
	Node* m_head;	//����ͷָ�룬ָ��ͷ��� 
	Node* m_tail;	//����βָ�룬ָ��β��� 
	int m_length;	//������ 
	int haveUsed;	//�ñ�־ָʾ�������Ƿ�ʹ�ù�����ʹ�ù���ֵΪ0xFFFF
					//��δʹ�ù�����ֵΪ���ֵ�� 
} LinkList,*PLinkList;

//������������صĲ������� 

////////////////����ά��ϵ�к���/////////////////// 
Status InitList(LinkList* pList);//
Status DestroyList(LinkList* pList);//
Status ClearList(LinkList* pList);//
Status IsEmpty(const LinkList* pList);//
int GetLength(LinkList* pList);//
void TraverseList(LinkList* pList,void (*visit)(Node* pElem));//

/////////////////����ϵ�к���///////////////////////// 

Node* Locate(const LinkList* pList,int i);//
Node* GetFirst(const LinkList* pList);//
Node* GetLast(const LinkList* pList);//
Node* GetPrior(const Node* pElem);//
Node* GetNext(const Node* pElem);//
Node* Search(const LinkList* pList,
			ElemType* pCond,
			Status (*compare)(ElemType* pCondElem,ElemType* pCmpElem));//

///////////////����ϵ�к���////////////////////////// 

Status InsertFirst(LinkList* pList,ElemType* pInElem);// 
Status InsertLast(LinkList* pList,ElemType* pInElem);//
Status InsertBefore(LinkList* pList,int i,ElemType* pInElem);//
Status InsertAfter(LinkList* pList,int i,ElemType* pInElem);//

///////////////ɾ��ϵ�к���//////////////////////// 

Status Delete(LinkList* pList,Node* pDelElem);//
Status DeleteFirst(LinkList* pList,ElemType* pOutElem);//
Status DeleteLast(LinkList* pList,ElemType* pOutElem);//
Status DeleteByOrder(LinkList* pList,int i,ElemType* pOutElem);//

#endif
