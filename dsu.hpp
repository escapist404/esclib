#include <vector>

template <typename T>
class Dsu {
private:
	T n;
	std::vector<T> fa, siz;

public:
	Dsu(T n) : n(n), fa(n), siz(n) {
		for (T i = 0; i < n; ++i) fa[i] = i, siz[i] = 1;
	}
	bool empty() { return n == 0; }
	T size() { return n; }
	void reset() {
		for (T i = 0; i < n; ++i) fa[i] = i, siz[i] = 1;
	}
	void resize(T _n) : n(_n) { reset(); }
	size_t father(T x) { return fa[x] == x ? x : fa[x] = father(fa[x]); }
	bool is_root(T x) { return father(x) == x; }
	bool merge(T _u, T _v) {
		_u = father(_u), _v = father(_v);
		if (_u == _v) return false;
		if (siz[_u] < siz[_v]) std::swap(_u, _v);
		siz[fa[_u] = _v] += siz[_u];
		return true;
	}
	bool check(T _u, T _v) { return father(_u) == father(_v); }
	T size(T x) { return siz[father(x)]; }
};
