#pragma once
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class Stack {
private:
	vector<T> vector;
public:
	Stack& operator = (const Stack& smth) {
		vector = smth.vector;
	}
	void Push(const T& data) {
		vector.push_back(data);
	}
	T Pop() {
		if (isEmpty()) throw "Stack is empty";
		T tmp = vector.back();
		vector.pop_back();
		return tmp;
	}
	bool const isEmpty() {
		return vector.empty();
	}
	T Top() {
		if (isEmpty()) throw "Stack is empty";
		return vector.back();
	}
	size_t const size() {
		return vector.size();
	}
	bool const isFull() {
		return vector.size() == 255 ? true : false;
	}
};