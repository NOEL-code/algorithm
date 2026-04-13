#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    unordered_map<string, int> word_lst;
    for (int i = 0; i < N; ++i) {
        string word;
        cin >> word;
        if ((int)word.size() < M) continue;
        word_lst[word] += 1;
    }

    vector<pair<string, int>> items(word_lst.begin(), word_lst.end());
    sort(items.begin(), items.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        if (a.second != b.second) return a.second > b.second;
        if (a.first.size() != b.first.size()) return a.first.size() > b.first.size();
        return a.first < b.first;
    });

    for (auto& p : items) {
        cout << p.first << "\n";
    }
    return 0;
}
