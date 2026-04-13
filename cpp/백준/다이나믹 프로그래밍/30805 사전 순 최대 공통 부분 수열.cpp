#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> A(N);
    for (int i = 0; i < N; ++i) cin >> A[i];

    int M;
    cin >> M;
    vector<int> B(M);
    for (int i = 0; i < M; ++i) cin >> B[i];

    int ia = 0, ib = 0;
    vector<int> ans;

    const int MAX_VAL = 100;

    while (ia < N && ib < M) {
        int chosen = -1;
        int next_ia = -1, next_ib = -1;

        for (int v = MAX_VAL; v >= 1; --v) {
            int pa = -1;
            for (int i = ia; i < N; ++i) {
                if (A[i] == v) {
                    pa = i;
                    break;
                }
            }
            if (pa == -1) continue;

            int pb = -1;
            for (int j = ib; j < M; ++j) {
                if (B[j] == v) {
                    pb = j;
                    break;
                }
            }
            if (pb == -1) continue;

            chosen = v;
            next_ia = pa + 1;
            next_ib = pb + 1;
            break;
        }

        if (chosen == -1) break;

        ans.push_back(chosen);
        ia = next_ia;
        ib = next_ib;
    }

    cout << ans.size() << "\n";
    if (!ans.empty()) {
        for (size_t i = 0; i < ans.size(); ++i) {
            cout << ans[i];
            if (i + 1 < ans.size()) cout << ' ';
        }
        cout << "\n";
    }

    return 0;
}
