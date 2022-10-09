#include<iostream>
#include<utility>
#include<string>

#ifndef DEBUG_UTILITY_H_
#define DEBUG_UTILITY_H_



using std::cerr;
using std::pair;
using std::string;

void __print(int x);
void __print(long x);
void __print(long long x);
void __print(unsigned x);
void __print(unsigned long x);
void __print(unsigned long long x);
void __print(float x);
void __print(double x);
void __print(long double x);
void __print(char x);
void __print(const char *x);
void __print(const string &x);
void __print(bool x);

//TODO: fix templates
template<typename T, typename V>
inline void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ','; __print(x.second); cerr << '}';}
template<typename T>
inline void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? "," : ""), __print(i); cerr << "}";}
inline void _print() {cerr << "]\n";}
template <typename T, typename... V>
inline void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
#ifdef DEBUG
#define debug_var(x...) cerr <<  __FILE__ << ":" << __LINE__ << " [" << #x << "] = ["; __print(x); cerr << "]\n";
#define debug_msg(x) cerr <<  __FILE__ << ":" << __LINE__ << " " << x << "\n";
#else
#define debug_var(x...)
#define debug_msg(x)
#endif
#endif