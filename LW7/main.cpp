#include <iostream>
#include <vector>
#include <bitset>

inline constexpr bool chmin(auto& n, const auto& m) { return n > m ? n = m, true : false; }
inline constexpr bool chmax(auto& n, const auto& m) { return n < m ? n = m, true : false; }

class Knapsnack {
private:
    using ll = ptrdiff_t;
    struct data {size_t w, c, ind;};
    ll n, m;
    std::vector<data> d;
public:
    Knapsnack(std::istream& is) {
        is >> n >> m;
        d.reserve(n);
        for (size_t i{0}; i < n; ++i) {
            size_t w, c;
            is >> w >> c;
            d.push_back({w, c, i + 1});
        }
    }
    //RULE 5
    ~Knapsnack() = default;
    Knapsnack(const Knapsnack&) = delete;
    Knapsnack(Knapsnack&&) = delete;
    Knapsnack operator = (const Knapsnack&) = delete;
    Knapsnack operator = (Knapsnack&&) = delete;
    //END RULE 5
    std::pair<ll, std::vector<size_t>> solve() const {
        // dp[k][w] — максимальная сумма для k предметов и веса w
        std::vector dp(n + 1, std::vector<ll>(m + 1, -1));
        std::vector used(n + 1, std::vector<std::bitset<101>>(m + 1));
        dp[0][0] = 0;
        for (ll i{1}; i <= n; ++i) {
            auto [ww, cc, ind] = d[i - 1];
            for (ll k{i}; k > 0; --k) {
                for (ll w{m}; w >= ww; --w) {
                    if (dp[k - 1][w - ww] != -1) {
                        ll cur = dp[k - 1][w - ww] + ll(cc);
                        if (chmax(dp[k][w], cur)) {
                            dp[k][w] = cur;
                            used[k][w] = used[k - 1][w - ww];
                            used[k][w][i - 1] = 1;
                        }
                    }
                }
            }
        }
        ll posk{0}, posw{0};
        ll res{0};
        for (ll k{1}; k <= n; ++k) {
            for (ll w{0}; w <= m; ++w) {
                if (chmax(res, k * dp[k][w])) {
                    posk = k;
                    posw = w;
                }
            }
        }
        std::vector<size_t> poses;
        for (size_t i{0}; i < n; ++i) {
            if (used[posk][posw][i]) poses.push_back(d[i].ind);
        }
        return {res, poses};
    }
};

auto main() -> signed {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Knapsnack k(std::cin);
    auto [res, vec] = k.solve();
    std::cout << res << '\n';
    for (auto i : vec) std::cout << i << ' ';
    return 0;
}