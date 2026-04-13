#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

string bfs(int A, int B) {
    vector<bool> visited(10000, false);
    vector<int> parent(10000, -1);
    vector<char> op(10000, 0);

    queue<int> q;
    q.push(A);
    visited[A] = true;

    while (!q.empty()) {
        int num = q.front();
        q.pop();
        if (num == B) break;

        int d = (num * 2) % 10000;
        if (!visited[d]) {
            visited[d] = true;
            parent[d] = num;
            op[d] = 'D';
            q.push(d);
        }

        int s = (num == 0) ? 9999 : num - 1;
        if (!visited[s]) {
            visited[s] = true;
            parent[s] = num;
            op[s] = 'S';
            q.push(s);
        }

        int l = num / 1000 + (num % 1000) * 10;
        if (!visited[l]) {
            visited[l] = true;
            parent[l] = num;
            op[l] = 'L';
            q.push(l);
        }

        int r = num / 10 + (num % 10) * 1000;
        if (!visited[r]) {
            visited[r] = true;
            parent[r] = num;
            op[r] = 'R';
            q.push(r);
        }
    }

    string command;
    int cur = B;
    while (cur != A) {
        command.push_back(op[cur]);
        cur = parent[cur];
    }
    reverse(command.begin(), command.end());
    return command;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int A, B;
        cin >> A >> B;
        cout << bfs(A, B) << "\n";
    }
    return 0;
}
