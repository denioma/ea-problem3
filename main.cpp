#include <iostream>
#include <vector>

class Node {
public:
    int time;
    bool found_successor;
    std::vector<int> requires;
    std::vector<int> required_by;

    Node() {}

    void depends_of(int id) { requires.push_back(id); }

    void is_required(int id) { required_by.push_back(id); }

    bool is_start() { return requires.empty(); }

    bool is_end() { return required_by.empty(); }

    ~Node() {}
};

bool dfs(std::vector<Node>& graph, std::vector<bool>& marked, int v) {
    marked[v] = true;
    for (const auto& u : graph[v].required_by) {
        if (marked[u]) return false;
        dfs(graph, marked, u);
    }

    return true;
}

// Not sure if needed in the future
int start = -1, end = -1;

bool checkPipeline(std::vector<Node>& graph) {
    // Use DFS for cycle checking and for connected graph
    std::vector<bool> marked(graph.size(), false);

    // Find more than 1 initial node
    bool found_start = false, found_end = false;

    for (int i = 0; i < (int) graph.size(); i++) {
        Node& node = graph[i];
        if (node.is_start()) {
            if (found_start) return false;
            found_start = true;
            start = i;
            if (!dfs(graph, marked, i)) return false;
        }

        if (node.is_end()) {
            if (found_end) return false;
            found_end = true;
            end = i;
        }
    }

    return true;
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int n, t, d, id, stat;
    std::cin >> n;
    std::vector<Node> graph(n);
    for (int i = 0; i < n; i++) {
        std::cin >> t >> d;
        graph[i].time = t;
        for (int j = 0; j < d; j++) {
            std::cin >> id;
            graph[i].depends_of(id-1);
            graph[id-1].is_required(i);
        }
    }
    std::cin >> stat;

    if (!checkPipeline(graph)) {
        std::cout << "INVALID\n";
        return 0;
    }

    switch (stat) {
        case 0:
            std::cout << "VALID\n";
            break;
        default:
            // TODO implement 3 stats
            std::cout << "Stat " << stat << " not implemented\n";
            break;
    }

    return 0;
}

