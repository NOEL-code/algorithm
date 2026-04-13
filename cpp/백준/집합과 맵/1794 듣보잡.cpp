#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    set<string> never_heard;
    set<string> never_seen;

    for (int i = 0; i < N; ++i) {
        string name;
        cin >> name;
        never_heard.insert(name);
    }
    for (int i = 0; i < M; ++i) {
        string name;
        cin >> name;
        never_seen.insert(name);
    }

    vector<string> never_heard_seen;
    for (const auto& name : never_heard) {
        if (never_seen.find(name) != never_seen.end()) {
            never_heard_seen.push_back(name);
        }
    }

    cout << never_heard_seen.size() << "\n";
    for (const auto& name : never_heard_seen) {
        cout << name << "\n";
    }
    return 0;
}
