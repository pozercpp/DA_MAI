#include <iostream>
#include <algorithm>
#include <vector>
#include <string_view>
#include <ranges>
#include <numeric>

namespace cmp {

using uchar = unsigned char;

constexpr int alphabet_size = 256;

constexpr std::string_view alphabet = "$abcdefghijklmnopqrstuvwxyz";

class Compressor {
private:
    inline static auto build_sa(const std::string& s) -> std::vector<int> {
        int n = int(s.size());
        std::vector<int> p(n);
        std::vector<int> c(n), cnt(std::max(alphabet_size, n));
        for (int i = 0; i < n; i++) ++cnt[uchar(s[i])];
        for (int i = 0; i + 1 < alphabet_size; i++) cnt[i + 1] += cnt[i];
        for (int i = 0; i < n; i++) p[--cnt[uchar(s[i])]] = i;
        c[p[0]] = 0;
        int classes = 1;
        for (int i = 1; i < n; i++) {
            if (s[p[i]] != s[p[i - 1]]) ++classes;
            c[p[i]] = classes - 1;
        }
        std::vector<int> pn(n), cn(n);
        for (int h = 0; (1 << h) < n; ++h) {
            for (int i = 0; i < n; i++) {
                pn[i] = p[i] - (1 << h);
                if (pn[i] < 0) pn[i] += n;
            }
            fill(cnt.begin(), cnt.begin() + classes, 0);
            for (int i = 0; i < n; i++) ++cnt[c[pn[i]]];
            for (int i = 1; i < classes; i++) cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; i--) p[--cnt[c[pn[i]]]] = pn[i];
            cn[p[0]] = 0;
            classes = 1;
            for (int i = 1; i < n; i++) {
                int mid1 = p[i] + (1 << h);
                if (mid1 >= n) mid1 -= n;
                int mid2 = p[i - 1] + (1 << h);
                if (mid2 >= n) mid2 -= n;
                if (c[p[i]] != c[p[i - 1]] || c[mid1] != c[mid2]) ++classes;
                cn[p[i]] = classes - 1;
            }
            c = cn;
            if (classes == n) break;
        }
        return p;
    }
    inline static auto make_bwt_string(const std::string& str) -> std::string {
        std::string res;
        int n = int(str.size());
        res.reserve(n);
        auto suff = build_sa(str);
        for (int i{0}; i < n; ++i) res.push_back(str[(suff[i] + n - 1) % n]);
        return res;
    }
    inline static auto rle_compressor(const std::vector<int>& v) -> std::vector<std::pair<int, int>> {
        int i{0}, n = int(v.size());
        std::vector<std::pair<int, int>> res;
        while (i < n) {
            int cnt{1};
            while (i + cnt < n && v[i] == v[i + cnt]) ++cnt;
            res.emplace_back(cnt, v[i]);
            i += cnt;
        }
        return res;
    }
    inline static auto mwt(const std::string& str) -> std::vector<int> {
        std::vector<char> alph(alphabet.begin(), alphabet.end());
        int n = int(str.size());
        std::vector<int> res;
        res.reserve(n);
        for (int i{0}; i < n; ++i) {
            auto pos = std::ranges::find(alph, str[i]);
            res.push_back(pos - alph.begin());
            alph.erase(pos);
            alph.insert(alph.begin(), str[i]);
        }
        return res;
    }
public:
    inline static auto compress(const std::string& str) -> std::vector<std::pair<int, int>>  { return rle_compressor(mwt(make_bwt_string(str + '$'))); }
};

class Decompresser {
private:
    inline static auto rle_decompressor(const std::vector<std::pair<int, int>>& rle) -> std::vector<int> {
        std::vector<int> res;
        for (auto [cnt, elem] : rle) {
            for (int i{0}; i < cnt; ++i) res.push_back(elem);
        }
        return res;
    }
    inline static auto make_string_of_mwt(const std::vector<int>& mwt) -> std::string {
        std::vector<char> alph(alphabet.begin(), alphabet.end());
        int n = int(mwt.size());
        std::string res;
        res.reserve(n);
        for (int i{0}; i < n; ++i) {
            auto pos = mwt[i];
            char c = alph[pos];
            res += c;
            alph.erase(alph.begin() + pos);
            alph.insert(alph.begin(), c);
        }
        return res;
    }
    inline static auto make_string_of_bwt(const std::string& str) -> std::string {
        int n = int(str.size());
        std::vector<int> cnt(alphabet_size);
        for (uchar c : str) ++cnt[c];
        std::exclusive_scan(cnt.begin(), cnt.end(), cnt.begin(), 0);
        std::vector<int> cnt_pos(alphabet_size);
        std::vector<int> pos(n);
        for (int i{0}; i < n; ++i) {
            uchar c = str[i];
            pos[i] = cnt[c] + cnt_pos[c];
            ++cnt_pos[c];
        }
        std::string res;
        res.reserve(n);
        auto cur = std::ranges::find(str, '$') - str.begin();
        for (int i{0}; i < n; ++i) {
            res.push_back(str[cur]);
            cur = pos[cur];
        }
        std::ranges::reverse(res);
        res.pop_back();
        return res;
    }
public:
    inline static auto decompress(const std::vector<std::pair<int, int>>& codes) -> std::string { return make_string_of_bwt(make_string_of_mwt(rle_decompressor(codes))); }
};
};

auto main() -> signed {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string command;
    std::cin >> command;
    if (command == "compress") {
        std::string text;
        std::cin >> text;
        auto res = cmp::Compressor::compress(text);
        for (auto [i, j] : res) std::cout << i << ' ' << j << '\n';
    } else {
        std::vector<std::pair<int, int>> codes;
        int i, j;
        while (std::cin >> i >> j) codes.emplace_back(i, j);
        auto res = cmp::Decompresser::decompress(codes);
        std::cout << res << '\n';
    }
    return 0;
}