#include <iostream>
#include <queue>

using namespace std;

enum COLOR {RED, BLACK};

struct Node {
	COLOR color;
	Node* left;
	Node* right;
	Node* parent;
	int value;
	int count;
	Node(int value) {
		this->value = value;
		this->count = 0;
		this->left = NULL;
		this->right = NULL;
		this->parent = NULL;
		this->color = RED;
	}
};

class RedBlackTree {
public:
	Node* root;

	RedBlackTree(int value) {
		root = new Node(value);
		this->root->color = BLACK;
	}
	void insertNode(int value) {
		Node* current_node = new Node(value);
		Node* current = root;
		Node* parent = root;
		while (current != nullptr) {
			parent = current;
			if (current_node->value < current->value) {
				current = current->left;
			}
			else if (current_node->value > current->value) {
				current = current->right;
			}
			else {
				current->count++;
				delete current_node;
				return;
			}
		}
		current_node->parent = parent;
		if (current_node->value < parent->value) {
			parent->left = current_node;
		}
		else {
			parent->right = current_node;
		}
		BalanceTree(current_node);
	}
	void BalanceTree(Node* &node) {
		Node* parent = nullptr;
		Node* grandparent = nullptr;

		while (node != root && node->color == RED && node->parent->color == RED) {
			parent = node->parent;
			grandparent = parent->parent;

			bool isLeft = grandparent->left == parent;
			Node* uncle = isLeft ? grandparent->right : grandparent->left;

			if (uncle && uncle->color == RED) {
				grandparent->color = RED;
				parent->color = BLACK;
				uncle->color = BLACK;
				node = grandparent;
			}
			else {
				if (node == (isLeft ? parent->right : parent->left)) {
					node = parent;
					isLeft ? leftRotate(node) : rightRotate(node);
				}
				parent->color = BLACK;
				grandparent->color = RED;
				isLeft ? rightRotate(grandparent) : leftRotate(grandparent);
			}
		}
		root->color = BLACK;
	}


	void leftRotate(Node*& node) {
		Node* right_child = node->right;
		node->right = right_child->left;
		if (node->right != nullptr) {
			node->right->parent = node;
		}
		right_child->parent = node->parent;
		if (right_child->parent == nullptr) {
			root = right_child;
		}
		else if (node->parent->left == node) {
			node->parent->left = right_child;
		}
		else node->parent->right = right_child;
		right_child->left = node;
		node->parent = right_child;
	}
	void rightRotate(Node*& node) {
		Node* left_child = node->left;
		node->left = left_child->right;
		if (node->left != nullptr) {
			node->left->parent = node;
		}
		left_child->parent = node->parent;
		if (left_child->parent == nullptr) {
			root = left_child;
		}
		else if (node->parent->left == node) {
			node->parent->left = left_child;
		}
		else node->parent->right = left_child;
		left_child->right = node;
		node->parent = left_child;
	}

	void symmetricalBypass(Node* &node) {
		if (node == nullptr) return;
		symmetricalBypass(node->left);
		cout << node->value << " ";
		symmetricalBypass(node->right);
	}

	void widthBypass(Node* &node) {
		if (node == nullptr) return;
		
		queue<Node*> q;
		q.push(node);
		Node* current;
		while (!q.empty()) {
			current = q.front();
			q.pop();

			cout << current->value << " ";
		
			if (current->left) {
				q.push(current->left);
			}
			if (current->right) {
				q.push(current->right);
			}
		}
	}
	
	double sumNodes(Node* &node) {
		if (node == nullptr) return 0;
		return node->value + sumNodes(node->left) + sumNodes(node->right);
	}

	int countNodes(Node*& node) {
		if (node == nullptr) return 0;
		return 1 + countNodes(node->left) + countNodes(node->right);
	}

	double averageNodes(Node* &root) {
		return sumNodes(root) / countNodes(root);
	}

	int lengthNode(int value) {
		Node* current = root;
		int length = 0;
		while (current != nullptr) {
			if (value == current->value) return length;
			
			current = (value < current->value ? current->left : current->right);
			length++;
		}
		return -1;
	}

	void PrintTree(Node* node, int space = 0, int indent = 6) {
		if (node == nullptr) return;

		space += indent;

		PrintTree(node->right, space);

		cout << endl;
		for (int i = indent; i < space; i++) {
			cout << " ";
		}

		cout << node->value << " (" << (node->color == RED ? "RED" : "BLACK") << ")" << endl;

		PrintTree(node->left, space);
	}
};


void listCommand() {
	cout << "\nСписок команд: " << endl
		<< " - Вставка элемента (1)" << endl
		<< " - Симметричный обход (2)" << endl
		<< " - Обход в ширину (3)" << endl
		<< " - Среднее арифметическое узлов (4)" << endl
		<< " - Длина пути от корня до заданного значения (5)" << endl
		<< " - Вывод дерева (6)" << endl
		<< " - Для выхода нажмите 0" << endl << endl;
}

int main() {
	setlocale(LC_ALL, "ru");

	int root;
	cout << "Введите значение корня\n";
	cin >> root;
	RedBlackTree tree(root);

	int startCountNodes;
	int value;
	cout << "Введите количество элементов для заполнения дерева\n";
	cin >> startCountNodes;
	for (int i = 0; i < startCountNodes; i++) {
		cout << "Введите элемент: ";
		cin >> value;
		tree.insertNode(value);
	}

	listCommand(); 
	int command;
	while (true) {
		cout << "Команда: ";
		cin >> command;
		switch (command)
		{
		case 0: return 0;
		case 1:
			int insValue;
			cout << "Введите элемент: ";
			cin >> insValue;
			tree.insertNode(insValue);
			break;
		case 2:
			cout << "Симметричный обход: ";
			tree.symmetricalBypass(tree.root);
			break;
		case 3:
			cout << "Обход в ширину: ";
			tree.widthBypass(tree.root);
			break;
		case 4:
			cout << "Среднее арифметическое узлов: ";
			cout << tree.averageNodes(tree.root);
			break;
		case 5:
			int lengthValue;
			cout << "Введите элемент, для которого хотите подсчитать путь: ";
			cin >> lengthValue;
			cout << "Длина пути: ";
			cout << tree.lengthNode(lengthValue);
			break;
		case 6:
			tree.PrintTree(tree.root);
			break;
		}
		cout << '\n';
	}
}
