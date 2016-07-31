#include "stdafx.h"

#ifndef __LIST__
#define __LIST__
template <class T> 
class List
{
private:
	T dataItem;
	List<T> *next;
	bool IsInitialized;
public:
	__declspec(dllexport) List();
    __declspec(dllexport) List(T item);
	__declspec(dllexport) void Add(T item);
	__declspec(dllexport) int Count();
	__declspec(dllexport) List<T> *GetNext();
	__declspec(dllexport) List<T> *GetLast();
	__declspec(dllexport) T GetItem();
	__declspec(dllexport) void TerminateList();
	__declspec(dllexport) void EmptyList();
protected:
};

template <class T>
List<T>::List()
{
	IsInitialized = false;
	this->next = NULL;
}


template <class T>
List<T>::List(T item)
{
	IsInitialized = true;
	this->dataItem = item;
	this->next = NULL;
}

template <class T>
void List<T>::Add(T item)
{
	if (IsInitialized == false)
	{
		IsInitialized = true;
		this->dataItem = item;
		this->next = NULL;
		return;
	}

	List<T> *newNode = new List<T>(item);
	// find current end;
	List<T> *lastNode = this;

	while(lastNode->next)
	{
		lastNode = lastNode->next;
	}

	lastNode->next = newNode;
}

template <class T>
int List<T>::Count()
{
	if (IsInitialized == false)
	{
		return 0;
	}

	int count = 1;
	List<T> *lastNode = this;

	while(lastNode->next)
	{
		count++;
		lastNode = lastNode->next;
	}

	return count;
}

template <class T>
List<T> *List<T>::GetNext()
{
	return this->next;
}

template <class T>
List<T> *List<T>::GetLast()
{
	List<T> *item = this;

	while(item->next)
	{
		item = item->next;
	}

	return item;
}

template <class T>
T List<T>::GetItem()
{
	return this->dataItem;
}

template <class T>
void List<T>::TerminateList()
{
	List<T> *chainAhead = this->next;
	this->next = NULL;

	// delete all nodes from chainAhead;
	while(chainAhead)
	{
		List<T> *ptrToDeleteNodeAt = chainAhead;
		chainAhead = chainAhead->next;

		delete ptrToDeleteNodeAt;
	}
}

template <class T>
void List<T>::EmptyList()
{
	this->TerminateList();
	this->IsInitialized = false;
}


#endif