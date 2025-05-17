/*
put "ext_gcd and inv" here
https://github.com/KyungmoKu/Algorithms
*/

/*
Chinese Remainder Theorem when only it's pairwise coprime
reference: https://cp-algorithms.com/algebra/chinese-remainder-theorem.html
*/
struct cong{ll a, m;};
// (a, m) -> a mod m
cong crt(const vector<cong> &cgv) {
    // assume all mods are coprime each other
    if(cgv.size() == 1) return cgv[0];
    ll M = 1;
    for (const auto& cg : cgv) M *= cg.m;
    ll ans = 0;
    for (auto [a_i, m_i]: cgv) {
        ll M_i = M / m_i;
        ll N_i = inv(M_i, m_i);
        ans = (ans + a_i * M_i % M * N_i) % M;
    }
    return cong{ans, M};
}