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

bool union_parent(int a, int b) {
    a = find_parent(a);
    b = find_parent(b);
    if (a == b) return true;
    if (a < b) parent[b] = a;
    else parent[a] = b;
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    parent.assign(N, 0);
    for (int i = 0; i < N; ++i) parent[i] = i;

    for (int i = 1; i <= M; ++i) {
        int a, b;
        cin >> a >> b;
        if (union_parent(a, b)) {
            cout << i << "\n";
            return 0;
        }
    }

    cout << 0 << "\n";
    return 0;
}
