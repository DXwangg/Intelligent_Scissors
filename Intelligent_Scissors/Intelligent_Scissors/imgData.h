#pragma once
#include <assert.h>
//���ؽṹ��
struct imgNode {
	int x;
	int y;
	double cost;//��seed�ɱ�
	bool visit;//���ʱ��
	bool inList;//�Ƿ���������
	imgNode * back_pointer;//���·���ķ���
	imgNode * next;//������������
};

//��������
class PathList {
public:
	int size;
	PathList() { assert(head == nullptr); size = 0; }
	imgNode * head;
	imgNode * now;//���ڲ���
	void remove(imgNode * p)//������ȡ����ɾ����Ӧ���ص�
{
		if (isEmpty()){
			return;
		}
		now = head;

		if (head == p) {
			now = now->next;
			head = now;
			size--;
			return;
		}
		while (now->next != nullptr) {
			if (now->next == p) {
				now->next = now->next->next;
				size--;
				return;
			}
			now = now->next;
		}
	}
	void insert(imgNode *p)//�������������ɱ���С��������
	{
		if (head == nullptr) {
			head = p;
			size++;
			return;
		}
		now = head;
		while (now->next != nullptr) {
			if (p->cost < now->next->cost)
			{
				p->next = now->next;
				now->next = p;
				size++;
				return;
			}
			now = now->next;
		}
		now->next = p;
		size++;
	}
	bool isEmpty()
	{
		if (size == 0)
			return true;
		return false;
	}

};