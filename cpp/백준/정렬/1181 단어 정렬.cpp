#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    set<string> unique_words;
    for (int i = 0; i < n; ++i) {
        string word;
        cin >> word;
        unique_words.insert(word);
    }

    vector<string> array(unique_words.begin(), unique_words.end());
    stable_sort(array.begin(), array.end(), [](const string& a, const string& b) {
        return a.size() < b.size();
    });

    for (const auto& s : array) {
        cout << s << "\n";
    }
    return 0;
}
