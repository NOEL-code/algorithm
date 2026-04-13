#include <iostream>
#include <queue>
#include <string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    long long A, B;
    cin >> A >> B;

    queue<pair<long long, int>> q;
    q.push({A, 1});

    while (!q.empty()) {
        auto [position, count] = q.front();
        q.pop();

        if (position == B) {
            cout << count << "\n";
            return 0;
        }

        long long new_position = position * 2;
        if (new_position <= B) {
            q.push({new_position, count + 1});
        }

        long long appended = position * 10 + 1;
        if (appended <= B) {
            q.push({appended, count + 1});
        }
    }

    cout << -1 << "\n";
    return 0;
}
