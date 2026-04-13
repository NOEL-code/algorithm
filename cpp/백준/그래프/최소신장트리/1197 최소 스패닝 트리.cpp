#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
using namespace std;

vector<int> parent;

int find_parent(int x) {
    if (parent[x] != x) parent[x] = find_parent(parent[x]);
    return parent[x];
}

void union_parent(int a, int b) {
    a = find_parent(a);
    b = find_parent(b);
    if (a < b) parent[b] = a;
    else parent[a] = b;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, E;
    cin >> V >> E;

    parent.assign(V + 1, 0);
    vector<tuple<int, int, int>> edges;
    long long result = 0;

    for (int i = 0; i < E; ++i) {
        int a, b, cost;
        cin >> a >> b >> cost;
        edges.push_back({cost, a, b});
    }

    for (int i = 1; i <= V; ++i) parent[i] = i;

    sort(edges.begin(), edges.end());

    for (auto [cost, a, b] : edges) {
        if (find_parent(a) != find_parent(b)) {
            union_parent(a, b);
            result += cost;
        }
    }

    cout << result << "\n";
    return 0;
}
