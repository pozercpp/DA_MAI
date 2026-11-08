#include <iostream>
#include <vector>
#include <map>
#include <string>

class SuffixTree final {
private:
    struct Node;
    using nodeptr = Node*;
    struct Node final {
        std::map<char, nodeptr> edges;
        nodeptr par{nullptr}, link{nullptr};
        size_t first{0}, len{std::string::npos};
        Node(nodeptr parent, size_t f, size_t l) : par(parent), first(f), len(l) {}
        Node() = default;
    };
    nodeptr root, active_node;
    std::string text;
    size_t active_len{0}, active_char{0}, rem{0};
    size_t edge_len(nodeptr node, size_t pos) const {
        if (node == root) return 0;
        return (node->len == std::string::npos ? pos - node->first + 1 : node->len);
    }
    void del(nodeptr node) noexcept {
        for (auto& [c, nd] : node->edges) del(nd);
        delete node;
    }
public:
    SuffixTree() {
        root = new Node(nullptr, 0, 0);
        root->link = root;
        root->par = root;
        active_node = root;
    }
    void add(char c) {
        text += c;
        size_t pos = text.size() - 1;
        ++rem;
        nodeptr last = nullptr;
        while (rem) {
            if (!active_len) active_char = pos;
            auto it = active_node->edges.find(text[active_char]);
            if (it == active_node->edges.end()) {
                active_node->edges[text[active_char]] = new Node(active_node, pos, std::string::npos);
                if (last) last->link = active_node;
                last = active_node;
            } else {
                nodeptr nxt = it->second;
                size_t len_e = edge_len(nxt, pos);
                if (active_len >= len_e) {
                    active_char += len_e;
                    active_len -= len_e;
                    active_node = nxt;
                    continue;
                }
                if (text[nxt->first + active_len] == c) {
                    ++active_len;
                    if (last) last->link = active_node;
                    break;
                }
                nodeptr split = new Node(active_node, nxt->first, active_len);
                active_node->edges[text[active_char]] = split; 
                split->edges[c] = new Node(split, pos, std::string::npos);
                nxt->first += active_len;
                if (nxt->len != std::string::npos) nxt->len -= active_len;
                nxt->par = split;
                split->edges[text[nxt->first]] = nxt;
                if (last) last->link = split;
                last = split;
            }
            --rem;
            if (active_node == root && active_len > 0) {
                --active_len;
                active_char = pos - rem + 1;
            } else {
                active_node = active_node->link ? active_node->link : root;
            }
        }
    }
    SuffixTree(const std::string& s) : SuffixTree() {
        for (char c : s) add(c);
        add('#');
    }
    std::vector<size_t> calc_matches(const std::string& query) {
        std::vector<size_t> ms(query.size());
        nodeptr act_node = root;
        size_t act_len = 0, act_char = 0, total_len = 0;
        for (size_t i = 0; i < query.size(); ++i) {
            while (i + total_len < query.size()) {
                if (!act_len) act_char = i + total_len;
                auto it = act_node->edges.find(query[act_char]);
                if (it == act_node->edges.end()) break;
                nodeptr nxt = it->second;
                size_t elen = edge_len(nxt, text.size() - 1);
                if (text[nxt->first + act_len] != query[i + total_len]) break;
                ++act_len;
                ++total_len;
                if (act_len == elen) {
                    act_node = nxt;
                    act_len = 0;
                }
            }
            ms[i] = total_len;
            if (total_len > 0) {
                --total_len;
                if (act_node == root && act_len > 0) {
                    --act_len;
                    act_char = i + 1;
                } else {
                    act_node = act_node->link ? act_node->link : root;
                }
                while (act_len > 0) {
                    nodeptr nxt = act_node->edges[query[act_char]];
                    size_t elen = edge_len(nxt, text.size() - 1);
                    if (act_len < elen) break;
                    act_node = nxt;
                    act_len -= elen;
                    act_char += elen;
                }
            }
        }
        return ms;
    }
    ~SuffixTree() noexcept {
        del(root);
    }
};

auto main() -> signed {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string pat, txt;
    std::cin >> pat >> txt;
    SuffixTree tr(pat);
    auto ms = tr.calc_matches(txt);
    for (size_t i = 0; i < ms.size(); ++i) {
        if (ms[i] == pat.size()) std::cout << i + 1 << '\n';
    }
    return 0;
}