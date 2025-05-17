/*
CRT that can handle every case
reference: https://qoj.ac/submission/30536
*/

// PUT EXT_GCD HERE: https://github.com/KyungmoKu/Algorithms

// using lll = __int128_t;
struct cong{ll a, m;}; // a mod m
cong crt_merge(const cong &c1, const cong &c2){
    auto[xa, xm] = c1; auto[ya, ym] = c2;
    auto [g,x,y] = ext_gcd(xm, ym);
    // xm * x + ym * y = g;
    if((ya-xa) % g != 0) return cong{-1,0};
    auto ga = (ya-xa)/g;
    // xm * (x * ga) + ym * (y * ga) = ya - xa
    // xm * (x * ga) + xa = ym * (-y*gb) +  ya
    auto yt = ym / g;
    x = (x * ga % yt + yt) % yt;
    return cong{xm * x + xa, xm * yt};
}
cong crt(const vector<cong> &cgv) {
    auto ans = cong{0,1};
    for(const auto&cg: cgv){
        assert(cg.m != 0 && "mod not 0");
        ans = crt_merge(ans, cg);
        if(ans.m == 0) return ans;
    }
    return ans;
}