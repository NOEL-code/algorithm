#include <iostream>
#include <map>
#include <string>
using namespace std;

map<char, pair<char, char>> tree;

void pre_order(char root) {
    if (root != '.') {
        cout << root;
        pre_order(tree[root].first);
        pre_order(tree[root].second);
    }
}

void in_order(char root) {
    if (root != '.') {
        in_order(tree[root].first);
        cout << root;
        in_order(tree[root].second);
    }
}

void post_order(char root) {
    if (root != '.') {
        post_order(tree[root].first);
        post_order(tree[root].second);
        cout << root;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    for (int i = 0; i < N; ++i) {
        char root, left, right;
        cin >> root >> left >> right;
        tree[root] = {left, right};
    }

    pre_order('A');
    cout << "\n";
    in_order('A');
    cout << "\n";
    post_order('A');
    cout << "\n";
    return 0;
}
