#include<iostream>
#include<utility>
#include<string>
#include<map>


using std::cin;
using std::cout;
using std::endl;
using std::pair;
using std::string;

int main() {
    
    std::map<pair<int, int>, int> cnt;
    int T;
    cin >> T;
    while (T--) {
        int n, m, l;
        cin >> n >> m >> l;
        for (int i=0; i < n; ++i) {
            int x;
            cin >> x;
        }
        cnt[std::make_pair(n,m)]++;
        for (int z=0; z < 2*m; ++z) {
            int u,v;
            cin >> u >> v;
        }
    }

    for (auto p : cnt) {
        cout << "n = " << p.first.first << " m = " << p.first.second << ": " << p.second << endl;
    }
    
}