#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    unordered_map<string, string> _dict;
    for (int i = 0; i < N; ++i) {
        string site, password;
        cin >> site >> password;
        _dict[site] = password;
    }

    for (int i = 0; i < M; ++i) {
        string site;
        cin >> site;
        cout << _dict[site] << "\n";
    }

    return 0;
}
