#include<iostream>
#include<utility>
#include<string>
#include<map>
#include<vector>

using std::cin;
using std::cout;
using std::endl;
using std::pair;
using std::string;

int main() {
    string str;
    std::map<pair<int, int>, int> cnt;
    std::map<pair<int, int>, std::vector<int> > vec;
    int n, m;
    bool in_face = false;
    int wn = 0;
    while (cin >> str) {
        if (str == "Wedge") {
            n = 0;
            m = 0;
            cin >> str; //wedge number;
        }
        else if (str == "start") {
            cin >> str; //1->0
            cin >> str; //length
            cin >> str; //l
        }
        else if (str == "vertex") {
            n++;
            cin >> str; //i:
            in_face = false;
        }
        else if (str == "face") {
            in_face = true;
            cin >> str; //i:
        }
        else if (str == "feet") {
            cin >> str; //at
            cin >> str; //face
            cin >> str; //i:
            in_face = true;
            cnt[std::make_pair(n, m/2)]++;
            vec[std::make_pair(n, m/2)].push_back(wn++);
        }
        else  {
            if (!in_face) {
                m++;
            }
        }
    }

    for (auto p : cnt) {
        cout << "n = " << p.first.first << " m = " << p.first.second << ": " << p.second << endl;
        for (int x : vec[p.first]) {
            cout << x << " ";
        }
        cout << endl;
    }
    
}