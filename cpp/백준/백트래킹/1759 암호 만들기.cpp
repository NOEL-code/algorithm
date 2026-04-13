#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
using namespace std;

int L, C;
vector<string> alhabet;
vector<bool> used_alhabet;
set<char> vowels = {'a', 'e', 'i', 'o', 'u'};
vector<string> answer;

void back_tracking(vector<string>& arr, int now_length) {
    if (now_length == L) {
        int vowel_cnt = 0;
        for (auto& ch : arr) {
            if (vowels.count(ch[0])) vowel_cnt++;
        }
        int consonant_cnt = L - vowel_cnt;
        if (vowel_cnt >= 1 && consonant_cnt >= 2) {
            string s;
            for (auto& ch : arr) s += ch;
            answer.push_back(s);
        }
        return;
    }

    for (int idx = 0; idx < C; ++idx) {
        if (!used_alhabet[idx]) {
            if (now_length == 0 || arr[now_length - 1] < alhabet[idx]) {
                used_alhabet[idx] = true;
                arr.push_back(alhabet[idx]);
                back_tracking(arr, now_length + 1);
                arr.pop_back();
                used_alhabet[idx] = false;
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> L >> C;
    alhabet.resize(C);
    for (int i = 0; i < C; ++i) cin >> alhabet[i];
    sort(alhabet.begin(), alhabet.end());
    used_alhabet.assign(C, false);

    vector<string> arr;
    back_tracking(arr, 0);

    sort(answer.begin(), answer.end());
    for (auto& comb : answer) cout << comb << "\n";
    return 0;
}
