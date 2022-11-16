#include "AlonTarsi.hh"
#include<functional>

using std::vector;
using std::pair;



bool alon_tarsi(const ListGraph& g) {
    vector<pair<int, int> > el;
    for (int u = 0; u < g.n; ++u) {
        for (int v : g.al[u]) {
            if (u < v) el.emplace_back(u, v);
        }
    }

    if (el.empty()) return true;

    vector<int> bounds = g.list_sizes;

    for (int& x : bounds) x--;

    std::map<vector<int>, int> oridiffmap;
    vector<int> indeg(g.n, 0);
    vector<int> ori(g.m, -1); //0: first -> second, 1: second -> first
    int parity = 1;
    vector<pair<int, int> > decision_stack; // (e, d), d == 1 if can decide, d == 0 if forced
    auto edge_options = [&] (int e) -> vector<int> {
        vector<int> v;
        if (indeg[el[e].second] < bounds[el[e].second]) v.push_back(0);
        if (indeg[el[e].first] < bounds[el[e].first]) v.push_back(1);
        return v;
    };
    std::function<void(int, int)> set_edge_ori = [&] (int e, int o) -> void {
        if (ori[e] == 0) {
            indeg[el[e].second]--;
        }
        if (ori[e] == 1) {
            indeg[el[e].first]--;
            parity *= -1;
        }
        ori[e] = o;
        if (ori[e] == 0) {
            indeg[el[e].second]++;
        }
        if (ori[e] == 1) {
            indeg[el[e].first]++;
            parity *= -1;
        }

        if (o == 0 || o == 1) {
            for (int i=0; i < g.m; ++i) {
                if (ori[i] == -1 && edge_options(i).size() == 1) {
                    decision_stack.emplace_back(i, 0);
                    set_edge_ori(i, edge_options(i)[0]);
                }
            }
        }
    };

    auto choose_edge = [&] () -> pair<int, int> {
        pair<int, int> p (-1, -1);
        int min_slack = 2e9;
        for (int i=0; i < g.m; ++i) {
            if (ori[i] == -1 && !edge_options(i).empty()) {
                bool op0ok = (edge_options(i).size() == 2 || edge_options(i)[0] == 0);
                bool op1ok = (edge_options(i).size() == 2 || edge_options(i)[0] == 1);
                if (op0ok && bounds[el[i].second]-indeg[el[i].second] < min_slack) {
                    min_slack = bounds[el[i].second]-indeg[el[i].second];
                    p = pair<int, int>(i, 0);
                }
                if (op1ok && bounds[el[i].first]-indeg[el[i].first] < min_slack) {
                    min_slack = bounds[el[i].first]-indeg[el[i].first];
                    p = pair<int, int>(i, 1);
                }
            }
        }
        return p;
    };

    pair<int, int> initial_choice = choose_edge();
    if (initial_choice.first == -1) return false;
    if (edge_options(initial_choice.first).size() == 2) {
        decision_stack.emplace_back(initial_choice.first, 1);
        set_edge_ori(initial_choice.first, initial_choice.second);
    }
    else {
        decision_stack.emplace_back(initial_choice.first, 0);
        set_edge_ori(initial_choice.first, initial_choice.second);
    }

    while (!decision_stack.empty()) {

        bool all_set = true;
        bool blocked = false;
        for (int e = 0; e < g.m && !blocked; ++e) {
            if (ori[e] == -1) {
                all_set = false;
                if (edge_options(e).empty()) {
                    blocked = true;
                }
            }
        }
        if (all_set) {
            oridiffmap[indeg] += parity;
        }
        if (all_set || blocked) {
            while (!decision_stack.empty()) {
                pair<int, int> p = decision_stack.back();
                decision_stack.pop_back();
                if (p.second) {
                    decision_stack.emplace_back(p.first, 0);
                    set_edge_ori(p.first, 1-ori[p.first]);
                    break;
                }
                else {
                    set_edge_ori(p.first, -1);
                }
            }
        }
        else {
            pair<int, int> choice = choose_edge();
            decision_stack.emplace_back(choice.first, 1);
            set_edge_ori(choice.first, choice.second);
        }
    }

    for (auto x : oridiffmap) {
        if (x.second != 0) return true;
    }
    
	return false;
	
}