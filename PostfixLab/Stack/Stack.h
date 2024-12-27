#pragma once
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class Stack {
private:
	T* elems;
	int MaxSize;
	int top;
public:
	Stack(int MaxSize = 255);
	~Stack();
	bool isEmpty()const;
	int size();
	bool isFull()const;
	T Top()const;
	void Push(const T& elem);
	T Pop();
};
template <class T>
int Stack<T>::size() {
	return top+1;
}
template <class T>
Stack<T>::Stack(int MaxSize)
{
	if (MaxSize < 0)
		throw std::exception("invalid size");
	top = -1;
	this->MaxSize = MaxSize;
	elems = new T[MaxSize];
}


template <class T>
Stack<T>::~Stack()
{
	if (elems != nullptr)
		delete[] elems;
}

template <class T>
bool Stack<T>::isEmpty()const
{
	return top == -1;
}

template <class T>
bool Stack<T>::isFull()const
{
	return top + 1 == MaxSize;
}

template <class T>
T Stack<T>::Top()const
{
	if (top == -1)
		throw std::exception("stack is empty");
	return elems[top];
}

template <class T>
void Stack<T>::Push(const T& elem)
{
	if (top + 1 == MaxSize)
		throw std::exception("stack is full");
	elems[++top] = elem;
}

template <class T>
T Stack<T>::Pop()
{
	if (top-- == -1)
		throw std::exception("stack is empty");
	return elems[top + 1];
}