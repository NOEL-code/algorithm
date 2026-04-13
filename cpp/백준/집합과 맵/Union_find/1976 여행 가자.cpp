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

    int N, M;
    cin >> N >> M;

    parent.assign(N, 0);
    for (int i = 0; i < N; ++i) parent[i] = i;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            int oper;
            cin >> oper;
            if (oper == 1) union_parent(i, j);
        }
    }

    int a, b, c;
    cin >> a >> b >> c;
    --a; --b; --c;

    if (find_parent(a) == find_parent(b) && find_parent(b) == find_parent(c)) {
        cout << "YES" << "\n";
    } else {
        cout << "NO" << "\n";
    }

    return 0;
}
