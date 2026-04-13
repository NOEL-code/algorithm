#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    string num;
    cin >> num;

    vector<char> stack;
    int count = 0;
    for (char digit : num) {
        while (!stack.empty() && stack.back() < digit && count < K) {
            stack.pop_back();
            count += 1;
        }
        stack.push_back(digit);
    }

    if (count < K) {
        for (int i = 0; i < K - count; ++i) {
            stack.pop_back();
        }
    }

    for (char ch : stack) cout << ch;
    cout << "\n";
    return 0;
}
