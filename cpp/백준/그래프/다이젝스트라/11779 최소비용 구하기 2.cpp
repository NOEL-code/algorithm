#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;

const int INF = 1e9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N;
    cin >> M;

    vector<vector<pair<int, int>>> graph(N + 1);
    vector<int> distance(N + 1, INF);
    vector<int> prev(N + 1, -1);

    for (int i = 0; i < M; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        graph[a].push_back({b, c});
    }

    int start, endNode;
    cin >> start >> endNode;

    distance[start] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> queue;
    queue.push({0, start});

    while (!queue.empty()) {
        auto [dist, now] = queue.top();
        queue.pop();

        if (distance[now] < dist) continue;

        for (auto [next_node, next_cost] : graph[now]) {
            int cost = dist + next_cost;
            if (distance[next_node] > cost) {
                distance[next_node] = cost;
                prev[next_node] = now;
                queue.push({cost, next_node});
            }
        }
    }

    cout << distance[endNode] << "\n";

    vector<int> path;
    int current = endNode;
    while (current != -1) {
        path.push_back(current);
        current = prev[current];
    }
    reverse(path.begin(), path.end());

    cout << path.size() << "\n";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i + 1 < path.size()) cout << " ";
    }
    cout << "\n";
    return 0;
}
