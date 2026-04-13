#include <iostream>
#include <string>
#include <stack>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    stack<char> stack_l, stack_r;
    for (char c : s) stack_l.push(c);

    int n;
    cin >> n;

    while (n--) {
        char command;
        cin >> command;

        if (command == 'L') {
            if (!stack_l.empty()) {
                stack_r.push(stack_l.top());
                stack_l.pop();
            }
        } else if (command == 'D') {
            if (!stack_r.empty()) {
                stack_l.push(stack_r.top());
                stack_r.pop();
            }
        } else if (command == 'B') {
            if (!stack_l.empty()) stack_l.pop();
        } else if (command == 'P') {
            char ch;
            cin >> ch;
            stack_l.push(ch);
        }
    }

    string left_part;
    while (!stack_l.empty()) {
        left_part += stack_l.top();
        stack_l.pop();
    }
    for (int i = (int)left_part.size() - 1; i >= 0; --i) cout << left_part[i];
    while (!stack_r.empty()) {
        cout << stack_r.top();
        stack_r.pop();
    }
    cout << "\n";
    return 0;
}
