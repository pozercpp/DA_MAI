#include <iostream>
#include <queue>
#include <limits>
#include <optional>

using ll = long long;

inline constexpr auto chmin(auto& n, const auto& m) -> bool { return n > m ? n = m, true : false; }

inline constexpr auto chmax(auto& n, const auto& m) -> bool { return n < m ? n = m, true : false; }

template<typename T>
concept Weight = std::totally_ordered<T> && requires(T a, T b) { {a + b} -> std::same_as<T>;};

template<typename T>
concept HasLess = requires(const T& a, const T& b) { { a < b } -> std::convertible_to<bool>; };

template<typename T>
concept HasGreater = requires(const T& a, const T& b) { { a > b } -> std::convertible_to<bool>; };

template<HasLess T> using max_pq = std::priority_queue<T, std::vector<T>, std::less<T>>;

template<HasGreater T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

template <Weight T> struct Edge { 
    T w;
    size_t to;
    inline auto operator <=>(const Edge&) const noexcept = default;
};

template<Weight T> inline auto inf() -> T { return std::numeric_limits<T>::max(); }

template<Weight T> class Graph {
public:
    inline Graph() = default;
    inline explicit Graph(size_t n_) : n(n_ + 1), g(n) {
        for (size_t i{1}; i < n; ++i) add(0, i, 0);
    }
    inline void add(size_t u, size_t v, T w) {  g[u].emplace_back(w, v); }
    [[nodiscard]] inline auto get_distance() -> std::optional<std::vector<std::vector<T>>> {
        auto dp = FordBelman(0);
        if (!dp.has_value()) return std::nullopt;
        auto h = std::move(dp.value());
        for (size_t u{0}; u < n; ++u) {
            for (auto& [w, v] : g[u]) w = w + h[u] - h[v];
        }
        std::vector<std::vector<T>> res;
        res.reserve(n);
        for (size_t v{0}; v < n; ++v) res.push_back(dijkstra(v));
        for (size_t i{0}; i < res.size(); ++i) {
            for (size_t j{0}; j < res[i].size(); ++j) {
                if (res[i][j] == inf<T>()) continue;
                res[i][j] = res[i][j] + h[j] - h[i];
            }
        }
        return res;
    };
private:
    [[nodiscard]] inline auto dijkstra(size_t s) -> std::vector<T> const {
        min_pq<Edge<T>> pq;
        std::vector<T> dp(n, inf<T>());
        dp[s] = T{0};
        pq.emplace(dp[s], s);
        while (!pq.empty()) {
            auto [weight, v] = pq.top();
            pq.pop();
            for (auto [w, to] : g[v]) {
                if (chmin(dp[to], weight + w)) pq.emplace(dp[to], to);
            }
        }
        return dp;
    }
    [[nodiscard]] inline auto FordBelman(size_t s) -> std::optional<std::vector<T>> const {
        std::vector<T> dp(n, inf<T>());
        std::vector<T> ndp(n, inf<T>());
        bool is_relaxed{false};
        dp[0] = 0;
        for (size_t k{1}; k <= n; ++k) {
            is_relaxed = false;
            for (size_t u{0}; u < n; ++u) {
                for (auto [w, v] : g[u]) {
                    if (dp[u] == inf<T>()) continue;
                    if (chmin(ndp[v], dp[u] + w)) is_relaxed = true;
                }
            }
            dp = ndp;
        }
        if (is_relaxed) return std::nullopt;
        return dp;
    }
private:
    size_t n;
    std::vector<std::vector<Edge<T>>> g;
};

inline auto solve() -> void {
    size_t n, m;
    std::cin >> n >> m;
    Graph<ll> g(n);
    for (size_t i{0}; i < m; ++i) {
        size_t u, v;
        ll w;
        std::cin >> u >> v >> w;
        g.add(u, v, w);
    }
    auto res = g.get_distance();
    if (!res.has_value()) {
        std::cout << "Negative cycle";
    } else {
        for (size_t i{1}; i < res.value().size(); ++i) {
            for (size_t j{1}; j < res.value()[i].size(); ++j) {
                if (res.value()[i][j] == inf<ll>()) {
                    std::cout << "inf ";
                } else {
                    std::cout << res.value()[i][j] << ' ';
                }
            }
            std::cout << '\n';
        }
    }
}

auto main() -> signed {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    solve();
    return 0;
}