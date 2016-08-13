#include<stdlib.h>
#include<string.h>
#include"DS_LinkList.h"

Status InitList(LinkList* pList)
{
/*
作用：该函数用来初始化pList所指向的链表结构，包括
	分配头结点、初始化尾指针和长度。 
*/
	if(!pList) return DS_FALSE;
	
	/*因为有些编译器不会对未初始化的变量赋0，所以
	可能会导致下面语句的执行出现异常。于是此判断用
	来检链表是否被初始化过，若被初始化过，则按照一
	般的初始化步骤进行；若未初始化过，则未初始化的
	值需要进行特殊处理。
	*/
	if(pList->haveUsed ^ 0xFFFF)	//未初始化过 
	{											
		pList->m_head = pList->m_tail = NULL;	 
		pList->m_length = 0;					 
	}											
	/*若初始化过，且pList不是一个空表，则先销毁。这样能防止同
	表复用带来的内存泄露问题。*/											 																								 
	else if(pList->m_head && !DestroyList(pList)) return DS_FALSE;	
																
	//分配头结点,若分配失败则返回																																															 	
	pList->m_head = (Node*)malloc(sizeof(Node));	 
	if(!pList->m_head) return DS_OVERFLOW;				 
	pList->m_head->m_prior = pList->m_head->m_next = NULL;
	//初始状态尾指针等于头指针 
	pList->m_tail = pList->m_head;		
	//初始状态表长为0，不计头结点			
	pList->m_length = 0;							 
	
	//此步说明该链表已被初始化过。
	pList->haveUsed = 0xFFFF;		 
	
	return DS_OK;
}

Status DestroyList(LinkList* pList)
{
/*
作用：该函数用来销毁pList所指向的链表结构，即
	释放掉所有结点，包括头结点。 
*/
	//释放所有非头结点
	if(!ClearList(pList)) return DS_FALSE;		 
	
	//释放头结点
	if(pList->m_head) free(pList->m_head);	 
	pList->m_head = pList->m_tail = NULL;
	pList->m_length = 0;
	
	return DS_OK;
}

Status ClearList(LinkList* pList)
{
/*
作用：该函数用来清理pList所指向的链表结构，但
	不包括头结点。 
*/
	if(!pList) return DS_FALSE;
	
	/*若表长度非0，则进入清理循环。删除结点从
	末尾开始。*/
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
作用：判断pList所指向的链表是否为空表，若是返
	回DS_TRUE，否则返回DS_FALSE。 
*/
	if(!pList) return DS_ERROR;
	if(pList->m_length) return DS_FALSE;
	return DS_TRUE;
}

int GetLength(LinkList* pList)
{
/*
作用：返回pList所指向的链表的长度。 
*/
	if(!pList) return DS_ERROR;
	return pList->m_length;
}

void TraverseList(LinkList* pList,void (*visit)(Node* pElem))
{
/*
作用：该函数用来遍历pList所指向链表的每个结点，
	遍历方式视函数指针visit而定。 
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
作用：该函数返回pList所指向链表的第i个结点的地址。
	其中i必须满足1≤i ≤pList->m_length。 
*/
	if(!pList) return NULL;
	if(i < 1 || i > pList->m_length) return NULL;
	
	/*以下三行代码寻找第i个结点的地址*/
	int order = 1;
	Node* pos = pList->m_head->m_next;
	for(;order != i; order++,pos = pos->m_next);
	
	return pos;
}

Node* GetFirst(const LinkList* pList)
{
/*
作用：该函数返回pList所指链表的第一个结点的地址。 
*/
	if(!pList || !pList->m_length) return NULL;
	
	return pList->m_head->m_next;
}

Node* GetLast(const LinkList* pList)
{
/*
作用：该函数返回pList所指链表的尾结点的地址。 
*/
	if(!pList || !pList->m_length) return NULL;
	
	return pList->m_tail;
}

Node* GetPrior(const Node* pElem)
{
/*
作用：该函数获取结点pElem的前驱，若pElem不是
	指向结点的指针，则会发生意想不到的后果。 
*/
	if(!pElem) return NULL;
	
	return pElem->m_prior;
}

Node* GetNext(const Node* pElem)
{
/*
作用：该函数获取结点pElem的后继，若pElem不是
	指向结点的指针，则会发生意想不到的后果。 
*/
	if(!pElem) return NULL;
	
	return pElem->m_next;
}

Node* Search(const LinkList* pList,
				ElemType* pCond,
				Status (*compare)(ElemType* pCondElem,ElemType* pCmpElem))
{
/*
作用：该函数根据给出的pCond指向的条件数据域，
	在pList指向的链表中查找第一个符合条件的结
	点。查找规则由compare指向的函数而定。符合
	条件则compare返回DS_TRUE，否则返回DS_FALSE。 
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
作用：该函数用来在链表pList的头结点后插入一个新结点，
	结点数据域为pInElem所指向结构的内容。 
*/
	if(!pList) return DS_FALSE;
	
	//分配新结点的空间
	Node* p = (Node*)malloc(sizeof(Node));	 
	if(!p) return DS_OVERFLOW;
	
	if(pInElem) p->m_data = *pInElem;
	//若pInElem为空，则将数据域清零。			 
	else memset(&(p->m_data),0,sizeof(p->m_data));	
													 
	/*若表长非零，则让p与后续结点建立连接; 
	否则，新结点就是尾结点，后继为空*/
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
	
	//让p与头结点建立连接
	pList->m_head->m_next = p;			 
	p->m_prior = pList->m_head;
	
	//表长自增 1
	pList->m_length++;		 
	
	return DS_OK;
}

Status InsertLast(LinkList* pList,ElemType* pInElem)
{
/*
作用：该函数用来在链表pList的尾结点后插入一个新结点，
	结点数据域为pInElem所指向结构的内容。
*/

	if(!pList) return DS_FALSE;
	
	Node* p = (Node*)malloc(sizeof(Node));	 
	if(!p) return DS_OVERFLOW;
	
	if(pInElem) p->m_data = *pInElem;
	//若pInElem为空，则将数据域清零。			 
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
作用：在pList所指向链表的第i个结点前插入新结点，以
	pInElem所指向的结构为新结点的数据域。其中i必须
	满足：1≤i ≤pList->m_length。 
*/
	Node* pos = Locate(pList,i);
	if(!pos) return DS_FALSE;
	
	Node* p = (Node*)malloc(sizeof(Node));
	if(!p) return DS_OVERFLOW;
	
	/*若pInElem为空，则将新结点数据域清零*/
	if(pInElem) p->m_data = *pInElem;
	else memset(&(p->m_data),0,sizeof(p->m_data));
	
	/*将新结点插入原第i个结点前*/ 
	p->m_next = pos;
	p->m_prior = pos->m_prior;
	pos->m_prior = p;
	p->m_prior->m_next = p;
	
	//记得要表长增 1 啊！！！！！ 
	pList->m_length++;

	return DS_OK;
}

Status InsertAfter(LinkList* pList,int i,ElemType* pInElem)
{
/*
作用：在pList所指向链表的第i个结点前插入新结点，以
	pInElem所指向的结构为新结点的数据域。其中i必须
	满足：1≤i ≤pList->m_length。 
*/
	Node* pos = Locate(pList,i);
	if(!pos) return DS_FALSE;
	
	Node* p = (Node*)malloc(sizeof(Node));
	if(!p) return DS_OVERFLOW;
	
	/*若pInElem为空，则将新结点数据域清零*/
	if(pInElem) p->m_data = *pInElem;
	else memset(&(p->m_data),0,sizeof(p->m_data));
	
	/*将新结点插入到第i个结点后*/ 
	p->m_next = pos->m_next;
	p->m_prior = pos;
	pos->m_next = p;
	/*若p不是尾结点，则将其后继的前驱指向p;否则，
	p是尾结点，需将链表尾指针指向p*/
	if(p->m_next)	p->m_next->m_prior = p;
	else pList->m_tail = p;
	
	pList->m_length++;
	
	return DS_OK;
}


Status Delete(LinkList* pList,Node* pDelElem)
{
/*
作用：该函数删除pList所指链表中的pDelElem指向的
	结点。若pDelElem所指结点不是链表的结点，则会
	发生意想不到的情况。 
*/
	if(!(pList && pDelElem && pList->m_length)) return DS_FALSE;
	
	/*头结点不允许删除*/
	if(pDelElem == pList->m_head) 
		return DS_FALSE;
	/*删除第一个结点*/
	if(pDelElem == pList->m_head->m_next) 
		return DeleteFirst(pList,NULL);
	/*删除尾结点*/
	if(pDelElem == pList->m_tail) 
		return DeleteLast(pList,NULL);
	/*一般结点的删除*/
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
作用：该函数删除pList指向链表的第一个结点，并用
	pOutElem指向的结构返回该结点的数据域。 
*/
	if(!pList || !pList->m_length) return DS_FALSE; 
	
	if(pOutElem) *pOutElem = pList->m_head->m_next->m_data;
	Node* p = pList->m_head->m_next;
	pList->m_head->m_next = p->m_next;
	/*若p的后继是空，证明p是尾结点，则需将链表尾
	指针指向头结点；否则按一般结点处理。*/
	if(p->m_next) p->m_next->m_prior = pList->m_head;
	else pList->m_tail = pList->m_head;
	
	/*将p隔离并释放*/ 
	p->m_prior = p->m_next = NULL;
	free(p);
	
	//表长减 1 很重要！ 
	pList->m_length--;
	
	return DS_OK;
}

Status DeleteLast(LinkList* pList,ElemType* pOutElem)
{
/*
作用：若pList所指向的链表非空表，则删除表中最后一
	个结点，并用pOutElem指向的结构体返回该结点的
	数据域，链表长度减 1。	 
*/
	if(!pList || !pList->m_length) return DS_FALSE;
	
	if(pOutElem) *pOutElem = pList->m_tail->m_data;
	Node* p = pList->m_tail;
	pList->m_tail = p->m_prior;		//将尾指针指向尾结点的前一个 
	pList->m_tail->m_next = NULL;	//将尾结点隔离 
	p->m_prior = NULL;
	free(p);						//释放尾结点 
	pList->m_length--;				//长度自减 1 
	
	return DS_OK;
}

Status DeleteByOrder(LinkList* pList,int i,ElemType* pOutElem)
{
/*
作用：该函数删除pList所指链表中的第i个结点，并用
	pOutElem所指的结构返回该结点的数据域。 
*/
	Node* p = Locate(pList,i);
	if(!p) return DS_FALSE;
	
	if(pOutElem) *pOutElem = p->m_data;
	Delete(pList,p);
	
	return DS_OK;
}
