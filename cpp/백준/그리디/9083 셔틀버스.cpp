#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>
#include <string>
using namespace std;

int parseTime(const string& s) {
    int h = stoi(s.substr(0, 2));
    int m = stoi(s.substr(3, 2));
    return h * 60 + m;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int D, A, B;
        cin >> D >> A;

        vector<tuple<int, char, int>> bus;
        for (int i = 0; i < A; ++i) {
            string s;
            cin >> s;
            int t = parseTime(s);
            bus.push_back({t, 'S', t + D});
        }

        cin >> B;
        for (int i = 0; i < B; ++i) {
            string s;
            cin >> s;
            int t = parseTime(s);
            bus.push_back({t, 'T', t + D});
        }

        sort(bus.begin(), bus.end(), [](const tuple<int, char, int>& a, const tuple<int, char, int>& b) {
            return get<0>(a) < get<0>(b);
        });

        priority_queue<int, vector<int>, greater<int>> school;
        priority_queue<int, vector<int>, greater<int>> terminal;
        int cnt = 0;

        for (auto& item : bus) {
            int time = get<0>(item);
            char bus_type = get<1>(item);
            int ready_time = get<2>(item);

            if (bus_type == 'S') {
                vector<int> temp;
                while (!school.empty() && school.top() <= time) {
                    temp.push_back(school.top());
                    school.pop();
                }
                if (!temp.empty()) {
                    int maxVal = *max_element(temp.begin(), temp.end());
                    bool removed = false;
                    for (int t : temp) {
                        if (!removed && t == maxVal) {
                            removed = true;
                            continue;
                        }
                        school.push(t);
                    }
                } else {
                    if (!terminal.empty() && terminal.top() + D <= time) {
                        terminal.pop();
                    } else {
                        cnt += 1;
                    }
                }
                terminal.push(ready_time);
            } else {
                vector<int> temp;
                while (!terminal.empty() && terminal.top() <= time) {
                    temp.push_back(terminal.top());
                    terminal.pop();
                }
                if (!temp.empty()) {
                    int maxVal = *max_element(temp.begin(), temp.end());
                    bool removed = false;
                    for (int t : temp) {
                        if (!removed && t == maxVal) {
                            removed = true;
                            continue;
                        }
                        terminal.push(t);
                    }
                } else {
                    if (!school.empty() && school.top() + D <= time) {
                        school.pop();
                    } else {
                        cnt += 1;
                    }
                }
                school.push(ready_time);
            }
        }

        cout << cnt << "\n";
    }

    return 0;
}
