#include <iostream>
#include <deque>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    deque<int> cards;
    for (int i = 1; i <= n; ++i) cards.push_back(i);

    while (!cards.empty()) {
        cout << cards.front() << " ";
        cards.pop_front();
        if (!cards.empty()) {
            cards.push_back(cards.front());
            cards.pop_front();
        }
    }
    return 0;
}
