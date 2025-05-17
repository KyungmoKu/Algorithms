# Lazy Segtree Tutorial
- LazySeg seg(vector) or seg(size)
- S: monoid for tree
- S e() : identity of S
- S op(S a, S b): monoid action
- F: mapping function
- F id(): identity of f
- void mapping(F f, S&x) : x => f(x)
- void composition(F f, F&g): g(x) => f(g(x))
- If you need index information, modify `mapping` function to get additional parameter, and `apply` function
- `upd(l, r, f) : i \in [l, r], A[i] => f(A[i])`
- `qry(l, r) : op(A[l], ... , A[r])`
- (optional) template: `LazySeg<S, e, op, F, id, mapping, composition>`
- Tutorial(+Beats): LazySeg_Tutorial.md in the same folder

# SegTree Beats Tutorial
## mapping to try_mapping
- 1. add `bool fail` variable into `S`.
- 2. if mapping(F f, S x) can't be made, put `fail = true`, otherwise `fail = false`.
- 3. uncomment `if(tree[i].fail) push(i), pull(i);` in `apply` function.
## (optional) strengthened break condition
- put additional condition for returning `upd` function at the first.

# Dynamic Lazy Segtree
- same convention with LazySeg
- Lazy Seg Tutorial: LazySeg_Tutorial.md in the same folder
- `DynamicLazySeg seg(Range l, Range r);`
- If you need index information, consider one of following
    - 1. modify `make` function to put info.
    - 2. modify `mapping` function to get additional parameter (index), and modify `apply` function
- Range=> index type, S=> node monoid, F=> update monoid
- If you need constant-optimization, use Array version in github.
    - adjust LG = Log Size of Array.

# Examples
## Example 1
```cpp
// Example 1
// https://www.acmicpc.net/problem/10999
// S: range sum, F: range add
// WARNING: should initialize by LazySeg(vector(n, S{0, 1}));
struct S{
    ll sum;
    int cnt;
};
constexpr S e(){return {0, 0};}
S op(const S& l,const S& r){
    return S{
        l.sum + r.sum,
        l.cnt + r.cnt
    };
};
using F = ll; // add
constexpr F id(){return 0;}
// x = f(x)
void mapping(F f, S&x){x.sum += x.cnt * f;}
// g(x) = f(g(x))
void composition(F f, F&g){g += f;}
```

## Example 2
```cpp
// S : range max, F : range add
// https://www.acmicpc.net/problem/30857
using S = int; // max
constexpr S MIN = numeric_limits<S>::min() / 2; 
constexpr S e(){return MIN;}
S op(S a, S b){return max(a, b);}
using F = int; // add
// x => f(x)
constexpr F id(){return 0;}
void mapping(F f, S&x){x += f;}
// g(x) => f(g(x))
void composition(F f, F&g){g += f;}
```

## Example 3
```cpp
// Example 3
// S : range add, F : range mul, add, set 
// https://www.acmicpc.net/problem/13925
constexpr int MOD = (int)1e9 + 7;
struct S{
    int sum;
    int cnt;
};
constexpr S e(){return {0, 0};}
inline int add(int a,int b){
    a += b;
    if(a >= MOD) a -= MOD;
    return a;
}
inline int mul(int a,int b){
    return 1LL * a * b % MOD;
}
S op(S a, S b){
    return {add(a.sum, b.sum), a.cnt + b.cnt};
}

struct F{
    int mul, add;
};
constexpr F id(){
    return F{
        1,
        0
    };
}
void mapping(F f, S&x){
    if(!x.cnt) return;
    x.sum = add(mul(x.sum, f.mul), mul(x.cnt, f.add));
}
void composition(F f, F&g){
    g = F{
        mul(g.mul, f.mul),
        add(mul(g.add, f.mul), f.add)
    };
}
```

## Example 4 (Segment Tree Beats)
```cpp
/*
SegTree Beats
reference: https://hyperbolic.tistory.com/3
S: range max, range sum, F : range min
problem: https://www.acmicpc.net/problem/17474
*/
struct S{
    int mx, mxcnt;
    int mx2;
    ll sum;
    bool fail; // true when mapping failed, false otherwise
};
constexpr S e(){return S{-1, 0, -1, 0, false};}
S op(S a, S b){
    if(a.mx < b.mx) swap(a, b);
    // a.mx > b.mx
    return S{
        a.mx,
        a.mxcnt + ((a.mx == b.mx) ? b.mxcnt : 0),
        max(a.mx2, ((a.mx == b.mx) ? b.mx2 : b.mx)),
        a.sum + b.sum,
        false,
    };
}
using F = int;
constexpr F INF = numeric_limits<F>::max();
constexpr F id(){return INF;}
// x => f(x)
void mapping(F f, S&x){
    if(f >= x.mx) return;
    if(f > x.mx2){
        x.sum -= 1LL * (x.mx - f) * x.mxcnt;
        x.mx = f;
        return;
    }
    x.fail = true;
    return;
}
// g(x) => f(g(x))
void composition(F f, F&g){g = min(g, f);}
```