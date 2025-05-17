// Include => SCC latest version: https://github.com/KyungmoKu/Algorithms, SCC_Tarjan.cpp or SCC_Kosaraju.cpp

/*
two_sat
there should be scc_graph struct on top: https://github.com/KyungmoKu/Algorithms
reference1: https://github.com/kth-competitive-programming/kactl/blob/main/content/graph/2sat.h
reference2: https://atcoder.github.io/ac-library/production/document_en/twosat.html
test: https://atcoder.jp/contests/practice2/submissions/53219502
*/
struct two_sat{
    using vi = vector<int>;
    int n;
    vector<bool> ans;
    scc_graph scc;
    two_sat(int _n=0):n(_n), scc(2*n){}
    // 2x + 0: false, 2x + 1: true
    inline int idx(int i, bool a){return 2*i+a;}
    vector<pii> stk; // optional
    void pop_clause(){ // optional
        assert(stk.size() > 0 && "stk not empty");
        auto[x,y] = stk.back(); stk.pop_back();
        scc.g[x^1].pop_back(); scc.g[y^1].pop_back();
    }
    // (x OR y) <=> (~x -> y AND ~y -> x)
    void add_clause(int x,int y){
        // stk.emplace_back(x, y); // optional
        scc.add_edge(x^1, y); scc.add_edge(y^1, x);
    }
    // (i = a OR j = b) <=> if one is false, the other is true
    void add_clause(int i, bool a, int j, bool b){
        assert(0 <= i && i < n && "i in range");
        assert(0 <= j && j < n && "j in range");
        add_clause(idx(i, a), idx(j, b));
    }
    // ((i == a) == (j == b))   <-> not ( (i == a and j == !b) or (i==!a and j==b) ) <=> (i==!a or j ==b) and (i==a or j==!b)
    void is_equal(int i, bool a, int j, bool b){
        add_clause(i, a, j, !b);
        add_clause(i, !a, j, b);
    }
    void atMostOneNaive(const vector<pair<int,bool>>& v){
        if(v.size() <= 1) return;
        for(int i=0;i<v.size();i++)
            for(int j=i+1;j<v.size();j++)
                add_clause(v[i].first, !v[i].second, v[j].first, !v[j].second);
    }
    int addVar(){
        scc.n += 2; scc.g.resize(scc.n);
        return n++;
    }
    void atMostOne(const vector<pair<int,bool>>& v){
        if(v.size() <= 1) return;
        if(v.size() <= 4){
            atMostOneNaive(v);
            return;
        }
        auto [cur, a] = v[0]; cur = idx(cur, !a);
        for(int i=2;i<v.size();i++){
            int next = addVar();  next = idx(next, true);
            auto [x, b] = v[i]; x = idx(x, b);
            add_clause(cur, x^1);
            add_clause(cur, next);
            add_clause(x^1, next);
            cur = next^1;
        }
        auto [nxt, b] = v[1]; nxt = idx(nxt, b);
        add_clause(cur, nxt ^ 1);
    }
    bool satisfiable() {
        scc.reset();
        auto&&id = scc.scc_id();
        ans = vector<bool>(n);
        for(int i = 0; i < n; i++){
            if(id[2*i] == id[2*i+1]) return false; // (i, false) and (i, true) in same SCC
            ans[i] = id[2*i+1] > id[2*i];
        }
        return true;
    }
    vector<bool> answer(){
        if(ans.size() == 0) satisfiable();
        return ans;
    }
};
