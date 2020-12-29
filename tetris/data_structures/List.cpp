#include "List.h"

#include "../MenuObject.h"

template <class T>
List<T>::List() : head(nullptr), tail(nullptr), length(0)
{
}

template <class T>
List<T>::~List()
{
	ForEach([](ListItem<T> *item) -> void
		{
			delete item;
		}
	);
}

template <class T>
void List<T>::Push(T object)
{
	ListItem<T>* item = new ListItem<T>{ object, nullptr };
	length += 1;

	if (head == nullptr && tail == nullptr)
	{
		head = item;
		tail = item;
		return;
	}

	if (head == tail)
	{
		tail = item;
		head->next = tail;
		return;
	}

	tail->next = item;
	tail = tail->next;
}

template <class T>
void List<T>::ForEach(void (*fn)(ListItem<T>*))
{
	ListItem<T> *item, *next;
	for (item = head; item; item = next)
	{
		next = item->next;
		fn(item);
	}
}

template class List<MenuObject*>;
template class List<Button*>;
template class List<void (*)(GLFWwindow*)>;