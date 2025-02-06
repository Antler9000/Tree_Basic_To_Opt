#include <iostream>
#include "messy_tree.h"
using namespace std;



node::node(int key, int data) {
	cout << "node is made!" << endl;
	this->key = key;
	this->data = data;
	this->lchild = NULL;
	this->rchild = NULL;
}



tree::tree() {
	cout << "tree is made!" << endl;
}

tree::~tree() {
	remove_all();
}

void tree::traverse_print() {
	if (head == NULL) {
		cout << "cannot traverse. head is NULL." << endl;
		return;
	}
	cout << "node key : " << head->key << " / node data : " << head->data << endl;
	if (head->lchild != NULL) head->lchild->traverse_print();
	if (head->rchild != NULL) head->rchild->traverse_print();
}

void tree::remove_all() {
	if (head != NULL) {
		if (head->lchild != NULL) head->lchild->remove_all();
		if (head->rchild != NULL) head->rchild->remove_all();
		delete head;
		head = NULL;
	}
}

int tree::search(int target_key) {
	if (head == NULL) {
		cout << "can not search. there is no such key." << endl;
	}

	if (target_key < head->key) {
		if (head->lchild != NULL) return head->lchild->search(target_key);
	} 
	else if (head->key < target_key) {
		if (head->rchild != NULL) return head->rchild->search(target_key);
	}
	else {
		return head->data;
	}
}

void tree::insert(int new_key, int new_data) {
	if (head == NULL) {
		head = new node(new_key, new_data);
	}
	else {
		if (new_key < head->key) {
			if(head->lchild == NULL) head->lchild = new tree();
			head->lchild->insert(new_key, new_data);
		}
		else if(head->key < new_key) {
			if (head->rchild == NULL) head->rchild = new tree();
			head->rchild->insert(new_key, new_data);
		}
		else {
			cout << "cannot insert! key is same!" << endl;
		}
	}
}

void tree::remove(int target_key) {
	if (head == NULL) {
		cout << "cannot remove! tree is emptied!" << endl;
		return;
	}

			
	if (target_key < head->key) {
		if (head->lchild->head->lchild == NULL && head->lchild->head->rchild == NULL) {	
			delete head->lchild;
			head->lchild = NULL;
		}
		else {
			head->lchild->remove(target_key);
		}
	}
	else if (head->key < target_key) {
		if (head->rchild->head->lchild == NULL && head->rchild->head->rchild == NULL) {
			delete head->rchild;
			head->rchild = NULL;
		}
		else {
			head->rchild->remove(target_key);
		}
	}
	else {
		if (head->lchild != NULL && head->rchild != NULL) {			//두 자식 모두 있는 경우엔, 중위선행자와 중위후속자 중에서 그냥 중위후속자(오른쪽 자식 트리에서 제일 작은 키 값의 노드)를 없애기로함
			replace_with_inorder_successor();
		}
		else if (head->lchild == NULL && head->rchild != NULL) {
			replace_with_inorder_successor();
		}
		else if (head->lchild != NULL && head->rchild == NULL) {
			replace_with_inorder_predecessor();
		}
		else {
			delete head;
			head = NULL;
		}
	}
}

void tree::replace_with_inorder_predecessor() {
	tree* previous_ptr = NULL;
	tree* traverse_ptr = head->lchild;
	while (traverse_ptr->head->rchild != NULL) {
		previous_ptr = traverse_ptr;
		traverse_ptr = traverse_ptr->head->rchild;
	}
	if (previous_ptr != NULL) previous_ptr->head->rchild = traverse_ptr->head->lchild;		//삭제대상의 자리를 매꾸러갈 중위선행자가 가지고 있을 수 있는 왼쪽 자식을 중위선행자의 부모에게 맡겨야함
	else this->head->lchild = traverse_ptr->head->lchild;									//근데 삭제대상의 왼쪽자식이 바로 중위선행자인 경우에는 중위선행자에게 맡기는 것은 같으나 오른쪽 자식이 아닌 왼쪽 자식으로 맡겨야함.
	head->key = traverse_ptr->head->key;
	head->data = traverse_ptr->head->data;
	delete traverse_ptr;
}

void tree::replace_with_inorder_successor() {
	tree* previous_ptr = NULL;
	tree* traverse_ptr = head->rchild;
	while (traverse_ptr->head->lchild != NULL) {
		previous_ptr = traverse_ptr;
		traverse_ptr = traverse_ptr->head->lchild;
	}
	if (previous_ptr != NULL) previous_ptr->head->lchild = traverse_ptr->head->rchild;
	else this->head->rchild = traverse_ptr->head->rchild;
	head->key = traverse_ptr->head->key;
	head->data = traverse_ptr->head->data;
	delete traverse_ptr;
}
