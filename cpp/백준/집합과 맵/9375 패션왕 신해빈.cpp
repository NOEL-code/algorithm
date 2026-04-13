#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;
        unordered_map<string, int> wardrobe;
        for (int i = 0; i < n; ++i) {
            string name, clothes_type;
            cin >> name >> clothes_type;
            wardrobe[clothes_type] += 1;
        }

        long long count = 1;
        for (const auto& p : wardrobe) {
            count *= (p.second + 1);
        }
        cout << count - 1 << "\n";
    }

    return 0;
}
