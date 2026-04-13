#include <iostream>
#include <vector>
#include <string>
#include <deque>
using namespace std;

void rotate_wheel(deque<int>& w, int d) {
    if (d == 1) {
        int last = w.back();
        w.pop_back();
        w.push_front(last);
    } else {
        int first = w.front();
        w.pop_front();
        w.push_back(first);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<deque<int>> wheel(4);
    for (int i = 0; i < 4; ++i) {
        string s;
        cin >> s;
        for (char c : s) wheel[i].push_back(c - '0');
    }

    int K;
    cin >> K;

    for (int k = 0; k < K; ++k) {
        int num, direction;
        cin >> num >> direction;
        num -= 1;

        vector<int> dirs(4, 0);
        dirs[num] = direction;

        for (int i = num - 1; i >= 0; --i) {
            if (wheel[i][2] != wheel[i + 1][6]) {
                dirs[i] = -dirs[i + 1];
            } else {
                break;
            }
        }

        for (int i = num + 1; i < 4; ++i) {
            if (wheel[i - 1][2] != wheel[i][6]) {
                dirs[i] = -dirs[i - 1];
            } else {
                break;
            }
        }

        for (int i = 0; i < 4; ++i) {
            if (dirs[i] != 0) {
                rotate_wheel(wheel[i], dirs[i]);
            }
        }
    }

    int point = 0;
    for (int i = 0; i < 4; ++i) {
        if (wheel[i][0] == 1) {
            point += (1 << i);
        }
    }

    cout << point << "\n";
    return 0;
}
