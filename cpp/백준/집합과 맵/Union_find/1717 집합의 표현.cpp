#include <iostream>
#include <vector>
using namespace std;

vector<int> parent;

int find_parent(int x) {
    if (parent[x] != x) {
        parent[x] = find_parent(parent[x]);
    }
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

    int n, m;
    cin >> n >> m;

    parent.assign(n + 1, 0);
    for (int i = 0; i <= n; ++i) parent[i] = i;

    for (int i = 0; i < m; ++i) {
        int oper, a, b;
        cin >> oper >> a >> b;
        if (oper == 0) {
            union_parent(a, b);
        } else {
            if (find_parent(a) == find_parent(b)) cout << "YES" << "\n";
            else cout << "NO" << "\n";
        }
    }

    return 0;
}
