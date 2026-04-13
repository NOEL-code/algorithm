#include <iostream>
#include <vector>
using namespace std;

vector<int> parent;

int find(int x) {
    if (parent[x] == x) return x;
    return parent[x] = find(parent[x]);
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a != b) parent[a] = b;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    int truth_count;
    cin >> truth_count;

    vector<int> truth_people(truth_count);
    for (int i = 0; i < truth_count; ++i) cin >> truth_people[i];

    parent.assign(N + 1, 0);
    for (int i = 0; i <= N; ++i) parent[i] = i;

    vector<vector<int>> parties(M);
    for (int i = 0; i < M; ++i) {
        int k;
        cin >> k;
        parties[i].resize(k);
        for (int j = 0; j < k; ++j) cin >> parties[i][j];
        for (int j = 1; j < k; ++j) unite(parties[i][0], parties[i][j]);
    }

    int answer = 0;
    for (int i = 0; i < M; ++i) {
        bool ok = true;
        for (int p : parties[i]) {
            for (int t : truth_people) {
                if (find(p) == find(t)) {
                    ok = false;
                    break;
                }
            }
            if (!ok) break;
        }
        if (ok) ++answer;
    }

    cout << answer << "\n";
    return 0;
}
