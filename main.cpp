#include <iostream>
#include <vector>
#include <algorithm>

class Node {
public:
    int time;
    std::vector<int> requires;
    std::vector<int> required_by;

    Node() {}

    ~Node() {}

    void depends_on(int id) { requires.push_back(id); }

    void is_required(int id) { required_by.push_back(id); }

    bool is_start() { return requires.empty(); }

    bool is_end() { return required_by.empty(); }
};

std::vector<bool> path;
bool cycle;
void dfs(std::vector<Node>& graph, std::vector<bool>& marked, int v) {
    marked[v] = true;
    path[v] = true;
    for (const auto& u : graph[v].required_by) {
        if (path[u]) {
            cycle = true;
            return;
        } else if (!marked[u]) {
            dfs(graph, marked, u);
        }
    }
    path[v] = false;
}

// Not sure if needed in the future
int start = -1, end = -1;

bool check_pipeline(std::vector<Node>& graph) {
    // Use DFS for cycle checking and for connected graph
    std::vector<bool> marked(graph.size(), false);
    path = std::vector<bool>(graph.size(), false);
    cycle = false;

    // Find more than 1 initial node
    bool found_start = false, found_end = false;

    for (int i = 0; i < (int) graph.size(); i++) {
        Node& node = graph[i];
        if (node.is_start()) {
            if (found_start) return false;
            found_start = true;
            start = i;
            dfs(graph, marked, i);
            if (cycle) return false;
        }

        if (node.is_end()) {
            if (found_end) return false;
            found_end = true;
            end = i;
        }
    }

    for (const auto& mark : marked) {
        if (!mark) return false;
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

std::vector<int> dp2;
int parallel_pipeline(std::vector<Node>& graph, int idx) {
    if (dp2[idx] != -1) return dp2[idx];

    Node& node = graph[idx];
    int total = node.time;
    int max = 0, temp;
    for (const auto& task : node.required_by) {
        temp = parallel_pipeline(graph, task);
        if (max < temp) max = temp;
    }

    total += max;
    dp2[idx] = total;

    return total;
}


int count_up(std::vector<Node>& graph, int i) {
    int total = 0;
    path[i] = true;
    for (const auto& node : graph[i].requires) {
        if (!path[node]) {
            total += count_up(graph, node) + 1;
        }
    }
    return total;
}


int count_down(std::vector<Node>& graph, int i) {
    int total = 0;
    path[i] = true;
    for (const auto& node : graph[i].required_by) {
        if (!path[node]) {
            total += count_down(graph, node) + 1;
        }
    }
    return total;
}

inline
void checkBottleneck(std::vector<Node>& graph, int i) {
    path = std::vector<bool>(graph.size(), false);    
    int up = count_up(graph, i), down = count_down(graph, i);

    if (up + down + 1 == (int) graph.size())
        std::cout << i + 1 << "\n";
}

void bottlenecks(std::vector<Node>& graph) {
    std::vector<int> deg(graph.size());
    for (size_t i = 0; i < graph.size(); i++) deg[i] = (int) graph[i].requires.size();

    for (int k = 0; k < (int) graph.size(); k++) {
        for (int i = 0; i < (int) deg.size(); i++) {
            if (deg[i] != 0) continue;
            
            Node& node = graph[i];
            for (const auto& id : node.required_by) {
                deg[id]--;
            }
            
            deg[i]--;
            if (i == start || i == end) {
                std::cout << i + 1 << "\n";
                continue;
            }
            checkBottleneck(graph, i);
            break;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int n, t, d, id, stat;
    while (std::cin >> n) {
        bool loop = false;
        std::vector<Node> graph(n);
        for (int i = 0; i < n; i++) {
            std::cin >> t >> d;
            graph[i].time = t;
            for (int j = 0; j < d; j++) {
                std::cin >> id;
                id--;

                if (id == i) {
                    loop = true;
                    break;
                }

                graph[i].depends_on(id);
                graph[id].is_required(i);
            }
        }
        std::cin >> stat;

        if (loop || !check_pipeline(graph)) {
            std::cout << "INVALID\n";
            continue;
        }

        switch (stat) {
            case 0:
                std::cout << "VALID\n";
                break;
            case 1:
                topological_sorting(graph);
                break;
            case 2:
                dp2 = std::vector<int> (graph.size(), -1);
                std::cout << parallel_pipeline(graph, start) << "\n";
                break;
            case 3:
                // TODO
                bottlenecks(graph);
                break;
            default:
                break;
        }
    }

    return 0;
}

