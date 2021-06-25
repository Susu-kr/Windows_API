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
	bool empty() const { return head->next == tail; } // 리스트가 비어있는지 확인
	int ListSize();
	void insert(Node * v, CObject * o);
	void insertBack(CObject * o) { insert(tail->prev, o); }// 맨 뒤에 삽입
	void remove(Node * v);
	void removeFront() { remove(head->next); } // 맨 앞을 제거
	void removeBack() { remove(tail->prev); }; // 맨 뒤를 제거
	void removeIndex(int num); // 해당위치의 노드 삭제
};
