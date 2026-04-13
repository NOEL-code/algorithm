#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> plus;
    vector<int> minus;
    long long result = 0;

    for (int i = 0; i < n; ++i) {
        int num;
        cin >> num;
        if (num > 1) {
            plus.push_back(num);
        } else if (num <= 0) {
            minus.push_back(num);
        } else {
            result += num;
        }
    }

    sort(plus.begin(), plus.end(), greater<int>());
    sort(minus.begin(), minus.end());

    for (int i = 0; i < (int)plus.size(); i += 2) {
        if (i + 1 >= (int)plus.size()) {
            result += plus[i];
        } else {
            result += (long long)plus[i] * plus[i + 1];
        }
    }

    for (int i = 0; i < (int)minus.size(); i += 2) {
        if (i + 1 >= (int)minus.size()) {
            result += minus[i];
        } else {
            result += (long long)minus[i] * minus[i + 1];
        }
    }

    cout << result << "\n";
    return 0;
}
