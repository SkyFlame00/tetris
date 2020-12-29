#ifndef LIST_H
#define LIST_H

template <class T>
struct ListItem
{
	T val;
	ListItem<T>* next;
};

template <class T>
class List
{
public:
	ListItem<T>* head;
	ListItem<T>* tail;
	int length;

	List();
	~List();

	void Push(T object);
	T* Remove(int pos);
	void ForEach(void (*fn)(ListItem<T> *item));
private:

};

#endif // !LIST_H