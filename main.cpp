#include <iostream>
#include <vector>
#include <algorithm>

class Node {
public:
    int time;
    bool found_successor;
    std::vector<int> requires;
    std::vector<int> required_by;

    Node() {}

    void depends_on(int id) { requires.push_back(id); }

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

bool check_pipeline(std::vector<Node>& graph) {
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

    if (start == -1 || end == -1) return false;
    return true;
}

void topological_sorting(std::vector<Node>& graph) {
    int total = 0;
    std::vector<int> sorted;
    std::vector<int> deg(graph.size());
    for (size_t i = 0; i < graph.size(); i++) deg[i] = (int) graph[i].requires.size();

    while (sorted.size() != graph.size()) {
        for (int i = 0; i < (int) deg.size(); i++) {
            if (deg[i] != 0) continue;

            Node& node = graph[i];
            for (const auto& id : node.required_by) {
                deg[id]--;
            }
            sorted.push_back(i+1);
            total += node.time;
            deg[i]--;
            break;
        }
    }

    std::cout << total << "\n";
    for (const auto& node : sorted) std::cout << node << "\n";
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
            id--;

            if (id == i) {
                std::cout << "INVALID\n";
                return 0;
            }

            graph[i].depends_on(id);
            graph[id].is_required(i);
        }
    }
    std::cin >> stat;

    if (!check_pipeline(graph)) {
        std::cout << "INVALID\n";
        return 0;
    }

    switch (stat) {
        case 0:
            std::cout << "VALID\n";
            break;
        case 1:
            topological_sorting(graph);
            break;
        case 2:
            // TODO
            std::cout << "TODO\n";
            break;
        case 3:
            // TODO
            std::cout << "TODO\n";
            break;
    }

    return 0;
}

