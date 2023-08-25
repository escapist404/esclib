#include <queue>
#include <vector>

struct Edge {
	int to, nxt;
	Edge(const int to = 0, const int nxt = -1) : to(to), nxt(nxt) {}
};

template <typename T>
struct Flow : public Edge {
	T capicity, flow;
	Flow(const int to = 0, const int nxt = -1, const T capicity = 0, const T flow = 0)
		: Edge(to, nxt), capicity(capicity), flow(flow) {}
};

class Graph {
public:
	int n, m;
	std::vector<int> head;
	int cnt;
	Graph(int n, int m) : n(n), m(m), head(n, -1), cnt(-1) {}
};

template <typename T>
class NetworkFlow : public Graph {
public:
	int s, t;
	T inf;
	std::vector<int> dep, cur;

	std::vector<Flow<T>> ed;

	inline void add_edge(const int from = 0, const int to = 0, const T capicity = 0, const T flow = 0) {
		ed[++cnt].to = to, ed[cnt].nxt = head[from], head[from] = cnt, ed[cnt].capicity = capicity,
		ed[cnt].flow = flow;
	}

	NetworkFlow(int n, int m, int s, int t, T inf)
		: Graph(n, m), s(s), t(t), inf(inf), dep(n), cur(n, -1), ed(m) {}

private:
	auto bfs(int s, int t) {
		std::queue<int> q;
		while (q.size()) q.pop();
		dep.assign(n, 0);
		dep[s] = 1;
		q.push(s);
		while (q.size()) {
			int x = q.front();
			q.pop();
			for (int i = head[x]; ~i; i = ed[i].nxt) {
				if ((!dep[ed[i].to]) && (ed[i].capicity > ed[i].flow)) {
					dep[ed[i].to] = dep[x] + 1;
					q.push(ed[i].to);
				}
			}
		}
		return dep[t] > 0;
	}

	T dfs(int x, int t, T flow) {
		if (x == t || (!flow)) return flow;
		T ret = 0;
		for (int& i = cur[x]; ~i; i = ed[i].nxt) {
			T d;
			if ((dep[ed[i].to] == dep[x] + 1) &&
				(d = dfs(ed[i].to, t, std::min(flow - ret, ed[i].capicity - ed[i].flow)))) {
				ret += d;
				ed[i].flow += d;
				ed[i ^ 1].flow -= d;
				if (ret == flow) return ret;
			}
		}
		return ret;
	}

public:
	T dinic() {
		T max_flow = 0;
		while (bfs(s, t)) {
			cur = head;
			max_flow += dfs(s, t, inf);
		}
		return max_flow;
	}
};
