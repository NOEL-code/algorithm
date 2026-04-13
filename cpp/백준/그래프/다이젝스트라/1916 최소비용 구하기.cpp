#include <iostream>
#include <vector>
#include <queue>
#include <utility>
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

    for (int i = 0; i < M; ++i) {
        int a, b, cost;
        cin >> a >> b >> cost;
        graph[a].push_back({b, cost});
    }

    int start, endNode;
    cin >> start >> endNode;

    distance[start] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> q;
    q.push({0, start});

    while (!q.empty()) {
        auto [dist, now] = q.top();
        q.pop();

        if (distance[now] < dist) continue;

        for (auto [next_node, next_cost] : graph[now]) {
            int cost = dist + next_cost;
            if (cost < distance[next_node]) {
                distance[next_node] = cost;
                q.push({cost, next_node});
            }
        }
    }

    cout << distance[endNode] << "\n";
    return 0;
}
