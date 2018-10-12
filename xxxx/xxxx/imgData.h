#pragma once
#include <assert.h>
//像素结构体
struct imgNode {
	int x;
	int y;
	double cost;//到seed成本
	bool visit;//访问标记
	bool inList;//是否在链表中
	imgNode * back_pointer;//最短路径的方向
	imgNode * next;//用于链表连接
};

//像素链表
class PathList {
public:
	int size;
	PathList() { assert(head == nullptr); size = 0; }
	imgNode * head;
	imgNode * now;//用于插入
	void remove(imgNode * p)//从链表取出并删除相应像素点
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
	void insert(imgNode *p)//插入链表，并按成本从小到大排列
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