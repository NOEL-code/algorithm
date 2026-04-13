#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;

const int INF = 1e9;

int N, E;
vector<vector<pair<int, int>>> graph;

vector<int> dijkstra(int start) {
    vector<int> distance(N + 1, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> queue;
    queue.push({0, start});
    distance[start] = 0;

    while (!queue.empty()) {
        auto [dist, now] = queue.top();
        queue.pop();

        if (distance[now] < dist) continue;

        for (auto [next_node, next_dist] : graph[now]) {
            int cost = dist + next_dist;
            if (cost < distance[next_node]) {
                distance[next_node] = cost;
                queue.push({cost, next_node});
            }
        }
    }
    return distance;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> E;
    graph.assign(N + 1, {});

    for (int i = 0; i < E; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        graph[a].push_back({b, c});
        graph[b].push_back({a, c});
    }

    int base1, base2;
    cin >> base1 >> base2;

    vector<int> distance_from_start = dijkstra(1);
    vector<int> distance_from_base1 = dijkstra(base1);
    vector<int> distance_from_base2 = dijkstra(base2);

    long long path1 = (long long)distance_from_start[base1] + distance_from_base1[base2] + distance_from_base2[N];
    long long path2 = (long long)distance_from_start[base2] + distance_from_base2[base1] + distance_from_base1[N];

    long long result = min(path1, path2);
    if (result >= INF) cout << -1 << "\n";
    else cout << result << "\n";
    return 0;
}
