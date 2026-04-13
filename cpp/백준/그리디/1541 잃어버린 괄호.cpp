#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    cin >> line;

    vector<string> parts;
    {
        string cur;
        for (char ch : line) {
            if (ch == '-') {
                parts.push_back(cur);
                cur.clear();
            } else {
                cur += ch;
            }
        }
        parts.push_back(cur);
    }

    auto sumPluses = [](const string& s) {
        int total = 0;
        string cur;
        for (char ch : s) {
            if (ch == '+') {
                total += stoi(cur);
                cur.clear();
            } else {
                cur += ch;
            }
        }
        if (!cur.empty()) total += stoi(cur);
        return total;
    };

    int result = sumPluses(parts[0]);
    for (int i = 1; i < (int)parts.size(); ++i) {
        result -= sumPluses(parts[i]);
    }

    cout << result << "\n";
    return 0;
}
