#include <iostream>
#include <deque>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int> A(N), B(N);
    for (int i = 0; i < N; ++i) cin >> A[i];
    for (int i = 0; i < N; ++i) cin >> B[i];

    int M;
    cin >> M;
    vector<int> C(M);
    for (int i = 0; i < M; ++i) cin >> C[i];

    deque<int> queue;
    for (int i = 0; i < N; ++i) {
        if (A[i] == 0) queue.push_back(B[i]);
    }

    for (int i = 0; i < M; ++i) {
        queue.push_front(C[i]);
        cout << queue.back() << " ";
        queue.pop_back();
    }
    cout << "\n";
    return 0;
}
