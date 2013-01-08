#include "tools.h"

/***********************************************************
 *   Дека порожня?                                         *
 ***********************************************************/
int DequeIsEmpty(const struct CDeque *Deque)
{
	return (Deque->deque_head == Deque->deque_tail);
}

/***********************************************************
 *   Дека заповнена?                                       *
 ***********************************************************/
int DequeIsFull(const struct CDeque *Deque)
{
	return ((Deque->deque_head == Deque->deque_tail + 1)
		|| (Deque->deque_head == 0 && (Deque->deque_tail == Deque->deque_max)));
}

/***********************************************************
 *   Додати до хвоста деки                                 *
 ***********************************************************/
int AddToDequeTail(void *Item, struct CDeque *Deque)
{
	if (DequeIsFull(Deque))
		return 0;
	Deque->deque_data[Deque->deque_tail]=Item;
	++Deque->deque_tail;
	if (Deque->deque_tail == Deque->deque_max)
		Deque->deque_tail = 0;
	return 1;
}

/***********************************************************
 *   Взяти елемент з голови деки                           *
 ***********************************************************/
int GetFromDequeHead(void **Item, struct CDeque *Deque)
{
	if (DequeIsEmpty(Deque))
		return 0;

	*Item=Deque->deque_data[Deque->deque_head];

	Deque->deque_head++;
	if (Deque->deque_head == Deque->deque_max)
		Deque->deque_head = 0;

	return 1;
}

/***********************************************************
 *   Додати до голови деки                                 *
 ***********************************************************/
int AddToDequeHead(void *Item, struct CDeque *Deque)
{
	if (DequeIsFull(Deque))
		return 0;
	if (Deque->deque_head == 0)
		Deque->deque_head = Deque->deque_max;
	--Deque->deque_head;
	Deque->deque_data[Deque->deque_head]=Item;
	return 1;
}

/***********************************************************
 *   Взяти елемент з хвоста деки                           *
 ***********************************************************/
int GetFromDequeTail(void **Item, struct CDeque *Deque)
{
	if (DequeIsEmpty(Deque))
		return 0;

	if (Deque->deque_head == 0)
		Deque->deque_head = Deque->deque_max;
	--Deque->deque_head;

	*Item=Deque->deque_data[Deque->deque_tail];

	return 1;
}
