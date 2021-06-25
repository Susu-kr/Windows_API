#include "DLL.h"
List::List() {
	head = new Node;
	tail = new Node;
	head->next = tail;
	tail->prev = head;
}

List::~List() {
	while (!empty()) removeFront();
	delete head;
	delete tail;
}

CObject * List::OBINDEX(int num)
{
	Node * cur = head->next;
	for (int i = 0; i < ListSize(); i++) {
		if (i == num) {
			return cur->object;
		}
		cur = cur->next;
	}
}

int List::ListSize()
{
	int cnt = 0;
	Node * cur = head;
	while (cur->next != tail) {
		cur = cur->next;
		cnt++;
	}
	return cnt;
}

void List::insert(Node * v, CObject * o)
{
	Node * t = new Node;
	t->object = o;
	t->next = v->next;
	t->prev = v;
	v->next->prev = t;
	v->next = t;
}

void List::remove(Node * v)
{
	Node * p = v->prev;
	Node * n = v->next;
	p->next = n;
	n->prev = p;
	delete v;
}

void List::removeIndex(int num)
{
	if (head->next == tail) cout << "리스트가 비어있으므로 삭제 실패.\n";
	else {
		if (ListSize() < num) {
			//cout << "삭제하고자 하는 위치가 리스트 사이즈보다 크므로 맨뒤에 삭제\n";
			removeBack();
		}
		else if (num <= 0) {
			//cout << "삭제하고자 하는 위치가 0이거나 작으면 맨 앞 삭제\n";
			removeFront();
		}
		else {
			Node * cur = head->next;
			for (int i = 0; i < num; i++) {
				cur = cur->next;
			}
			remove(cur);
		}
	}
}
