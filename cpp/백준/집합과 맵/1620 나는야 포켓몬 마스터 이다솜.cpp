#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    unordered_map<string, int> pocketMon_name;
    vector<string> pocketMon_int(N + 1);

    for (int i = 1; i <= N; ++i) {
        string name;
        cin >> name;
        pocketMon_name[name] = i;
        pocketMon_int[i] = name;
    }

    for (int i = 0; i < M; ++i) {
        string quiry;
        cin >> quiry;
        bool isDigit = !quiry.empty();
        for (char c : quiry) {
            if (!isdigit((unsigned char)c)) { isDigit = false; break; }
        }
        if (isDigit) {
            cout << pocketMon_int[stoi(quiry)] << "\n";
        } else {
            cout << pocketMon_name[quiry] << "\n";
        }
    }

    return 0;
}
