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
struct DynamicLazySeg{
    public:
    int size = 1 << LG;
    struct Node{
        Range ns, ne;
        S value;
        F lz;
        int l, r;
        Node():value(e()){}
        Node(Range l, Range r): ns(l), ne(r), value(e()), lz(id()), l(0), r(0){}
    };
    inline int make(Range l, Range r){
        if(pivot == size){
            nalloc = new Node[size * 2];
            ranges::move(alloc, alloc+size, nalloc);
            delete[] alloc;
            alloc = nalloc;
            nalloc = nullptr;
            size *= 2;
        }
        assert(pivot < size);
        alloc[pivot] = Node(l, r);
        return pivot++;
    }
    // Node Range: ex) -inf, inf
    DynamicLazySeg(Range l, Range r){
        alloc = new Node[size]; nalloc = nullptr; pivot = 1;
        root = make(l, r);
    }
    // i \in [l, r], A[i] => f(A[i])
    void upd(Range l, Range r, F f){
        return upd(l, r, f, root);
    }
    // op(A[l], ... , A[r])
    S qry(Range l, Range r){
        return qry(l, r, root);
    }

    S qry_all(){ return alloc[root].value; }
    
    ~DynamicLazySeg(){
        delete[] alloc;
    }

    private:
    int root;
    Node *alloc, *nalloc; int pivot;
    inline bool disjoint(Range l, Range r, Range s, Range e){
        return r < s or e < l;
    }
    void pull(int i){
        alloc[i].value = op(alloc[i].l ? alloc[alloc[i].l].value : e(), alloc[i].r ? alloc[alloc[i].r].value : e());
    }
    void apply(int i, F f){
        if(f == id()) return;
        // index information
        mapping(f, alloc[i].value, alloc[i].ne - alloc[i].ns + 1);
        if(alloc[i].ns < alloc[i].ne){
            composition(f, alloc[i].lz);
            // if(nd->value.fail) push(i), pull(i); // SegTree Beats, add mapping fail info to 'S' (necessary)
        }
    }
    void push(int i){
        if(alloc[i].lz == id()) return;
        Range nm = midpoint(alloc[i].ns, alloc[i].ne);
        if(not alloc[i].l) {
            int l = make(alloc[i].ns, nm);
            alloc[i].l = l;
        }
        if(not alloc[i].r){
            int r = make(nm + 1, alloc[i].ne);
            alloc[i].r = r;
        }
        apply(alloc[i].l, alloc[i].lz);
        apply(alloc[i].r, alloc[i].lz);
        alloc[i].lz = id();
    }
    void upd(Range l, Range r, F f, int i){
        if(not i or disjoint(l, r, alloc[i].ns, alloc[i].ne)) return;
        if(l <= alloc[i].ns and alloc[i].ne <= r){
            apply(i, f);
            return;
        }
        push(i);
        Range nm = midpoint(alloc[i].ns, alloc[i].ne);
        if(not alloc[i].l and not disjoint(l, r, alloc[i].ns, nm)){
            int l = make(alloc[i].ns, nm);
            alloc[i].l = l;
        }
        if(not alloc[i].r and not disjoint(l, r, nm + 1, alloc[i].ne)){
            int r = make(nm + 1, alloc[i].ne);
            alloc[i].r = r;
        }
        upd(l, r, f, alloc[i].l);
        upd(l, r, f, alloc[i].r);
        pull(i);
    }
    S qry(Range l, Range r, int i){
        if(not i or disjoint(l, r, alloc[i].ns, alloc[i].ne)) return e();
        if(l <= alloc[i].ns and alloc[i].ne <= r) return alloc[i].value;
        push(i);
        return op(qry(l, r, alloc[i].l), qry(l, r, alloc[i].r));
    }
};