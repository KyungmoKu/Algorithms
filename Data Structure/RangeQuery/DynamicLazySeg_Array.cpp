/*
# Dynamic Lazy Segtree - Memory Opt
- same convention with LazySeg
- Lazy Seg Tutorial: LazySeg_Tutorial.md in the same folder
- `DynamicLazySeg seg(Range l, Range r);`
- If you need index information, consider one of following
    - 1. modify `make` function to put info.
    - 2. modify `mapping` function to get additional parameter (index), and modify `apply` function
- Range=> index type, S=> node monoid, F=> update monoid
- If you need constant-optimization, use Array version in github.
    - adjust LG = Log Size of Array.
*/
$0
// Example
// https://www.acmicpc.net/problem/20212
// Range: int, S: range sum, F: range add
constexpr int LG = 21; // Log Size of Array
using Range = int;
using S = ll;
constexpr S e(){return 0;}
S op(S l, S r){return S{l + r};}
using F = ll; // add
constexpr F id(){return 0;}
// x = f(x)
void mapping(F f, S&x, Range len){x += len * f;}
// g(x) = f(g(x))
void composition(F f, F&g){g += f;}


// template<   class Range, 
//             class S, S (*e)(),
//             S (*op)(S, S),
//             class F, F(*id)(),
//             void (*mapping)(F, S&),
//             void (*composition)(F, F&),
//             int LG = 21 >
template<int LG = 21>
struct DynamicLazySeg{
    public:
    struct Node{
        Range ns, ne;
        S value;
        F lz;
        int l, r;
        Node():value(e()){}
        Node(Range l, Range r): ns(l), ne(r), value(e()), lz(id()), l(0), r(0){}
    };
    inline int make(Range l, Range r){
        assert(pivot < size);
        alloc[pivot] = Node(l, r);
        return pivot++;
    }
    // Node Range: ex) -inf, inf
    DynamicLazySeg(Range l, Range r){
        size = 1 << LG;
        alloc = new Node[size]; pivot = 1;
        root = make(l, r);
    }
    // i \in [l, r], A[i] => f(A[i])
    void upd(Range l, Range r, F f){
        return upd(l, r, f, &alloc[root]);
    }
    // op(A[l], ... , A[r])
    S qry(Range l, Range r){
        return qry(l, r, &alloc[root]);
    }

    S qry_all(){ return alloc[root].value; }
    
    ~DynamicLazySeg(){
        delete[] alloc;
    }

    private:
    int root;
    Node *alloc; int size, pivot;
    inline bool disjoint(Range l, Range r, Range s, Range e){
        return r < s or e < l;
    }
    void pull(Node*nd){
        nd->value = op(nd->l ? alloc[nd->l].value : e(), nd->r ? alloc[nd->r].value : e());
    }
    void apply(Node*nd, F f){
        if(f == id()) return;
        mapping(f, nd->value, nd->ne - nd->ns + 1);
        if(nd->ns < nd->ne){
            composition(f, nd->lz);
            // if(nd->value.fail) push(nd), pull(nd); // SegTree Beats, add mapping fail info to 'S' (necessary)
        }
    }
    void push(Node*nd){
        if(nd -> lz == id()) return;
        Range nm = midpoint(nd->ns, nd->ne);
        if(not nd->l) nd->l = make(nd->ns, nm);
        apply(&alloc[nd->l], nd->lz);
        if(not nd->r) nd->r = make(nm + 1, nd->ne);
        apply(&alloc[nd->r], nd->lz);
        nd->lz = id();
    }
    void upd(Range l, Range r, F f, Node*nd){
        if(not nd or disjoint(l, r, nd->ns, nd->ne)) return;
        if(l <= nd->ns and nd->ne <= r){
            apply(nd, f);
            return;
        }
        push(nd);
        Range nm = midpoint(nd->ns, nd->ne);
        if(not nd->l and not disjoint(l, r, nd->ns, nm)) nd->l = make(nd->ns, nm);
        if(not nd->r and not disjoint(l, r, nm + 1, nd->ne)) nd->r = make(nm + 1, nd->ne);
        upd(l, r, f, &alloc[nd->l]);
        upd(l, r, f, &alloc[nd->r]);
        pull(nd);
    }
    S qry(Range l, Range r, Node *nd){
        if(not nd or disjoint(l, r, nd->ns, nd->ne)) return e();
        if(l <= nd->ns and nd->ne <= r) return nd->value;
        push(nd);
        return op(qry(l, r, &alloc[nd->l]), qry(l, r, &alloc[nd->r]));
    }
};