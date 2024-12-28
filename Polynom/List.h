#pragma once

template <class T>
class List {
public:
	class Iterator {
	private:
		List<T>* list;
		typename List<T>::Node* current;
	public:
		Iterator(List<T>* list, typename List<T>::Node* current) : list(list), current(current) {}
		bool operator!=(const Iterator& other) const {
			return current != other.current;
		}
		Iterator& operator++() {
			if (current) {
				current = current->next;
			}
			return *this;
		}
		T& operator*() {
			return current->data;
		}
		T* get() {
			if (current) {
				return &current->data;
			}
			else {
				return nullptr;
			}
		}
		Iterator getNext() const {
			if (current) {
				return Iterator(list, current->next);
			}
			else {
				return Iterator(nullptr, nullptr);
			}
		}
	};
	class ConstIterator {
	private:
		const List<T>* list;
		typename List<T>::Node* current;

	public:
		ConstIterator(const List<T>* list, typename List<T>::Node* current) : list(list), current(current) {}

		bool operator!=(const ConstIterator& other) const {
			return current != other.current;
		}

		ConstIterator& operator++() {
			if (current) {
				current = current->next;
			}
			return *this;
		}

		const T& operator*() const {
			return current->data;
		}

		const T* get() const {
			if (current) {
				return &current->data;
			}
			else {
				return nullptr;
			}
		}
		ConstIterator getNext() const {
			if (current) {
				return ConstIterator(list, current->next);
			}
			return ConstIterator(nullptr, nullptr);

		}
	};
	struct Node {
		T data;
		Node* next;
		Node(const T& data) : data(data), next(nullptr){}
	};
	Node* head;
	Node* current;
public:
	List() : head(nullptr), current(nullptr){}
	virtual ~List() {
		Node* temp = head;
		while (temp) {
			Node* next = temp->next;
			delete temp;
			temp = next;
		}
	}
	Iterator begin() {
		return Iterator(this, head);
	}
	ConstIterator begin() const {
		return ConstIterator(this, head);
	}
	Iterator end() {
		return Iterator(this, nullptr);
	}
	ConstIterator end() const {
		return ConstIterator(this, nullptr);
	}
	void insertFront(const T& data) {
		Node* newNode = new Node(data);
		newNode->next = head;
		head = newNode;
		if (!current) {
			current = head;
		}
	}
	void insertAfterCurrent(const T& data) {
		if (!current) {
			insertFront(data);
			return;
		}

		Node* newNode = new Node(data);
		newNode->next = current->next;
		current->next = newNode;

		if (!newNode->next) {
			current = newNode;
		}
	}
		void insertEnd(const T & data) {
			Node* newNode = new Node(data);
			if (!head) {
				head = newNode;
				if (!current)
					current = head;
				return;
			}
			Node* temp = head;
			while (temp->next) {
				temp = temp->next;
			}
			if (!current) {
				current = temp;
			}
			temp->next = newNode;
		}
		void removeFront() {
			if (!head) return;
			Node* temp = head;
			head = head->next;
			if (current == temp) {
				current = head;
			}
			delete temp;
		}

		void removeCurrent() {
			if (!head || !current) return;
			if (current == head) {
				removeFront();
				return;
			}
			Node* temp = head;
			while (temp->next != current) {
				temp = temp->next;
			}
			temp->next = current->next;


			if (current->next == nullptr) {
				current = temp;
			}
			else {
				current = current->next;
			}

			delete temp->next;
		}
		void setCurrentToHead() {
			current = head;
		}
		T* getCurrent() {
			if (current) {
				return &current->data;
			}
			else {
				return nullptr;
			}
		}
		bool isEmpty() const {
			return head == nullptr;
		}
};