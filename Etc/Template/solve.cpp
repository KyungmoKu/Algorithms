#pragma GCC optimize("Ofast", "unroll-loops")
// #pragma GCC target("avx2", "popcnt") // for bitset
#include"bits/stdc++.h"
// ext headers before any define: ext/pb_ds/assoc_container.hpp, tr2/dynamic_bitset
using namespace std;
auto ifloor = [](auto l,auto r){return l / r - ((l^r) < 0 && l % r);};
auto iceil = [](auto l, auto r){return ifloor(l + r - 1, r);};
template<typename T1, typename T2> istream& operator>> (istream&in, pair<T1, T2>&p){in>>p.first>>p.second; return in;}
template<typename T, size_t SZ> istream& operator>> (istream&in, array<T, SZ>&v){for(size_t i=0;i<SZ;i++)in>>v[i]; return in;}
template<typename T> istream& operator>> (istream&in, vector<T>&v){for(auto&x:v)in>>x; return in;}
template<typename T> ostream& operator<< (ostream&out, vector<T>&v){for(auto&x:v)out<<x<<' '; return out;}
template<typename T> T randint(T l, T r) {static std::mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count()); return std::uniform_int_distribution<T>(l,r)(gen);}
#define all(v) (v).begin(),(v).end()
#define nl '\n'
#define sp ' '
#define rep(i, n) for(int i=0;i<(n);i++)
#define rep1(i, n) for(int i=1;i<=(n);i++)
#define repr(i, a, b) for(int i=(a);i<=(b);i++)
#define repd(i, b, a) for(int i=(b);i>=(a);i--)
#define int int64_t
using ll = int64_t; using pii = pair<int,int>; int cur_tc;
template<size_t SZ> using ia = array<int, SZ>;
constexpr int INF = (int) 1e9 + 3; constexpr ll LINF = (ll) 4e18 + 3;


int32_t main(){
    cin.tie(0)->sync_with_stdio(0); cin.exceptions(ios::badbit | ios::failbit);
    // cout<<fixed<<setprecision(15);
    int tc = 1;
    // cin >> tc;
    auto solve_tc = [&](){
        // ${0}
    };
    for(cur_tc=1;cur_tc<=tc;cur_tc++) solve_tc();
    return 0;
}