#include<iostream>
using namespace std;

class node {
	friend class tree;
	int key;
	int data;
	node* lchild;
	node* rchild;


public:
	node(int key, int data) {
		this->key = key;
		this->data = data;
		this->lchild = NULL;
		this->rchild = NULL;
	}
};

//�ϴ� ������ ������ �������� Ÿ���� node*�̰�, �ִ� ������ 100����� �����ϰ� ��� ������ �����.
//������ ������ ������� ������ �޸� ũ�⸦ ����ϵ���, ���Ŀ� ����� ��� ������ ������ ����
class stack {
	node* head_data[100];
	int item_num;

public:
	stack() {
		for (int i = 0; i < 100; i++) {
			head_data[i] = NULL;
		}
		item_num = 0;
	}

	void push(node* new_head) {
		if (item_num >= 99) {
			cout << "cannot push to stack. it is full!" << endl;
			return;
		}
		head_data[item_num] = new_head;
		item_num++;
	}

	node* pop() {
		if (item_num <= 0) {
			cout << "cannot pop from stack. it is emptied!" << endl;
			return NULL;
		}

		return head_data[--item_num];
	}

	node* get_top() {
		if (item_num <= 0) {
			cout << "cannot get top from stack. it is emptied!" << endl;
			return NULL;
		}

		return head_data[item_num-1];
	}
};

class tree {
	node* head;

public:
	tree() {
		cout << "tree is made!" << endl;
		head = NULL;
	}

	~tree() {
		remove_all();
	}

	void preorder_traverse_print() {
		stack head_stack;
		node* traverse_ptr = NULL;
		head_stack.push(this->head);
		while ((traverse_ptr = head_stack.pop())) {
			cout << "node key : " << traverse_ptr->key << " / node data : " << traverse_ptr->data << endl;
			if(traverse_ptr->rchild != NULL) head_stack.push(traverse_ptr->rchild);
			if(traverse_ptr->lchild != NULL) head_stack.push(traverse_ptr->lchild);
		}
	}

	void inorder_traverse_print() {
		stack head_stack;
		node* traverse_ptr = NULL;
		bool is_popped = false;
		head_stack.push(this->head);
		while (traverse_ptr = head_stack.get_top()) {
			if (traverse_ptr->lchild != NULL && is_popped == false) {
				head_stack.push(traverse_ptr->lchild);
				is_popped = false;
			}
			else if (traverse_ptr->rchild != NULL) {
				cout << "node key : " << traverse_ptr->key << " / node data : " << traverse_ptr->data << endl;
				head_stack.push(traverse_ptr->rchild);
				is_popped = false;
			}
			else {
				cout << "node key : " << traverse_ptr->key << " / node data : " << traverse_ptr->data << endl;
				int key_whoese_popped_from = head_stack.pop()->key;
				traverse_ptr = head_stack.pop();
				if (traverse_ptr == NULL) break;
				while (!(traverse_ptr->lchild != NULL && traverse_ptr->lchild->key == key_whoese_popped_from)) {
					key_whoese_popped_from = traverse_ptr->key;
					traverse_ptr = head_stack.pop();
					if (traverse_ptr == NULL) break;
				}	//"������ �ڽ��� ���� ���ٸ� ��� -> ������ȸ�� ���� ���� ���"�� �̵��Ѱ���
				is_popped = true;
				head_stack.push(traverse_ptr);
			}
		}
	}

	void postorder_traverse_print() {
		stack head_stack;
		node* traverse_ptr = NULL;
		bool is_popped = false;
		head_stack.push(this->head);
		while (traverse_ptr = head_stack.get_top()) {
			if (traverse_ptr->lchild != NULL && is_popped == false) {
				head_stack.push(traverse_ptr->lchild);
				is_popped = false;
			}
			else if (traverse_ptr->rchild != NULL) {
				head_stack.push(traverse_ptr->rchild);
				is_popped = false;
			}
			else {
				cout << "node key : " << traverse_ptr->key << " / node data : " << traverse_ptr->data << endl;
				int key_whoese_popped_from = head_stack.pop()->key;
				traverse_ptr = head_stack.pop();
				if (traverse_ptr == NULL) break;
				while (!(traverse_ptr->lchild != NULL && traverse_ptr->lchild->key == key_whoese_popped_from)) {
					cout << "node key : " << traverse_ptr->key << " / node data : " << traverse_ptr->data << endl;
					key_whoese_popped_from = traverse_ptr->key;
					traverse_ptr = head_stack.pop();
					if (traverse_ptr == NULL) break;
				}	//"������ �ڽ��� ���� ���ٸ� ��� -> ������ȸ�� ���� ���� ���"�� �̵��Ѱ���
				is_popped = true;
				head_stack.push(traverse_ptr);
			}
		}
	}


	int search(int target_key) {
		if (head == NULL) {
			cout << "can not search. there is no such key." << endl;
			return -1;
		}

		node* search_ptr = head;
		while (search_ptr->key != target_key) {
			if (target_key < search_ptr->key) {
				if (search_ptr->lchild != NULL) search_ptr = search_ptr->lchild;
				else  cout << "there is no such key in searching." << endl;
			}
			else if (search_ptr->key < target_key) {
				if (search_ptr->rchild != NULL) search_ptr = search_ptr->rchild;
				else  cout << "there is no such key in searching." << endl;
			}
			else {
				return search_ptr->data;
			}
		}
	}


	void insert(int new_key, int new_data) {
		if (head == NULL) {
			head = new node(new_key, new_data);
		}
		else {
			node* search_ptr = head;
			while (true) {
				if (new_key < search_ptr->key) {
					if (search_ptr->lchild != NULL) search_ptr = search_ptr->lchild;
					else {
						search_ptr->lchild = new node(new_key, new_data);
						return;
					}
				}
				else if (search_ptr->key < new_key) {
					if (search_ptr->rchild != NULL) search_ptr = search_ptr->rchild;
					else {
						search_ptr->rchild = new node(new_key, new_data);
						return;
					}
				}
			}
		}
	}

	void remove_all() {
		
	}

	void remove(int target_key) {
		if (head == NULL) {
			cout << " cannot remove node. there is no node!" << endl;
			return;
		}

		if (head->key == target_key) {
			remove_target(head);
		}
		else {
			node* search_ptr = head;
			while (true) {
				if (target_key < search_ptr->key) {
					if (search_ptr->lchild != NULL) {
						if (search_ptr->lchild->key == target_key) {
							remove_target(search_ptr->lchild);
							return;
						}
						else search_ptr = search_ptr->lchild;
					}
					else {
						cout << "cannot remove node. there is no such node!" << endl;
						return;
					}
				}
				else if (search_ptr->key < target_key) {
					if (search_ptr->rchild != NULL) {
						if (search_ptr->rchild->key == target_key) {
							remove_target(search_ptr->rchild);
							return;
						}
						else search_ptr = search_ptr->rchild;
					}
					else {
						cout << "cannot remove node. there is no such node!" << endl;
						return;
					}
				}
				else {
					cout << "should not reach here while removing. key matching should be detected in other 'if'." << endl;
				}
			}
		}
	}

	void replace_with_inorder_predecessor(node*& target_ptr) {
		node* previous_ptr = NULL;
		node* traverse_ptr = target_ptr->lchild;
		while (traverse_ptr->rchild != NULL) {
			previous_ptr = traverse_ptr;
			traverse_ptr = traverse_ptr->rchild;
		}
		if (previous_ptr != NULL) previous_ptr->rchild = traverse_ptr->lchild;				//��������� �ڸ��� �Ųٷ��� ���������ڰ� ������ ���� �� �ִ� ���� �ڽ��� ������������ �θ𿡰� �ðܾ���
		else target_ptr->lchild = traverse_ptr->lchild;									//�ٵ� ��������� �����ڽ��� �ٷ� ������������ ��쿡�� ���������ڿ��� �ñ�� ���� ������ ������ �ڽ��� �ƴ� ���� �ڽ����� �ðܾ���.
		target_ptr->key = traverse_ptr->key;
		target_ptr->data = traverse_ptr->data;
		delete traverse_ptr;
	}

	void replace_with_inorder_successor(node*& target_ptr) {
		node* previous_ptr = NULL;
		node* traverse_ptr = target_ptr->rchild;
		while (traverse_ptr->lchild != NULL) {
			previous_ptr = traverse_ptr;
			traverse_ptr = traverse_ptr->lchild;
		}
		if (previous_ptr != NULL) previous_ptr->lchild = traverse_ptr->rchild;
		else target_ptr->rchild = traverse_ptr->rchild;
		target_ptr->key = traverse_ptr->key;
		target_ptr->data = traverse_ptr->data;
		delete traverse_ptr;
	}

	void remove_target(node*& target_ptr) {
		if (target_ptr->lchild != NULL && target_ptr->rchild != NULL) {						//�� �ڽ� ��� �ִ� ��쿣, ���������ڿ� �����ļ��� �߿��� �׳� �����ļ���(������ �ڽ� Ʈ������ ���� ���� Ű ���� ���)�� ���ֱ����
			replace_with_inorder_successor(target_ptr);
		}
		else if (target_ptr->lchild == NULL && target_ptr->rchild != NULL) {
			replace_with_inorder_successor(target_ptr);
		}
		else if (target_ptr->lchild != NULL && target_ptr->rchild == NULL) {
			replace_with_inorder_predecessor(target_ptr);
		}
		else {
			delete target_ptr;
			target_ptr = NULL;
		}
	}
};


int main() {
	tree& test_tree = *(new tree());
	test_tree.insert(5, 1515);
	test_tree.insert(7, 2727);
	test_tree.insert(3, 1313);
	test_tree.insert(4, 2424);
	test_tree.insert(6, 3636);
	cout << "traverse" << endl;
	test_tree.postorder_traverse_print();

	test_tree.remove(7);
	cout << "traverse" << endl;
	test_tree.postorder_traverse_print();

	test_tree.remove(3);
	cout << "traverse" << endl;
	test_tree.postorder_traverse_print();

	test_tree.remove(5);
	cout << "traverse" << endl;
	test_tree.postorder_traverse_print();

	test_tree.remove(4);
	cout << "traverse" << endl;
	test_tree.postorder_traverse_print();

	test_tree.remove(6);
	cout << "traverse" << endl;
	test_tree.postorder_traverse_print();

	return 0;
}