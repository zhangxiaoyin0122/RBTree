#include <iostream>
using namespace std;

enum Color {
	RED,
	BLACK
};

//节点
template<class K,class V>
struct RBNode {
	pair<K, V> _value;
	Color _c;
	RBNode<K,V>* _left;
	RBNode<K,V>* _right;
	RBNode<K,V>* _parent;

	RBNode(const pair<K,V>& val = pair<K,V>())
		:_value(val)
		,_c(RED)
		,_left(nullptr)
		,_right(nullptr)
		,_parent(nullptr)
	{}
};
//红黑树
template<class K,class V>
class RBTree {
	typedef RBNode<K, V> Node;

	RBTree()
		:_header(new Node)
	{
		_header->_left = _header->_right = _header;
	}
	//元素的插入:
	bool insert(const pair<K, V>& val) {
		//空树
		if (_header->_parent == nullptr) {
			Node* root = new Node(val);
			root->_c = BLACK;

			_header->_parent = root;
			root->_parent = _header;

			_header->_left = root;
			_header->_right = root;
			return true;
		}
		//非空树,走查找
		Node* cur = _header->_parent;
		Node* parent = nullptr;
		while (cur) {
			parent = cur;
			if (cur->_value.first == val.first) {
				return false;
			}
			else if (cur->_value.first > val.first) {
				cur = cur->_left;
			}
			else {
				cur = cur->_right;
			}
		}
		//插入
		cur = new Node(val);
		if (parent->_value.first > val.first) {
			parent->_left = cur;
		}
		else {
			parent->_right = cur;
		}
		cur->_parent = parent;

		//调整颜色
		while (cur != _header->_parent && cur->_parent->_c == RED) {
			Node* p = cur->_parent;
			Node* g = p->_parent;

			if (g->_left == p) {
				Node* u = g->_right;
				if (u && u->_c == RED) {
					p->_c = u->_c = BLACK;
					g->_c = RED;
					cur = g;
				}
				//u不存在或者u的颜色是黑色
				else {
					if (p->_right == cur) {
						RotateL(p);
						swap(p, cur);
					}
					RotateR(g);

					p->_c = BLACK;
					g->_c = RED;

					break;
				}
			}
			else {
				Node* u = g->_left;
				if (u && u->_c == RED) {

					p->_c = u->_c = BLACK;
					g->_c = RED;
					//继续向上调整
					cur->_right;
				}//u不存在或者u的颜色是黑色
				else {
					if (p->_left == cur) {
						//右边的左边高:右旋
						RotateR(p);
						swap(p, cur);
					}
					//左旋
					RotateL(g);
					p->_c = BLACK;
					g->_c = RED;
					break;
				}
			}
		}
		//根节点颜色置为黑色
		_header->_parent->_c = BLACK;
		_header->_left = LeftMost();
		_header->_right = RightMost();
		return true;
	}

	Node* LeftMost() {
		Node* cur = _header->_parent;
		while (cur && cur->_left) {
			cur = cur->_left;
		}
		return cur;
	}

	Node* RightMost() {
		Node* cur = _header->_parent;
		while (cur && cur->_right) {
			cur = cur->_right;
		}
		return cur;
	}
	//右旋:左边的左边高
	void RotateR(Node* parent) {
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		subL->_right = parent;
		parent->_left = subLR;

		if (subLR) {
			subLR->_parent = parent;
		}

		if (parent == _header->_parent) {
			_header->_parent = subL;
			subL->_parent = _header;
		}
		else {
			Node* g = parent->_parent;
			subL->_parent = g;
			if (g->_left == parent)
				g->_left = subL;
			else
				g->_right = subL;
		}
		parent->_parent = subL;
	}
	//左旋:右边的右边高
	void RotateL(Node* parent) {
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		subR->_left = parent;
		parent->_right = subRL;

		if (subRL) {
			subRL->_parent = parent;
		}

		if (parent == _header->_parent) {
			_header->_parent = subR;
			subR->_parent = _header;
		}
		else {
			Node* g = parent->_parent;
			subR->_parent = g;
			if (g->_left == parent)
				g->_left = subR;
			else
				g->_right = subR;
		}
		parent->_parent = subR;
	}
	//中序遍历
	void inorder() {
		_inorder(_header->_parent);
		cout << endl;
	}
	void _inorder(Node* root) {
		if (root) {
			_inorder(root->_left);
			cout << root->_value.first << " ";
			_inorder(root->_right);
		}
	}

private:
	Node* _header;
};