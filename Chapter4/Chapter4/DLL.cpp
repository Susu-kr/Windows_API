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
	if (head->next == tail) cout << "����Ʈ�� ��������Ƿ� ���� ����.\n";
	else {
		if (ListSize() < num) {
			//cout << "�����ϰ��� �ϴ� ��ġ�� ����Ʈ ������� ũ�Ƿ� �ǵڿ� ����\n";
			removeBack();
		}
		else if (num <= 0) {
			//cout << "�����ϰ��� �ϴ� ��ġ�� 0�̰ų� ������ �� �� ����\n";
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
