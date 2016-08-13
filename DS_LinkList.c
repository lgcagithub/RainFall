#include<stdlib.h>
#include<string.h>
#include"DS_LinkList.h"

Status InitList(LinkList* pList)
{
/*
���ã��ú���������ʼ��pList��ָ�������ṹ������
	����ͷ��㡢��ʼ��βָ��ͳ��ȡ� 
*/
	if(!pList) return DS_FALSE;
	
	/*��Ϊ��Щ�����������δ��ʼ���ı�����0������
	���ܻᵼ����������ִ�г����쳣�����Ǵ��ж���
	���������Ƿ񱻳�ʼ������������ʼ����������һ
	��ĳ�ʼ��������У���δ��ʼ��������δ��ʼ����
	ֵ��Ҫ�������⴦��
	*/
	if(pList->haveUsed ^ 0xFFFF)	//δ��ʼ���� 
	{											
		pList->m_head = pList->m_tail = NULL;	 
		pList->m_length = 0;					 
	}											
	/*����ʼ��������pList����һ���ձ��������١������ܷ�ֹͬ
	���ô������ڴ�й¶���⡣*/											 																								 
	else if(pList->m_head && !DestroyList(pList)) return DS_FALSE;	
																
	//����ͷ���,������ʧ���򷵻�																																															 	
	pList->m_head = (Node*)malloc(sizeof(Node));	 
	if(!pList->m_head) return DS_OVERFLOW;				 
	pList->m_head->m_prior = pList->m_head->m_next = NULL;
	//��ʼ״̬βָ�����ͷָ�� 
	pList->m_tail = pList->m_head;		
	//��ʼ״̬��Ϊ0������ͷ���			
	pList->m_length = 0;							 
	
	//�˲�˵���������ѱ���ʼ������
	pList->haveUsed = 0xFFFF;		 
	
	return DS_OK;
}

Status DestroyList(LinkList* pList)
{
/*
���ã��ú�����������pList��ָ�������ṹ����
	�ͷŵ����н�㣬����ͷ��㡣 
*/
	//�ͷ����з�ͷ���
	if(!ClearList(pList)) return DS_FALSE;		 
	
	//�ͷ�ͷ���
	if(pList->m_head) free(pList->m_head);	 
	pList->m_head = pList->m_tail = NULL;
	pList->m_length = 0;
	
	return DS_OK;
}

Status ClearList(LinkList* pList)
{
/*
���ã��ú�����������pList��ָ�������ṹ����
	������ͷ��㡣 
*/
	if(!pList) return DS_FALSE;
	
	/*�����ȷ�0�����������ѭ����ɾ������
	ĩβ��ʼ��*/
	if(pList->m_length)	 
	{
		while(DeleteLast(pList,NULL));
	}
	pList->m_length = 0;
	
	return DS_OK;
}

Status IsEmpty(const LinkList* pList)
{
/*
���ã��ж�pList��ָ��������Ƿ�Ϊ�ձ����Ƿ�
	��DS_TRUE�����򷵻�DS_FALSE�� 
*/
	if(!pList) return DS_ERROR;
	if(pList->m_length) return DS_FALSE;
	return DS_TRUE;
}

int GetLength(LinkList* pList)
{
/*
���ã�����pList��ָ�������ĳ��ȡ� 
*/
	if(!pList) return DS_ERROR;
	return pList->m_length;
}

void TraverseList(LinkList* pList,void (*visit)(Node* pElem))
{
/*
���ã��ú�����������pList��ָ�������ÿ����㣬
	������ʽ�Ӻ���ָ��visit������ 
*/
	
	if(!pList || !pList->m_length) return;
	if(!visit) return;
	
	Node* p = pList->m_head->m_next;
	while(p)
	{
		visit(p);
		p = p->m_next;
	}
}


Node* Locate(const LinkList* pList,int i)
{
/*
���ã��ú�������pList��ָ������ĵ�i�����ĵ�ַ��
	����i��������1��i ��pList->m_length�� 
*/
	if(!pList) return NULL;
	if(i < 1 || i > pList->m_length) return NULL;
	
	/*�������д���Ѱ�ҵ�i�����ĵ�ַ*/
	int order = 1;
	Node* pos = pList->m_head->m_next;
	for(;order != i; order++,pos = pos->m_next);
	
	return pos;
}

Node* GetFirst(const LinkList* pList)
{
/*
���ã��ú�������pList��ָ����ĵ�һ�����ĵ�ַ�� 
*/
	if(!pList || !pList->m_length) return NULL;
	
	return pList->m_head->m_next;
}

Node* GetLast(const LinkList* pList)
{
/*
���ã��ú�������pList��ָ�����β���ĵ�ַ�� 
*/
	if(!pList || !pList->m_length) return NULL;
	
	return pList->m_tail;
}

Node* GetPrior(const Node* pElem)
{
/*
���ã��ú�����ȡ���pElem��ǰ������pElem����
	ָ�����ָ�룬��ᷢ�����벻���ĺ���� 
*/
	if(!pElem) return NULL;
	
	return pElem->m_prior;
}

Node* GetNext(const Node* pElem)
{
/*
���ã��ú�����ȡ���pElem�ĺ�̣���pElem����
	ָ�����ָ�룬��ᷢ�����벻���ĺ���� 
*/
	if(!pElem) return NULL;
	
	return pElem->m_next;
}

Node* Search(const LinkList* pList,
				ElemType* pCond,
				Status (*compare)(ElemType* pCondElem,ElemType* pCmpElem))
{
/*
���ã��ú������ݸ�����pCondָ�������������
	��pListָ��������в��ҵ�һ�����������Ľ�
	�㡣���ҹ�����compareָ��ĺ�������������
	������compare����DS_TRUE�����򷵻�DS_FALSE�� 
*/
	if(!(pList && pCond && compare)) return NULL;
	if(!pList->m_length)  return NULL;
	
	Node* p = pList->m_head->m_next;
	while(p)
	{
		if(compare(pCond,&(p->m_data))) return p;
		p = p->m_next;
	}
	
	return NULL;
}


Status InsertFirst(LinkList* pList,ElemType* pInElem)
{
/*
���ã��ú�������������pList��ͷ�������һ���½�㣬
	���������ΪpInElem��ָ��ṹ�����ݡ� 
*/
	if(!pList) return DS_FALSE;
	
	//�����½��Ŀռ�
	Node* p = (Node*)malloc(sizeof(Node));	 
	if(!p) return DS_OVERFLOW;
	
	if(pInElem) p->m_data = *pInElem;
	//��pInElemΪ�գ������������㡣			 
	else memset(&(p->m_data),0,sizeof(p->m_data));	
													 
	/*�������㣬����p�������㽨������; 
	�����½�����β��㣬���Ϊ��*/
	if(pList->m_length)
	{										
		p->m_next = pList->m_head->m_next;	 
		p->m_next->m_prior = p;
	}
	else 
	{
		p->m_next = NULL;					 
		pList->m_tail = p;
	}
	
	//��p��ͷ��㽨������
	pList->m_head->m_next = p;			 
	p->m_prior = pList->m_head;
	
	//������ 1
	pList->m_length++;		 
	
	return DS_OK;
}

Status InsertLast(LinkList* pList,ElemType* pInElem)
{
/*
���ã��ú�������������pList��β�������һ���½�㣬
	���������ΪpInElem��ָ��ṹ�����ݡ�
*/

	if(!pList) return DS_FALSE;
	
	Node* p = (Node*)malloc(sizeof(Node));	 
	if(!p) return DS_OVERFLOW;
	
	if(pInElem) p->m_data = *pInElem;
	//��pInElemΪ�գ������������㡣			 
	else memset(&(p->m_data),0,sizeof(p->m_data));
	
	p->m_next = NULL;
	p->m_prior = pList->m_tail;
	pList->m_tail->m_next = p;
	pList->m_tail = p;
	
	pList->m_length++;

	return DS_OK;
}

Status InsertBefore(LinkList* pList,int i,ElemType* pInElem)
{
/*
���ã���pList��ָ������ĵ�i�����ǰ�����½�㣬��
	pInElem��ָ��ĽṹΪ�½�������������i����
	���㣺1��i ��pList->m_length�� 
*/
	Node* pos = Locate(pList,i);
	if(!pos) return DS_FALSE;
	
	Node* p = (Node*)malloc(sizeof(Node));
	if(!p) return DS_OVERFLOW;
	
	/*��pInElemΪ�գ����½������������*/
	if(pInElem) p->m_data = *pInElem;
	else memset(&(p->m_data),0,sizeof(p->m_data));
	
	/*���½�����ԭ��i�����ǰ*/ 
	p->m_next = pos;
	p->m_prior = pos->m_prior;
	pos->m_prior = p;
	p->m_prior->m_next = p;
	
	//�ǵ�Ҫ���� 1 ������������ 
	pList->m_length++;

	return DS_OK;
}

Status InsertAfter(LinkList* pList,int i,ElemType* pInElem)
{
/*
���ã���pList��ָ������ĵ�i�����ǰ�����½�㣬��
	pInElem��ָ��ĽṹΪ�½�������������i����
	���㣺1��i ��pList->m_length�� 
*/
	Node* pos = Locate(pList,i);
	if(!pos) return DS_FALSE;
	
	Node* p = (Node*)malloc(sizeof(Node));
	if(!p) return DS_OVERFLOW;
	
	/*��pInElemΪ�գ����½������������*/
	if(pInElem) p->m_data = *pInElem;
	else memset(&(p->m_data),0,sizeof(p->m_data));
	
	/*���½����뵽��i������*/ 
	p->m_next = pos->m_next;
	p->m_prior = pos;
	pos->m_next = p;
	/*��p����β��㣬�����̵�ǰ��ָ��p;����
	p��β��㣬�轫����βָ��ָ��p*/
	if(p->m_next)	p->m_next->m_prior = p;
	else pList->m_tail = p;
	
	pList->m_length++;
	
	return DS_OK;
}


Status Delete(LinkList* pList,Node* pDelElem)
{
/*
���ã��ú���ɾ��pList��ָ�����е�pDelElemָ���
	��㡣��pDelElem��ָ��㲻������Ľ�㣬���
	�������벻��������� 
*/
	if(!(pList && pDelElem && pList->m_length)) return DS_FALSE;
	
	/*ͷ��㲻����ɾ��*/
	if(pDelElem == pList->m_head) 
		return DS_FALSE;
	/*ɾ����һ�����*/
	if(pDelElem == pList->m_head->m_next) 
		return DeleteFirst(pList,NULL);
	/*ɾ��β���*/
	if(pDelElem == pList->m_tail) 
		return DeleteLast(pList,NULL);
	/*һ�����ɾ��*/
	pDelElem->m_prior->m_next = pDelElem->m_next;
	pDelElem->m_next->m_prior = pDelElem->m_prior;
	pDelElem->m_prior = pDelElem->m_next = NULL;
	free(pDelElem);
	pList->m_length--;
	
	return DS_OK;
}

Status DeleteFirst(LinkList* pList,ElemType* pOutElem)
{
/*
���ã��ú���ɾ��pListָ������ĵ�һ����㣬����
	pOutElemָ��Ľṹ���ظý��������� 
*/
	if(!pList || !pList->m_length) return DS_FALSE; 
	
	if(pOutElem) *pOutElem = pList->m_head->m_next->m_data;
	Node* p = pList->m_head->m_next;
	pList->m_head->m_next = p->m_next;
	/*��p�ĺ���ǿգ�֤��p��β��㣬���轫����β
	ָ��ָ��ͷ��㣻����һ���㴦��*/
	if(p->m_next) p->m_next->m_prior = pList->m_head;
	else pList->m_tail = pList->m_head;
	
	/*��p���벢�ͷ�*/ 
	p->m_prior = p->m_next = NULL;
	free(p);
	
	//���� 1 ����Ҫ�� 
	pList->m_length--;
	
	return DS_OK;
}

Status DeleteLast(LinkList* pList,ElemType* pOutElem)
{
/*
���ã���pList��ָ�������ǿձ���ɾ���������һ
	����㣬����pOutElemָ��Ľṹ�巵�ظý���
	�����������ȼ� 1��	 
*/
	if(!pList || !pList->m_length) return DS_FALSE;
	
	if(pOutElem) *pOutElem = pList->m_tail->m_data;
	Node* p = pList->m_tail;
	pList->m_tail = p->m_prior;		//��βָ��ָ��β����ǰһ�� 
	pList->m_tail->m_next = NULL;	//��β������ 
	p->m_prior = NULL;
	free(p);						//�ͷ�β��� 
	pList->m_length--;				//�����Լ� 1 
	
	return DS_OK;
}

Status DeleteByOrder(LinkList* pList,int i,ElemType* pOutElem)
{
/*
���ã��ú���ɾ��pList��ָ�����еĵ�i����㣬����
	pOutElem��ָ�Ľṹ���ظý��������� 
*/
	Node* p = Locate(pList,i);
	if(!p) return DS_FALSE;
	
	if(pOutElem) *pOutElem = p->m_data;
	Delete(pList,p);
	
	return DS_OK;
}
