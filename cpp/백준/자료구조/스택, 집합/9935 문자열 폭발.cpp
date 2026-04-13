#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, bomb;
    cin >> s >> bomb;

    int bomb_len = (int)bomb.size();
    vector<char> stack;
    stack.reserve(s.size());

    for (char ch : s) {
        stack.push_back(ch);
        if ((int)stack.size() >= bomb_len) {
            bool match = true;
            for (int i = 0; i < bomb_len; ++i) {
                if (stack[stack.size() - bomb_len + i] != bomb[i]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                for (int i = 0; i < bomb_len; ++i) stack.pop_back();
            }
        }
    }

    if (stack.empty()) {
        cout << "FRULA\n";
    } else {
        for (char c : stack) cout << c;
        cout << "\n";
    }
    return 0;
}
