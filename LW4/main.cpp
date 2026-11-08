#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <sstream>

class Aho final {
private:
    struct Node;
    using nodeptr = Node*;
    struct Node final {
        nodeptr link = nullptr;
        std::map<std::string, nodeptr> next;
        std::vector<size_t> ind;
        static void add(nodeptr& node, const std::vector<std::string>& vec, size_t i) {
            auto v = node;
            for (const auto& s : vec) {
                if (v->next.find(s) == v->next.end()) {
                    v->next[s] = new Node();
                }
                v = v->next[s];
            }
            v->ind.push_back(i);
        }
        static void init(nodeptr& node) {
            std::queue<nodeptr> q;
            for (auto& [str, ptr] : node->next) {
                q.push(ptr);
                ptr->link = node;
            }
            while (!q.empty()) {
                auto v = q.front();
                q.pop();
                for (auto& [str, child] : v->next) {
                    auto ptr = v->link;
                    while (ptr != node && ptr->next.find(str) == ptr->next.end()) {
                        ptr = ptr->link;
                    }
                    child->link = (ptr->next.find(str) == ptr->next.end() ? node : ptr->next[str]);
                    for (size_t i : child->link->ind) {
                        child->ind.push_back(i);
                    }
                    q.push(child);
                }
            }
        }
        static std::vector<std::pair<size_t, size_t>> search(nodeptr& node, const std::vector<std::string>& vec) {
            auto ptr = node;
            std::vector<std::pair<size_t, size_t>> res;
            for (size_t i = 0; i < vec.size(); ++i) {
                const auto& str = vec[i];
                while (ptr != node && ptr->next.find(str) == ptr->next.end()) {
                    ptr = ptr->link;
                }
                ptr = (ptr->next.find(str) != ptr->next.end() ? ptr->next[str] : node);
                for (size_t id : ptr->ind) {
                    res.emplace_back(id, i);
                }
            }
            return res;
        }
    };
    nodeptr root;
public:
    Aho() {
        root = new Node();
        root->link = root;
    }
    void add(const std::vector<std::string>& vec, size_t i) {
        Node::add(root, vec, i);
    }
    void init() {
        Node::init(root);
    }
    std::vector<std::pair<size_t, size_t>> search(const std::vector<std::string>& vec) {
        return Node::search(root, vec);
    }
};

signed main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::vector<size_t> lens;
    std::string line;
    Aho a;
    size_t pat = 0;
    while (getline(std::cin, line) && !line.empty()) {
        std::vector<std::string> words;
        std::stringstream ss(line);
        std::string tmp;
        while (ss >> tmp) {
        	for (char& c : tmp) {
          		c = std::tolower(c);
            }
            words.push_back(std::move(tmp));
        }
        lens.push_back(words.size());
        a.add(words, ++pat);
    }
    a.init();
    std::vector<std::string> text_words;
    std::vector<std::pair<size_t, size_t>> pos;
    size_t num_text = 0;
    while (getline(std::cin, line)) {
        ++num_text;
        std::stringstream ss(line);
        std::string word;
        size_t word_cnt = 0;
        while (ss >> word) {
            ++word_cnt;
        	for (char& c : word) {
            	c = std::tolower(c);
            }
            text_words.push_back(std::move(word));
            pos.emplace_back(num_text, word_cnt);
        }
    }
    auto res = a.search(text_words);
    for (const auto& [ind, end] : res) {
        size_t len = lens[ind - 1];
        size_t pos_ind = end + 1 - len; 
        std::cout << pos[pos_ind].first << ", " << pos[pos_ind].second << ", " << ind << '\n';
    }
    return 0;
}