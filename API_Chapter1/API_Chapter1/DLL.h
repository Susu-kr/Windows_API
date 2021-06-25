#pragma once
#include <string>
#include <iostream>
#include "Object.h"

using namespace std;

class Node {
private:
	CObject * object;
	Node* next;
	Node* prev;
	friend class List;
};

class List {
	Node * head;
	Node * tail;
public:
	List();
	~List();
	CObject * OBINDEX(int num);
	bool empty() const { return head->next == tail; } // ����Ʈ�� ����ִ��� Ȯ��
	int ListSize();
	void insert(Node * v, CObject * o);
	void insertBack(CObject * o) { insert(tail->prev, o); }// �� �ڿ� ����
	void remove(Node * v);
	void removeFront() { remove(head->next); } // �� ���� ����
	void removeBack() { remove(tail->prev); }; // �� �ڸ� ����
	void removeIndex(int num); // �ش���ġ�� ��� ����
};
