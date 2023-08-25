#include <queue>
#include <tuple>
#include <utility>
#include <vector>

template <typename value_T, typename index_T>
class Splay {
	const int not_exist = 0;

public:
	index_T root;

private:
	enum child_t { left = false, right = true };
	struct Node {
		value_T key;
		index_T size, count;
		index_T child[2];
		index_T parent;
		Node(const value_T key = value_T())
			: key(key), size(0), count(0), parent(not_exist) {
			child[left] = child[right] = not_exist;
		}
	};

	std::vector<Node> data;
	std::queue<index_T> bin;

	void maintain(index_T index) {
		if (index == not_exist) return;
		data[index].size = data[data[index].child[left]].size +
						   data[data[index].child[right]].size +
						   data[index].count;
	}

	child_t identify(index_T index) {
		return data[data[index].parent].child[right] == index ? right : left;
	}

	void clear(index_T index) {
		data[index] = Node();
		bin.push(index);
	}

	void rotate(index_T index) {
		int parent = data[index].parent;
		int grand = data[parent].parent;
		child_t type = identify(index);
		data[parent].child[type] = data[index].child[type ^ 1];
		if (data[index].child[type ^ 1])
			data[data[index].child[type ^ 1]].parent = parent;
		data[index].child[type ^ 1] = parent;
		if (grand) data[grand].child[identify(parent)] = index;
		data[index].parent = grand;
		data[parent].parent = index;
		maintain(parent), maintain(index);
	}

	void splay(index_T index) {
		for (index_T cur = data[index].parent; cur = data[index].parent, cur;
			 rotate(index)) {
			if (data[cur].parent != not_exist)
				rotate(identify(index) == identify(cur) ? cur : index);
		}
		root = index;
	}

	index_T new_node(value_T key) {
		index_T index;
		if (!bin.empty()) {
			index = bin.front();
			bin.pop();
		} else {
			index = data.size();
			data.push_back(Node());
		}
		data[index].key = key;
		data[index].count = 1;
		data[index].size = 1;
		return index;
	}

public:
	void insert(value_T key) {
		if (root == not_exist) {
			root = new_node(key);
			return;
		}
		index_T cur = root, parent = not_exist;
		while (cur != not_exist && data[cur].key != key) {
			parent = cur, cur = data[cur].child[data[cur].key < key];
		}
		if (cur == not_exist) {
			cur = new_node(key);
			data[cur].parent = parent;
			data[parent].child[data[parent].key < key] = cur;
		} else
			++data[cur].count;
		maintain(cur);
		maintain(parent);
		splay(cur);
	}

	index_T rank(value_T key) {
		index_T res = 0, cur = root;
		while (key != data[cur].key) {
			if (key < data[cur].key) {
				cur = data[cur].child[left];
			} else {
				res += data[data[cur].child[left]].size + data[cur].count;
				cur = data[cur].child[right];
			}
		}
		res += data[data[cur].child[left]].size;
		splay(cur);
		return res + 1;
	}

	value_T kth(index_T rank) {
		index_T cur = root;
		while (true) {
			if (data[cur].child[left] != not_exist &&
				rank <= data[data[cur].child[left]].size) {
				cur = data[cur].child[left];
			} else if (rank >
					   data[data[cur].child[left]].size + data[cur].count) {
				rank -= data[cur].count + data[data[cur].child[left]].size;
				cur = data[cur].child[right];
			} else {
				splay(cur);
				return data[cur].key;
			}
		}
	}

private:
	index_T prefix() {
		index_T cur = data[root].child[left];
		if (cur == not_exist) return cur;
		while (data[cur].child[right] != not_exist)
			cur = data[cur].child[right];
		splay(cur);
		return cur;
	}
	index_T suffix() {
		index_T cur = data[root].child[right];
		if (cur == not_exist) return cur;
		while (data[cur].child[left]) cur = data[cur].child[left];
		splay(cur);
		return cur;
	}

public:
	void erase(int k) {
		rank(k);
		if (data[root].count > 1) {
			--data[root].count;
			maintain(root);
			return;
		}
		if (data[root].child[left] == not_exist &&
			data[root].child[right] == not_exist) {
			clear(root);
			root = not_exist;
			return;
		}
		if (data[root].child[left] == not_exist ||
			data[root].child[right] == not_exist) {
			index_T cur = root;
			root = data[root].child[right] + data[root].child[left];
			data[root].parent = not_exist;
			clear(cur);
			return;
		}
		index_T cur = root, x = prefix();
		data[data[cur].child[right]].parent = x;
		data[x].child[right] = data[cur].child[right];
		clear(cur);
		maintain(root);
	}

	value_T prefix(value_T key) {
		insert(key);
		index_T index = prefix();
		erase(key);
		return data[index].key;
	}
	value_T suffix(value_T key) {
		insert(key);
		index_T index = suffix();
		erase(key);
		return data[index].key;
	}
	Splay() {
		root = new_node(value_T()), data[root].size = data[root].count = 0;
	}
};

template <typename index_T, typename value_T, index_T (*rand)()>
class FHQTreap {
private:
	enum child_t { left = false, right = true };
	struct Node {
		Node* child[2];
		value_T key;
		index_T value;
		index_T size, count;

		Node(value_T key) : key(key), size(1), count(1) {
			child[left] = child[right] = nullptr;
			value = rand();
		}
		Node(Node* _node) {
			child[left] = _node->child[left],
			child[right] = _node->child[right];
			key = _node->key, value = _node->value;
			size = _node->size, count = _node->count;
		}

		void maintain() {
			size = count;
			if (child[left] != nullptr) size += child[left]->size;
			if (child[right] != nullptr) size += child[right]->size;
		}
	};

public:
	Node* root;

private:
	std::pair<Node*, Node*> split_by_key(Node* cur, value_T key) {
		if (cur == nullptr) return std::make_pair(nullptr, nullptr);
		if (cur->key <= key) {
			std::pair<Node*, Node*> res = split_by_key(cur->child[right], key);
			cur->child[right] = res.first, cur->maintain();
			return std::make_pair(cur, res.second);
		} else {
			std::pair<Node*, Node*> res = split_by_key(cur->child[left], key);
			cur->child[left] = res.second, cur->maintain();
			return std::make_pair(res.first, cur);
		}
	}

	std::tuple<Node*, Node*, Node*> split_by_rank(Node* cur, index_T rank) {
		if (cur == nullptr) return std::make_tuple(nullptr, nullptr, nullptr);
		index_T left_size =
			cur->child[left] == nullptr ? 0 : cur->child[left]->size;
		if (rank <= left_size) {
			Node *l, *mid, *r;
			std::tie(l, mid, r) = split_by_rank(cur->child[left], rank);
			cur->child[left] = r, cur->maintain();
			return std::make_tuple(l, mid, cur);
		} else if (rank <= left_size + cur->count) {
			Node *l = cur->child[left], *r = cur->child[right];
			cur->child[left] = cur->child[right] = nullptr;
			return std::make_tuple(l, cur, r);
		} else {
			Node *l, *mid, *r;
			std::tie(l, mid, r) =
				split_by_rank(cur->child[right], rank - left_size - cur->count);
			cur->child[right] = l, cur->maintain();
			return std::make_tuple(cur, mid, r);
		}
	}

	Node* merge(Node* u, Node* v) {
		if (u == nullptr && v == nullptr) return nullptr;
		if (u == nullptr || v == nullptr) return u == nullptr ? v : u;
		if (u->value < v->value)
			return u->child[right] = merge(u->child[right], v), u->maintain(),
				   u;
		else
			return v->child[left] = merge(u, v->child[left]), v->maintain(), v;
	}

public:
	void insert(const value_T key) {
		std::pair<Node*, Node*> u = split_by_key(root, key);
		std::pair<Node*, Node*> v = split_by_key(u.first, key - 1);
		Node* cur;
		if (v.second == nullptr)
			cur = new Node(key);
		else
			++v.second->count, v.second->maintain();
		root = merge(merge(v.first, v.second == nullptr ? cur : v.second),
					 u.second);
	}

	void erase(const value_T key) {
		std::pair<Node*, Node*> u = split_by_key(root, key);
		std::pair<Node*, Node*> v = split_by_key(u.first, key - 1);
		if (v.second->count > 1)
			--v.second->count, v.second->maintain(),
				v.first = merge(v.first, v.second);
		else {
			if (u.first == v.second) u.first = nullptr;
			delete v.second;
			v.second = nullptr;
		}
		root = merge(v.first, u.second);
	}

	index_T rank(Node* cur, const value_T key) {
		std::pair<Node*, Node*> res = split_by_key(cur, key - 1);
		index_T rank = (res.first == nullptr ? 0 : res.first->size) + 1;
		root = merge(res.first, res.second);
		return rank;
	}

	value_T kth(Node* cur, const index_T rank) {
		Node *l, *mid, *r;
		std::tie(l, mid, r) = split_by_rank(cur, rank);
		value_T key = mid->key;
		root = merge(merge(l, mid), r);
		return key;
	}

	value_T prefix(value_T key) {
		std::pair<Node*, Node*> temp = split_by_key(root, key - 1);
		value_T ret = kth(temp.first, temp.first->size);
		root = merge(temp.first, temp.second);
		return ret;
	}
	value_T suffix(value_T key) {
		std::pair<Node*, Node*> temp = split_by_key(root, key);
		value_T ret = kth(temp.second, 1);
		root = merge(temp.first, temp.second);
		return ret;
	}

public:
	FHQTreap() : root(nullptr){};
	~FHQTreap() { delete root; }
};
