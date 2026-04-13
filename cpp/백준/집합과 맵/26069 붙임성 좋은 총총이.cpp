#include <iostream>
#include <string>
#include <unordered_set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    unordered_set<string> danced;
    danced.insert("ChongChong");

    for (int i = 0; i < N; ++i) {
        string a, b;
        cin >> a >> b;
        if (danced.count(a) || danced.count(b)) {
            danced.insert(a);
            danced.insert(b);
        }
    }

    cout << danced.size() << "\n";
    return 0;
}
