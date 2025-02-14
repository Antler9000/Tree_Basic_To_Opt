#ifndef BST_USING_WHILE_H
#define BST_USING_WHILE_H

#include "stack.h"

class tree_node {
	friend class tree;
	int key;
	int data;
	tree_node* lchild;
	tree_node* rchild;

	tree_node(int key, int data) {
		this->key = key;
		this->data = data;
		this->lchild = NULL;
		this->rchild = NULL;
	}

	void set_key(int key) {
		this->key = key;
	}

	void set_data(int data) {
		this->data = data;
	}
};

class tree {
	tree_node* head;

	//"to_do_with_target_tree_node" �Լ� �����ʹ� Ư�� target_key�� ���� Ʈ���� ��忡 ������ �۾��� ���� �������̽���
	//"to_do_with_target_hole" �Լ� �����ʹ� Ư�� target_key�� ���� ���ԵǱ⿡ ������ ��ġ(�ڽ� ������ ����)�� ������ �۾��� ���� �������̽���
	//�� �Լ� ������ ���, �θ��� �ڽ� ������ ������ ���� ������ �� �ֵ��� ���۷��� ���ڸ� ����
	tree_node* search(int target_key, tree_node* (*to_do_with_target_tree_node)(tree_node*&), tree_node* (*to_do_with_target_hole)(tree_node*&));

	//"to_do' �Լ� �����ʹ� ������ȸ�� ���鼭 �� ��忡 ������ �۾��� ���� �������̽���
	void preorder_traverse(void (*to_do)(tree_node*));

	//"to_do' �Լ� �����ʹ� ������ȸ�� ���鼭 �� ��忡 ������ �۾��� ���� �������̽���
	void inorder_traverse(void (*to_do)(tree_node*));

	//"to_do' �Լ� �����ʹ� ������ȸ�� ���鼭 �� ��忡 ������ �۾��� ���� �������̽���
	void postorder_traverse(void (*to_do)(tree_node*));

	/*-----�� �Ʒ��� private �޼ҵ���� �� �Ϲ� �޼ҵ��(Ž��, ��ȸ)�� �޼ҵ� ������(�������̽�)�� ���޵Ǿ� �����ϴ� ���� �޼ҵ����-----*/

	static void print_tree_node(tree_node* tree_node_ptr) {
		cout << "tree_node key : " << tree_node_ptr->key << " / tree_node data : " << tree_node_ptr->data << endl;
	}

	static void remove_childs(tree_node* tree_node_ptr);

	static tree_node* get_tree_node(tree_node*& parent_seat) {
		return parent_seat;
	}

	static tree_node* set_dummy_child(tree_node*& parent_seat) {
		return parent_seat = new tree_node(0, 0);
	}

	static tree_node* remove_target(tree_node*& target_ptr);

	static void replace_with_inorder_predecessor(tree_node*& target_ptr);

	static void replace_with_inorder_successor(tree_node*& target_ptr);

public:
	tree() {
		cout << "tree is being made!" << endl;
		head = NULL;
	}

	~tree() {
		cout << "tree is being removed" << endl;
		remove_all();
	}

	int get_data(int target_key) {
		tree_node* target_tree_node = search(target_key, &tree::get_tree_node, NULL);
		return target_tree_node->data;
	}

	tree_node* insert(int new_key, int new_data) {
		tree_node* made_child = search(new_key, NULL, &tree::set_dummy_child);
		made_child->set_key(new_key);
		made_child->set_data(new_data);
		return made_child;
	}

	void remove(int target_key) {
		search(target_key, &tree::remove_target, NULL);
	}

	void remove_all() {
		cout << "remove all" << endl;
		postorder_traverse(&tree::remove_childs);
		delete head;
		head = NULL;
	}

	void preorder_print() {
		cout << "preorder_traverse" << endl;
		preorder_traverse(&tree::print_tree_node);
		cout << endl;
	}

	void inorder_print() {
		cout << "inorder_traverse" << endl;
		inorder_traverse(&tree::print_tree_node);
		cout << endl;
	}

	void postorder_print() {
		cout << "postorder_traverse" << endl;
		postorder_traverse(&tree::print_tree_node);
		cout << endl;
	}
};

#endif //BST_USING_WHILE_H