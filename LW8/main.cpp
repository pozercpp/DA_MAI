#include <iostream>
#include <queue>

class graph {
public:
    inline graph() = default;
    inline graph(size_t n_) : graph() {
        n = n_;
        g.reserve(n);
    }
    inline void add(size_t u, size_t v) { g[u].push_back(v); }
    inline graph(size_t n_, size_t m) : graph(n_) {
        g.reserve(n);
        for (size_t i{0}; i < m; ++i) {
            size_t u, v;
            std::cin >> u >> v;
            --u, --v;
            add(u, v);
        }
    }
    [[nodiscard]] inline std::pair<bool, std::vector<size_t>> top_sort() const {
        std::vector<size_t> in(n);
        for (size_t u{0}; u < n; ++u) {
            for (auto v : g[u]) ++in[v];
        }
        std::vector<size_t> res;
        std::queue<size_t> q;
        for (size_t i{0}; i < n; ++i) {
            if (!in[i]) q.push(i);
        }
        while (!q.empty()) {
            auto u = q.front();
            res.push_back(u);
            q.pop();
            for (auto v : g[u]) {
                --in[v];
                if (!in[v]) q.push(v);
            }
        }
        return {res.size() == n, res};
    }
    inline size_t size() const noexcept { return n; }
private:
    size_t n;
    std::vector<std::vector<size_t>> g;
};

inline void solve() {
    size_t n, m;
    std::cin >> n >> m;
    graph g(n, m);
    auto [ok, res] = g.top_sort();
    if (!ok) {
        std::cout << -1;
    } else {
        for (size_t i{0}; i + 1 < n; ++i) {
            std::cout << res[i] + 1 << ' ' << res[i + 1] + 1 << '\n';
        }
    }
}

auto main() -> signed {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
	solve();
    return 0;
}