/*
Find Stronly Connected Components
Tarjan's algorithm
O(V+E)
test: https://atcoder.jp/contests/practice2/submissions/53219246
latest version: https://github.com/KyungmoKu/Algorithms
*/
struct scc_graph{
    using vi = vector<int>;
    int n, pv, scnt;
    bool called;
    vector<vi> g;
    vi dfsn, low, id, stk;
    vector<bool> in_stk;
    scc_graph(int _n=0):n(_n), called(false), g(n){}
    void add_edge(int a,int b){ g[a].push_back(b);}
    void dfs(int x){
        dfsn[x] = low[x] = ++pv;
        stk.push_back(x); in_stk[x] = true;
        for(int nx:g[x]){
            if(!dfsn[nx]){
                dfs(nx);
                low[x] = min(low[x], low[nx]);
            }else if(in_stk[nx]){
                // dfs(nx) didnt end, so dfn[nx] == low[nx]
                low[x] = min(low[x], dfsn[nx]);
            }
        }
        if(low[x] == dfsn[x]){
            int y; do{
                y = stk.back(); stk.pop_back();
                in_stk[y] = false;
                id[y] = scnt;
            }while(y != x);
            scnt++;
        }
    }
    void reset(){called = false;}
    void _get_scc(){
        called = true;
        dfsn = low = id = vector<int>(n); stk = vector<int>();
        in_stk = vector<bool>(n); 
        pv = scnt = 0;
        for(int i=0;i<n;i++) if(!dfsn[i]) dfs(i);
        for(int&k: id) k = (scnt-1) - k;
    }
    vi scc_id(){
        if(!called) _get_scc();
        return id;
    }
    vector<vi> scc(){
        if(!called) _get_scc();
        vector<vi> groups(scnt);
        vi counts(scnt);
        for(int k: id) ++counts[k];
        for(int i=0;i<scnt;i++) groups[i].reserve(counts[i]);
        for(int i=0;i<n;i++) groups[id[i]].push_back(i);
        return groups;
    }
};
