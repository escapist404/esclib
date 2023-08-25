#include <vector>

template <typename index_T, typename value_t, value_t (*opt)(value_t, value_t),
		  value_t (*e_val)(), typename tag_T,
		  value_t (*apply)(value_t, tag_T, index_T),
		  tag_T (*attach)(tag_T, tag_T), tag_T (*e_tag)()>
class LazySegmentTree {
private:
	index_T n;
	std::vector<value_t> val;
	std::vector<tag_T> tag;
	mutable std::vector<std::pair<index_T, index_T>> child;
	index_T root;
	const index_T no_child = 0;

	inline void new_node(index_T& index) {
		index = val.size();
		val.push_back(e_val());
		tag.push_back(e_tag());
		child.push_back(std::make_pair(no_child, no_child));
	}

	inline void push_up(const index_T index) {
		val[index] = opt(val[child[index].first], val[child[index].second]);
	}
	inline void push_down(const index_T index, const index_T size) {
		if (child[index].first == no_child) new_node(child[index].first);
		if (child[index].second == no_child) new_node(child[index].second);
		val[child[index].first] =
			apply(val[child[index].first], tag[index], size / 2);
		val[child[index].second] =
			apply(val[child[index].second], tag[index], size - size / 2);
		tag[child[index].first] = attach(tag[child[index].first], tag[index]);
		tag[child[index].second] = attach(tag[child[index].second], tag[index]);
		tag[index] = e_tag();
	}
	void set(const index_T _l, const index_T _r, const tag_T _tag,
			 const index_T l, const index_T r, const index_T index) {
		if (_l >= r || _r <= l) return;
		if (_l <= l && r <= _r) {
			val[index] = apply(val[index], _tag, r - l);
			tag[index] = attach(tag[index], _tag);
			return;
		}
		push_down(index, r - l);
		index_T mid = (l + r) >> 1;
		if (_l < mid) set(_l, _r, _tag, l, mid, child[index].first);
		if (_r >= mid) set(_l, _r, _tag, mid, r, child[index].second);
		push_up(index);
	}
	value_t get(const index_T _l, const index_T _r, const index_T l,
				const index_T r, const index_T index) {
		if (_l >= r || _r <= l) return e_val();
		if (_l <= l && r <= _r) return val[index];
		push_down(index, r - l);
		index_T mid = (l + r) >> 1;
		if (_l >= mid) return get(_l, _r, mid, r, child[index].second);
		if (_r < mid) return get(_l, _r, l, mid, child[index].first);
		return opt(get(_l, _r, l, mid, child[index].first),
				   get(_l, _r, mid, r, child[index].second));
	}

public:
	inline void set(const index_T _l, const index_T _r, const tag_T _tag) {
		set(_l, _r, _tag, 0, n, 0);
	}
	inline void set(const index_T _pos, const tag_T _tag) {
		set(_pos, _pos + 1, _tag, 0, n, 0);
	}
	inline value_t get(const index_T _l, const index_T _r) {
		return get(_l, _r, 0, n, 0);
	}
	inline value_t get(const index_T _pos) {
		return get(_pos, _pos + 1, 0, n, 0);
	}
	inline value_t get() { return val.front(); }
	LazySegmentTree(index_T n) : n(n) { new_node(root); }
};
