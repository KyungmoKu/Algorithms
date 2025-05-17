/*
# Lazy Segtree Tutorial
- LazySeg seg(vector) or seg(size)
- S: monoid for tree
- S e() : identity of S
- S op(S a, S b): monoid action
- F: mapping function
- F id(): identity of f
- void mapping(F f, S&x) : x => f(x)
- void composition(F f, F&g): g(x) => f(g(x))
- (optional) `LazySeg<S, e, op, F, id, mapping, composition>`
- `upd(l, r, f) : i \in [l, r], A[i] => f(A[i])`
- `qry(l, r) : op(A[l], ... , A[r])`
- Tutorial(+Beats): LazySeg_Tutorial.md in the same folder
*/

$0
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


// template<   class S, S (*e)(),
//             S (*op)(S, S),
//             class F, F(*id)(), 
//             void (*mapping)(F, S&),
//             void (*composition)(F, F&)>
struct LazySeg{
    public:
    LazySeg(int n = 0) : LazySeg(vector(n, e())) {}
    LazySeg(const vector<S>& v){
        n = bit_ceil(size(v));
        tree = vector (n<<1, e());
        lz = vector (n, id());
        ranges::copy(v, begin(tree) + n);
        for(int i=n-1;i>=1;i--) pull(i);
    }
    // i \in [l, r], A[i] => f(A[i])
    void upd(int l, int r, F f, int nd = 1, int ns = 0, int ne = -1){
        // assert(0 <= l and l <= r and r <= n - 1);
        if(ne == -1) ne = n - 1;
        if(r < ns or ne < l) return; // add 'strengthend condition' for segtree beats (optional)
        if(l <= ns and ne <= r){
            apply(nd, f);
            return;
        }
        push(nd); 
        int nm = midpoint(ns, ne);
        upd(l, r, f, nd << 1, ns, nm); upd(l, r, f, nd << 1 | 1, nm+1, ne);
        pull(nd);
    }
    // op(A[l], ... , A[r])
    S qry(int l, int r, int nd = 1, int ns = 0, int ne = -1){
        // assert(0 <= l and l <= r and r <= n - 1);
        if(ne == -1) ne = n - 1;
        if(r < ns or ne < l) return e();
        if(l <= ns and ne <= r) return tree[nd];
        push(nd);
        int nm = midpoint(ns, ne);
        return op(qry(l, r, nd << 1, ns, nm), qry(l, r, nd << 1 | 1, nm+1, ne));
    }
    S qry_all(){ return tree[1]; }
    
    private:
    int n;
    vector<S> tree; vector<F> lz;
    void pull(int i){ tree[i] = op(tree[i<<1], tree[i<<1|1]);}
    void apply(int i, F f){
        mapping(f, tree[i]);
        if(i < n){
            composition(f, lz[i]);
            // if(tree[i].fail) push(i), pull(i); // SegTree Beats, add mapping fail info to 'S'(necessary)
        }
    }
    void push(int i){
        apply(i << 1, lz[i]);
        apply(i << 1 | 1, lz[i]);
        lz[i] = id();
    }
};
