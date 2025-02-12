#include<iostream>
using namespace std;

class node {
	friend class tree;
	int key;
	int data;
	node* lchild;
	node* rchild;

	node(int key, int data) {
		this->key = key;
		this->data = data;
		this->lchild = NULL;
		this->rchild = NULL;
	}

	inline void set_key(int key) {
		this->key = key;
	}

	inline void set_data(int data) {
		this->data = data;
	}
};

//�ϴ� ������ ������ �������� Ÿ���� node*�̰�, �ִ� ������ 100����� �����ϰ� ��� ������ �����.
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

	bool is_empty() {
		if (item_num <= 0) return true;
		else return false;
	}
};

class tree {
	node* head;

	static inline void print_node(node* node_ptr) {
		cout << "node key : " << node_ptr->key << " / node data : " << node_ptr->data << endl;
	}

	static inline void remove_childs(node* node_ptr) {
		if (node_ptr->lchild) {
			delete node_ptr->lchild;
			node_ptr->lchild = NULL;
		}
		if (node_ptr->rchild) {
			delete node_ptr->rchild;
			node_ptr->rchild = NULL;
		}
	}

	//�θ��� Ư�� �ڽ������� ����(lchild or rchild)�� parent_seat�̶�� �̸��� ���۷��� ���ڷ� �޾ƿͼ� ���� �ڽ��� �Ҵ����ִ� �޼ҵ�. search �޼ҵ��� �Ϲ�ȭ�� ���� �̷��� ��������.
	static inline node* set_dummy_child(node*& parent_seat) {
		return parent_seat = new node(0, 0);
	}

	//�׳� �������� ���ڷ� ���� Ư�� ����� �ڽ� ������ �׳� �״�� ��ȯ��. search �޼ҵ��� �Ϲ�ȭ�� ���� �̷��� ��������.
	static inline node* get_node(node*& parent_seat) {
		return parent_seat;
	}

	static void replace_with_inorder_predecessor(node*& target_ptr) {
		node* previous_ptr = NULL;
		node* traverse_ptr = target_ptr->lchild;
		while (traverse_ptr->rchild != NULL) {
			previous_ptr = traverse_ptr;
			traverse_ptr = traverse_ptr->rchild;
		}
		if (previous_ptr != NULL) previous_ptr->rchild = traverse_ptr->lchild;			//��������� �ڸ��� �Ųٷ��� ���������ڰ� ������ ���� �� �ִ� ���� �ڽ��� ������������ �θ𿡰� �ðܾ���
		else target_ptr->lchild = traverse_ptr->lchild;								//�ٵ� ��������� �����ڽ��� �ٷ� ������������ ��쿡�� ���������ڿ��� �ñ�� ���� ������ ������ �ڽ��� �ƴ� ���� �ڽ����� �ðܾ���.
		target_ptr->key = traverse_ptr->key;
		target_ptr->data = traverse_ptr->data;
		delete traverse_ptr;
	}

	static void replace_with_inorder_successor(node*& target_ptr) {
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

	static node* remove_target(node*& target_ptr) {
		if (target_ptr->lchild != NULL && target_ptr->rchild != NULL) {				//�� �ڽ� ��� �ִ� ��쿣, ���������ڿ� �����ļ��� �߿��� �׳� �����ļ���(������ �ڽ� Ʈ������ ���� ���� Ű ���� ���)�� ���ֱ����
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
			return target_ptr;
		}
	}
	
	//"to_do_with_target_node" �Լ� �����ʹ� Ư�� target_key�� ���� Ʈ���� ��忡 ������ �۾��� ���� �������̽���
	//"to_do_with_target_hole" �Լ� �����ʹ� Ư�� target_key�� ���� ���ԵǱ⿡ ������ ��ġ(�ڽ� ������ ����)�� ������ �۾��� ���� �������̽���
	//�� �Լ� ������ ���, �θ��� �ڽ� ������ ������ ���� ������ �� �ֵ��� ���۷��� ���ڸ� ����
	node* search(int target_key, node* (* to_do_with_target_node)(node*&), node* (* to_do_with_target_hole)(node*&)) {
		if (head == NULL) return (*to_do_with_target_hole)(head);

		node* search_ptr = head;
		while (true) {
			if (target_key < search_ptr->key) {
				if (search_ptr->lchild != NULL) {
					if (search_ptr->lchild->key == target_key) return (*to_do_with_target_node)(search_ptr->lchild);
					else search_ptr = search_ptr->lchild;
				}
				else return (*to_do_with_target_hole)(search_ptr->lchild);
			}
			else if(search_ptr->key < target_key) {
				if (search_ptr->rchild != NULL) {
					if (search_ptr->rchild->key == target_key) return (*to_do_with_target_node)(search_ptr->rchild);
					else search_ptr = search_ptr->rchild;
				}
				else return (*to_do_with_target_hole)(search_ptr->rchild);
			}
			else {
				return (*to_do_with_target_node)(head);
			}
		}
	}

	//"to_do' �Լ� �����ʹ� ������ȸ�� ���鼭 �� ��忡 ������ �۾��� ���� �������̽���
	void preorder_traverse(void (* to_do)(node*)) {
		if (head == NULL) {
			cout << "can not traverse. there is no node." << endl;
			return;
		}
		stack head_stack;
		node* traverse_ptr = NULL;
		head_stack.push(this->head);
		while ((traverse_ptr = head_stack.pop())) {
			(*to_do)(traverse_ptr);
			if(traverse_ptr->rchild != NULL) head_stack.push(traverse_ptr->rchild);
			if(traverse_ptr->lchild != NULL) head_stack.push(traverse_ptr->lchild);
		}
	}

	//"to_do' �Լ� �����ʹ� ������ȸ�� ���鼭 �� ��忡 ������ �۾��� ���� �������̽���
	void inorder_traverse(void (* to_do)(node*)) {
		if (head == NULL) {
			cout << "can not traverse. there is no node." << endl;
			return;
		}
		stack head_stack;
		node* traverse_ptr = NULL;
		head_stack.push(head);
		bool new_left_spine = true;
		while (!head_stack.is_empty()) {
			while (new_left_spine && head_stack.get_top()->lchild) {
				head_stack.push(head_stack.get_top()->lchild);
			}
			traverse_ptr = head_stack.pop();
			(*to_do)(traverse_ptr);
			if (traverse_ptr->rchild) {
				new_left_spine = true;
				head_stack.push(traverse_ptr->rchild);
			}
			else new_left_spine = false;
		}
	}

	//"to_do' �Լ� �����ʹ� ������ȸ�� ���鼭 �� ��忡 ������ �۾��� ���� �������̽���
	void postorder_traverse(void (*to_do)(node*)) {
		if (head == NULL) {
			cout << "can not traverse. there is no node." << endl;
			return;
		}
		stack head_stack;
		node* traverse_ptr = NULL;
		head_stack.push(head);
		bool new_left_spine = true;
		bool new_right_spine = true;
		while (!head_stack.is_empty()) {
			while (new_left_spine && head_stack.get_top()->lchild) {
				head_stack.push(head_stack.get_top()->lchild);
			}
			traverse_ptr = head_stack.get_top();
			if (new_right_spine && traverse_ptr->rchild) {
				new_left_spine = true;
				head_stack.push(traverse_ptr->rchild);
			}
			else {
				(*to_do)(traverse_ptr);
				new_left_spine = false;
				node* previous_node = head_stack.pop();
				node* present_node = head_stack.get_top();
				if (present_node && present_node->rchild && present_node->rchild == previous_node) new_right_spine = false;
				else new_right_spine = true;
			}
		}
	}


public:
	tree() {
		cout << "tree is being made!" << endl;
		head = NULL;
	}

	~tree() {
		cout << "tree is being removed" << endl;
		remove_all();
	}

	void preorder_print() {
		cout << "preorder_traverse" << endl;
		preorder_traverse(&tree::print_node);
		cout << endl;
	}

	void inorder_print() {
		cout << "inorder_traverse" << endl;
		inorder_traverse(&tree::print_node);
		cout << endl;
	}

	void postorder_print() {
		cout << "postorder_traverse" << endl;
		postorder_traverse(&tree::print_node);
		cout << endl;
	}

	void remove_all() {
		cout << "remove all" << endl;
		postorder_traverse(&tree::remove_childs);
		delete head;
		head = NULL;
	}

	int get_data(int target_key) {
		node* target_node = search(target_key, &tree::get_node, NULL);
		return target_node->data;
	}

	node* insert(int new_key, int new_data) {
		node* made_child = search(new_key, NULL, &tree::set_dummy_child);
		made_child->set_key(new_key);
		made_child->set_data(new_data);
		return made_child;
	}

	void remove(int target_key) {
		search(target_key, &tree::remove_target, NULL);
	}
};


int main() {
	tree& test_tree = *(new tree());
	test_tree.insert(5, 1515);
	test_tree.insert(7, 2727);
	test_tree.insert(3, 1313);
	test_tree.insert(4, 2424);
	test_tree.insert(6, 3636);
	test_tree.insert(1, 5151);
	test_tree.insert(8, 5858);
	test_tree.preorder_print();

	test_tree.get_data(5);

	test_tree.remove(7);
	test_tree.preorder_print();

	test_tree.remove(3);
	test_tree.preorder_print();

	test_tree.remove(5);
	test_tree.preorder_print();

	test_tree.remove(4);
	test_tree.preorder_print();

	test_tree.remove(6);
	test_tree.preorder_print();

	test_tree.remove(1);
	test_tree.preorder_print();

	test_tree.remove(8);
	test_tree.preorder_print();
	delete &test_tree;
	return 0;
}