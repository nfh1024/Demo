/*************************************************************************
	> File Name: red.cpp
	> Author: Nfh
	> Mail: 1024222310@qq.com 
	> Created Time: 2020年08月13日 星期四 18时03分44秒
 ************************************************************************/

#include <cstdio>
using namespace std;

#define bro(x) (((x)->ftr->lc == (x)) ? ((x)->ftr->rc) : ((x)->ftr->lc))

typedef bool RB_COLOR;
#define RB_COLOR_RED true
#define RB_COLOR_BLACK false

template <typename T>
class redblacktree {
	protected:
		struct RB_Node;
		
		RB_Node* _root;
		RB_Node* _hot;
		
		int _size;
		
		void init(T);
		
		RB_Node* zig(RB_Node*);
		RB_Node* zag(RB_Node*);
		
		void SolveDoubleRed(RB_Node*);
		void SolveLostBlack(RB_Node*);
		
		RB_Node* find(T);
		
		void removetree(RB_Node*);	//dfs后序遍历
		
	public:
		struct iterator;
		
		redblacktree() : _root(NULL), _hot(NULL), _size(0) {}
		
		iterator insert(T);
		bool remove(T);
		bool remove(iterator&);
		
		iterator search(T);
		iterator lower_bound(T);
		iterator upper_bound(T);
		
		void clear();	//dfs后序遍历入口
		
		int size();
		
		bool empty();
		
		iterator begin();
		static iterator end();
};

template <typename T>
struct redblacktree<T>::RB_Node {
	T val;
	RB_COLOR RBc;
	RB_Node* ftr;
	RB_Node* lc;
	RB_Node* rc;
	
	RB_Node(T v = T(), RB_COLOR RB = RB_COLOR_RED, RB_Node* f = NULL, RB_Node* lchild = NULL, RB_Node* rchild = NULL) :
		val(v), RBc(RB), ftr(f), lc(lchild), rc(rchild) {}
	
	RB_Node* succ() {
		RB_Node* ptn = rc;
		while(ptn->lc) {
			ptn = ptn->lc;
		}
		return ptn;
	}
	
	RB_Node* left_node() {
		RB_Node* ptn = this;
		if(!lc) {
			while(ptn->ftr && ptn->ftr->lc == ptn) {
				ptn = ptn->ftr;
			}
			ptn = ptn->ftr;
		} else {
			ptn = ptn->lc;
			while(ptn->rc) {
				ptn = ptn->rc;
			}
		}
		return ptn;
	}
	
	RB_Node* right_node() {
		RB_Node* ptn = this;
		if(!rc) {
			while(ptn->ftr && ptn->ftr->rc == ptn) {
				ptn = ptn->ftr;
			}
			ptn = ptn->ftr;
		} else {
			ptn = ptn->rc;
			while(ptn->lc) {
				ptn = ptn->lc;
			}
		}
		return ptn;
	}
};

template <typename T>
struct redblacktree<T>::iterator {
	protected:
		RB_Node* _real__node;
		
	public:
		T operator*() {
			return _real__node->val;
		}
		
		bool operator==(iterator const& itr) {
			return _real__node == itr._real__node;
		}
		
		bool operator!=(iterator const& itr) {
			return _real__node != itr._real__node;
		}
		
		bool operator!() {
			return !_real__node;
		}
		
		iterator& operator=(iterator const& itr) {
			_real__node = itr._real__node;
			return *this;
		}
		
		iterator& operator++() {
			_real__node = _real__node->right_node();
			return *this;
		}
		
		iterator& operator--() {
			_real__node = _real__node->left_node();
			return *this;
		}
		
		iterator(RB_Node* node_nn = NULL) : _real__node(node_nn) {}
		iterator(T const& val_vv) : _real__node(find(val_vv)) {}
		iterator(iterator const& itr) : _real__node(itr._real__node) {}
};

template <typename T>
	typename
redblacktree<T>::RB_Node* redblacktree<T>::find(T v) {
	RB_Node* ptn = _root;
	_hot = NULL;
	while(ptn && ptn->val != v) {
		_hot = ptn;
		if(ptn->val > v) {
			ptn = ptn->lc;
		} else {
			ptn = ptn->rc;
		}
	}
	return ptn;
}

template <typename T>
	typename
redblacktree<T>::iterator redblacktree<T>::search(T v) {
	return iterator(find(v));
}

template <typename T>
	typename
redblacktree<T>::iterator redblacktree<T>::lower_bound(T v) {
	RB_Node* ptn = _root;
	_hot = NULL;
	while(ptn) {
		_hot = ptn;
		if(ptn->val >= v) {
			ptn = ptn->lc;
		} else {
			ptn = ptn->rc;
		}
	}
	if(_hot->val >= v) {
		ptn = _hot;
	} else {
		ptn = _hot->right_node();
	}
	return iterator(ptn);
}

template <typename T>
	typename
redblacktree<T>::iterator redblacktree<T>::upper_bound(T v) {
	RB_Node* ptn = _root;
	_hot = NULL;
	while(ptn) {
		_hot = ptn;
		if(ptn->val > v) {
			ptn = ptn->lc;
		} else {
			ptn = ptn->rc;
		}
	}
	if(_hot->val > v) {
		ptn = _hot;
	} else {
		ptn = _hot->right_node();
	}
	return iterator(ptn);
}

template <typename T>
void redblacktree<T>::init(T v) {
	_root = new RB_Node(v, RB_COLOR_BLACK, NULL, NULL, NULL);
	_size = 1;
}

template <typename T>
	typename
redblacktree<T>::RB_Node* redblacktree<T>::zig(RB_Node* ptn) {
	ptn->lc->ftr = ptn->ftr;
	if(ptn->ftr) {
		if(ptn->ftr->lc == ptn) {
			ptn->ftr->lc = ptn->lc;
		} else {
			ptn->ftr->rc = ptn->lc;
		}
	}
	if(ptn->lc->rc) {
		ptn->lc->rc->ftr = ptn;
	}
	ptn->ftr = ptn->lc;
	ptn->lc = ptn->lc->rc;
	ptn->ftr->rc = ptn;
	return ptn->ftr;
}

template <typename T>
	typename
redblacktree<T>::RB_Node* redblacktree<T>::zag(RB_Node* ptn) {
	ptn->rc->ftr = ptn->ftr;
	if(ptn->ftr) {
		if(ptn->ftr->lc == ptn) {
			ptn->ftr->lc = ptn->rc;
		} else {
			ptn->ftr->rc = ptn->rc;
		}
	}
	if(ptn->rc->lc) {
		ptn->rc->lc->ftr = ptn;
	}
	ptn->ftr = ptn->rc;
	ptn->rc = ptn->rc->lc;
	ptn->ftr->lc = ptn;
	return ptn->ftr;
}

template <typename T>
	typename
redblacktree<T>::iterator redblacktree<T>::insert(T v) {
	RB_Node* ptn = find(v);
	if(ptn) {
		return iterator(ptn);
	}
	if(!_hot) {
		//assert(_size == 0);
		init(v);
		return iterator(_root);
	}
	++_size;
	ptn = new RB_Node(v, RB_COLOR_RED, _hot, NULL, NULL);
	if(_hot->val < v) {
		_hot->rc = ptn;
	} else {
		_hot->lc = ptn;
	}
	SolveDoubleRed(ptn);
	return iterator(ptn);
}

template <typename T>
void redblacktree<T>::SolveDoubleRed(RB_Node* nn) {
	while(nn->ftr && nn->ftr->RBc == RB_COLOR_RED) {	//排除递归到根和RR-0的情况
		RB_Node* pftr = nn->ftr;
		RB_Node* uncle = bro(pftr);
		RB_Node* grdftr = pftr->ftr;
		if(uncle && uncle->RBc == RB_COLOR_RED) {		//RR-2
			grdftr->RBc = RB_COLOR_RED;
			uncle->RBc = RB_COLOR_BLACK;
			pftr->RBc = RB_COLOR_BLACK;
			nn = grdftr;
		} else {										//RR-1
			if(grdftr->lc == pftr) {
				if(pftr->lc == nn) {
					if(grdftr == _root) {
						_root = pftr;
					}
					zig(grdftr);
					pftr->RBc = RB_COLOR_BLACK;
					grdftr->RBc = RB_COLOR_RED;
				} else {
					if(grdftr == _root) {
						_root = nn;
					}
					zag(pftr); zig(grdftr);
					nn->RBc = RB_COLOR_BLACK;
					grdftr->RBc = RB_COLOR_RED;
				}
			} else {
				if(pftr->lc == nn) {
					if(grdftr == _root) {
						_root = nn;
					}
					zig(pftr); zag(grdftr);
					nn->RBc = RB_COLOR_BLACK;
					grdftr->RBc = RB_COLOR_RED;
				} else {
					if(grdftr == _root) {
						_root = pftr;
					}
					zag(grdftr);
					pftr->RBc = RB_COLOR_BLACK;
					grdftr->RBc = RB_COLOR_RED;
				}
			}
			return;
		}
	}
	if(nn == _root) {
		nn->RBc = RB_COLOR_BLACK;
	}
}

template <typename T>
	typename 
redblacktree<T>::iterator redblacktree<T>::begin() {
	RB_Node* ptn = _root;
	while(ptn->lc) {
		ptn = ptn->lc;
	}
	return iterator(ptn);
}

template <typename T>
	typename 
redblacktree<T>::iterator redblacktree<T>::end() {
	return iterator((RB_Node*)NULL);
}

template <typename T>
int redblacktree<T>::size() {
	return _size;
}

template <typename T>
bool redblacktree<T>::empty() {
	return !_size;
}

template <typename T>
void redblacktree<T>::clear() {
	removetree(_root);
	_size = 0;
	_root = NULL;
}

template <typename T>
void redblacktree<T>::removetree(RB_Node* ptn) {
	if(!ptn) {
		return;
	}
	removetree(ptn->lc);
	removetree(ptn->rc);
	delete ptn;
}

template <typename T>
void redblacktree<T>::SolveLostBlack(RB_Node* nn) {
	while(nn != _root) {
		RB_Node* pftr = nn->ftr;
		RB_Node* bthr = bro(nn);
		if(bthr->RBc == RB_COLOR_RED) {		//LB-1
			bthr->RBc = RB_COLOR_BLACK;
			pftr->RBc = RB_COLOR_RED;
			if(_root == pftr) {
				_root = bthr;
			}
			if(pftr->lc == nn) {
				zag(pftr);
			} else {
				zig(pftr);
			}
			bthr = bro(nn);
			pftr = nn->ftr;
		}
		
		if(bthr->lc && bthr->lc->RBc == RB_COLOR_RED) {			//LB-3
			RB_COLOR oldRBc = pftr->RBc;
			pftr->RBc = RB_COLOR_BLACK;
			if(pftr->lc == nn) {
				if(_root == pftr) {
					_root = bthr->lc;
				}
				zig(bthr); zag(pftr);
			} else {
				bthr->lc->RBc = RB_COLOR_BLACK;
				if(_root == pftr) {
					_root = bthr;
				}
				zig(pftr);
			}
			pftr->ftr->RBc = oldRBc;
			return;
		} else if(bthr->rc && bthr->rc->RBc == RB_COLOR_RED) {	//LB-3
			RB_COLOR oldRBc = pftr->RBc;
			pftr->RBc = RB_COLOR_BLACK;
			if(pftr->lc == nn) {
				bthr->rc->RBc = RB_COLOR_BLACK;
				if(_root == pftr) {
					_root = bthr;
				}
				zag(pftr);
			} else {
				if(_root == pftr) {
					_root = bthr->rc;
				}
				zag(bthr); zig(pftr);
			}
			pftr->ftr->RBc = oldRBc;
			return;
		}
		
		if(pftr->RBc == RB_COLOR_RED) {		//LB-2R
			pftr->RBc = RB_COLOR_BLACK;
			bthr->RBc = RB_COLOR_RED;
			return;
		} else {							//LB-2B
			bthr->RBc = RB_COLOR_RED;
			nn = pftr;
		}
	}
}

template <typename T>
bool redblacktree<T>::remove(T v) {
	RB_Node* ptn = find(v);
	RB_Node* node_suc;
	if(!ptn) {
		return false;
	}
	--_size;
	while(ptn->lc || ptn->rc) {
		if(!(ptn->lc)) {
			node_suc = ptn->rc;
		} else if(!(ptn->rc)) {
			node_suc = ptn->lc;
		} else {
			node_suc = ptn->succ();
		}
		ptn->val = node_suc->val;
		ptn = node_suc;
	}
	if(ptn->RBc == RB_COLOR_BLACK) {
		SolveLostBlack(ptn);
	}
	if(ptn->ftr) {
		if(ptn->ftr->lc == ptn) {
			ptn->ftr->lc = NULL;
		} else {
			ptn->ftr->rc = NULL;
		}
	}
	if(_root == ptn) {
		//assert(_size == 0);
		_root = NULL;
	}
	delete ptn;
	return true;
}

template <typename T>
bool redblacktree<T>::remove(iterator& itr) {
	RB_Node* ptn = itr._real__node;
	itr._real__node = itr._real__node->right_node();
	if(!(itr._real__node)) {
		itr._real__node = ptn->left_node();
	}
	RB_Node* node_suc;
	--_size;
	while(ptn->lc || ptn->rc) {
		if(!(ptn->lc)) {
			node_suc = ptn->rc;
		} else if(!(ptn->rc)) {
			node_suc = ptn->lc;
		} else {
			node_suc = ptn->succ();
		}
		ptn->val = node_suc->val;
		ptn = node_suc;
	}
	if(ptn->RBc == RB_COLOR_BLACK) {
		SolveLostBlack(ptn);
	}
	if(ptn->ftr) {
		if(ptn->ftr->lc == ptn) {
			ptn->ftr->lc = NULL;
		} else {
			ptn->ftr->rc = NULL;
		}
	}
	if(_root == ptn) {
		_root = NULL;
	}
	delete ptn;
	return true;
}

redblacktree<int> s;
#include <cstdlib>
#include <ctime>

int i;

int main() {
	srand(time(NULL));
	for(i = 0;i < 256;++i) {
		s.insert(i);
	}
	redblacktree<int>::iterator it;
	for(i = 0;i < 256;++i) {
		s.remove(i);
	}
	
	
	return 0;
}

