#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    priority_queue<long long, vector<long long>, greater<long long>> cards;
    for (int i = 0; i < n; ++i) {
        long long x;
        cin >> x;
        cards.push(x);
    }

    long long result = 0;
    while (cards.size() > 1) {
        long long first = cards.top(); cards.pop();
        long long second = cards.top(); cards.pop();

        result += (first + second);
        cards.push(first + second);
    }

    cout << result << "\n";
    return 0;
}
