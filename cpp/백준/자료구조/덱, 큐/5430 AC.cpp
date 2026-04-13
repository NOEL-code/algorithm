#include <iostream>
#include <deque>
#include <string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        string p;
        int n;
        string arr;
        cin >> p >> n >> arr;

        deque<string> queue;
        if (n > 0) {
            string body = arr.substr(1, arr.size() - 2);
            string cur;
            for (char c : body) {
                if (c == ',') {
                    queue.push_back(cur);
                    cur.clear();
                } else {
                    cur += c;
                }
            }
            if (!cur.empty()) queue.push_back(cur);
        }

        bool reverse_flag = false;
        bool error_flag = false;

        for (char cmd : p) {
            if (cmd == 'R') {
                reverse_flag = !reverse_flag;
            } else if (cmd == 'D') {
                if (!queue.empty()) {
                    if (reverse_flag) queue.pop_back();
                    else queue.pop_front();
                } else {
                    error_flag = true;
                    break;
                }
            }
        }

        if (error_flag) {
            cout << "error\n";
        } else {
            cout << "[";
            if (reverse_flag) {
                for (int i = (int)queue.size() - 1; i >= 0; --i) {
                    cout << queue[i];
                    if (i > 0) cout << ",";
                }
            } else {
                for (int i = 0; i < (int)queue.size(); ++i) {
                    cout << queue[i];
                    if (i + 1 < (int)queue.size()) cout << ",";
                }
            }
            cout << "]\n";
        }
    }
    return 0;
}
