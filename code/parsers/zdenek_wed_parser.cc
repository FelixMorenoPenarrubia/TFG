#include<iostream>
#include<utility>
#include<string>
#include<sstream>
#include<map>
#include<vector>
#include<algorithm>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::pair;
using std::string;
using std::vector;

int main() {
    string line;
    vector<vector<vector<int>>> adj_lists;
    vector<vector<int>> list_sizes;
    vector<vector<int>> cal;
    int l;
    int start_vertex = 0;
    while (std::getline(cin, line)) {
        std::stringstream ss(line);
        string title;
        ss >> title;
        if (title == "Wedge") {
            //initialize stuff
            cal.clear();
        }
        else if (title == "start") {
            string str;
            ss >> str; //1->0
            start_vertex = std::stoi(str.substr(str.find('>')+1));
            ss >> str; //length
            ss >> l;
            l++;
        }
        else if (title == "vertex") {
            int u = cal.size();
            cal.push_back({});
            ss >> title;
            string vs;
            while (ss >> vs) {
                int v = std::stoi(vs);
                cal[u].push_back(v);
            }
        }
        else if (title == "face") {
            //nothing, really
        }
        else if (title == "feet") {
            ss >> title; //at
            ss >> title; //face
            ss >> title; //0:
            string str;
            int n = cal.size();
            vector<int> cls(n, 5);
            vector<int> morph(n, -1);
            vector<int> outerface;
            
            while (ss >> str) {
                int v = std::stoi(str.substr(0, str.find('(')));
                int c = std::stoi(str.substr(str.find('(')+1, 1));
                cls[v] = 5-c;
                outerface.push_back(v);
            }

            int ofs = outerface.size();
            for (int i=0; i < ofs; ++i) {
                if (/*outerface[i] == 1 &&*/ outerface[(i+1)%ofs] == start_vertex) {
                    for (int j=1; j <= l; ++j) {
                        morph[outerface[(i+j)%ofs]] = j-1;
                        if (j < l) {
                            int idx = std::find(cal[outerface[(i+j)%ofs]].begin(), cal[outerface[(i+j)%ofs]].end(), outerface[(i+j+1)%ofs])-cal[outerface[(i+j)%ofs]].begin();
                            vector<int> ncal;
                            int cofs = cal[outerface[(i+j)%ofs]].size();
                            for (int k=0; k < cofs; ++k) {
                                ncal.push_back(cal[outerface[(i+j)%ofs]][(idx+k)%cofs]);
                            }
                            cal[outerface[(i+j)%ofs]] = ncal;
                        }
                    }
                    break;
                }
            }
            int nn = l;
            
            for (int i=0; i < n; ++i) {
                if (morph[i] == -1) {
                    morph[i] = nn++;
                }
            }

            vector<vector<int>> al(n);
            vector<int> ls(n);
            for (int u=0; u < n; ++u) {
             // cerr << morph[u]  << " " << n << endl;
             // cerr << adj_lists.size() << endl;
                ls[morph[u]] = cls[u];
                for (int v : cal[u]) {
                    al[morph[u]].push_back(morph[v]);
                }
            }
            adj_lists.push_back(al);
            list_sizes.push_back(ls);


        }
    }
    int T = adj_lists.size();
    cout << T << endl;
    for (int z=0; z < T; ++z) {
        int n = adj_lists[z].size();
        int m = 0;
        for (int i=0; i < n; ++i) {
            m += adj_lists[z][i].size();
        }
        m /= 2;
        int l = 0;
        while(l < n && list_sizes[z][l] == 1) l++;
        cout << n << " " << m << " " << l << endl;
        for (int i=0; i < n; ++i) cout << list_sizes[z][i] << " ";
        cout << endl;
        for (int u=0; u < n; ++u) {
            for (int v : adj_lists[z][u]) {
                cout << u << " " << v << endl;
            }
        }
    }

    
    
}