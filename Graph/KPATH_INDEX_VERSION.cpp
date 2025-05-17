// Index Version
/**
 * Leftist Heap Memory Optimized when max_size is determined.
 * alloc should be initialized as expected max size. (at least 1)
 * pivot should be initialized 0.
 */
template <typename Key, typename Value>
struct LeftistHeap {
    // using self_t = LeftistHeap<Key, Value>;
    int node_rank;
    Key key;
    Value value;
    int left, right;
    LeftistHeap(){}
    LeftistHeap(int rank_, Key key_, Value value_, int left_, int right_)
            : node_rank{rank_}, key{key_}, value{value_}, left{left_}, right{right_} {}
    friend inline int emplace(int rank_, Key key_, Value value_, int left_, int right_,
                              std::vector<LeftistHeap>& alloc, int &pivot){
        if(pivot == (int) alloc.size()) alloc.resize((int) alloc.size() * 2 + 1);
        assert(pivot < (int) alloc.size());
        alloc[pivot] = LeftistHeap(rank_, key_, value_, left_, right_);
        return pivot++;
    }
    friend int heap_insert(int a, const Key k, const Value v,
                           std::vector<LeftistHeap>& alloc, int &pivot) {
        assert(a < (int) alloc.size());
        if (a == -1 or k <= alloc[a].key)
            return emplace(1, k, v, a, -1, alloc, pivot);
        int l = alloc[a].left, r = heap_insert(alloc[a].right, k, v, alloc, pivot);
        assert(r != -1);
        // adjust to l.rank >= r.rank
        if (l == -1 or alloc[l].node_rank < alloc[r].node_rank) std::swap(l, r);
        return emplace(r != -1 ? alloc[r].node_rank + 1 : 0, alloc[a].key, alloc[a].value, l, r, alloc, pivot);
    }
};



// Link: https://github.com/KyungmoKu/Algorithms
// Test: https://judge.yosupo.jp/submission/225582
// Reference: https://judge.yosupo.jp/submission/87297
// MAX_DISTANCE: Infinity for path length

template <typename Distance, typename WeightedGraph>
struct K_Shortest_Paths_Solver{
private:
    const Distance MAX_DISTANCE, BASE_DISTANCE;
    const WeightedGraph& g;
    int n;
    bool is_dag;
    /*
    Information for Recovering path
    First we need previous values for each std::pair<Distance, heap_t*>
    1. heap_t values
    2. After we get the sequence of sidetracks, we need the best vertices
    */
    template <typename T> using min_heap = std::priority_queue<T, std::vector<T>, std::greater<T>>;
    using heap_t = LeftistHeap<Distance, std::pair<int, int>>;
    std::vector<Distance> d;
    std::vector<int> best;
    // for edge (u, v, w),  sidetrack weight will be key, next vertex v will be value
    std::vector<heap_t> alloc;
    int pivot;
    std::vector<int> h; // alloc indices
    std::vector<Distance> distances;
    std::vector<int> path_nodes;
    std::vector<std::pair<Distance, int>> nodes;
    std::vector<int> prev_node;
public:
    explicit K_Shortest_Paths_Solver(const WeightedGraph& g_, bool is_dag_, Distance MAX_DISTANCE_, Distance BASE_DISTANCE_)
            : g(g_), n(g_.size()), is_dag(is_dag_), MAX_DISTANCE(MAX_DISTANCE_), BASE_DISTANCE(BASE_DISTANCE_), pivot(0){}
    // O(m log n)
    auto dijkstra(const WeightedGraph& g_, int s) {
        std::vector<Distance> d_(g_.size(), MAX_DISTANCE);
        std::vector<int> prv(g_.size(), -1);
        min_heap<std::pair<Distance, int>> heap;
        heap.emplace(d_[s] = BASE_DISTANCE, s);
        while (!heap.empty()) {
            auto [dv, v] = heap.top();
            heap.pop();
            if (dv != d_[v]) continue;
            for (auto &[to, w] : g_[v]) {
                if (d_[to] > dv + w) {
                    d_[to] = dv + w;
                    heap.emplace(d_[to], to);
                    prv[to] = v;
                }
            }
        }
        return std::make_pair(d_, prv);
    }
    auto topology_sort(const WeightedGraph& g_){
        const int n_ = g_.size();
        std::vector<int> in_deg(n_, 0);
        for(int u = 0; u < n_; u++)
            for(auto &[v, w]: g_[u])
                in_deg[v]++;
        std::vector<int> order(n_);
        std::queue<int> q;
        for(int u = 0; u < n_; u++)
            if(not in_deg[u])
                q.push(u);
        for(auto &u : order){
            if(q.empty()) {
                assert(0 && "Cycle occured in DAG");
                return order;
            }
            u = q.front();
            q.pop();
            for(auto &[v, w]: g_[u])
                if(--in_deg[v] == 0)
                    q.push(v);
        }
        return order;
    }
    // O(n + m)
    auto shortest_path_dag(const WeightedGraph& g_, int s){
        std::vector<Distance> d_(g_.size(), MAX_DISTANCE);
        std::vector<int> prv(g_.size(), -1);
        d_[s] = BASE_DISTANCE;
        auto order = topology_sort(g_);
        for(auto v: order){
            if(d_[v] == MAX_DISTANCE) continue;
            for(auto &[to, w] : g_[v]){
                if(d_[to] > d_[v] + w){
                    d_[to] = d_[v] + w;
                    prv[to] = v;
                }
            }
        }
        return std::make_pair(d_, prv);
    }
    auto k_shortest_paths(int source, int sink, int k) {
        WeightedGraph g_rev(n);
        for (int u = 0; u < n; ++u)
            for (auto &[v, w] : g[u])
                g_rev[v].push_back({u, w});
        std::pair<std::vector<Distance>, std::vector<int>> info;
        if(is_dag) info = shortest_path_dag(g_rev, sink);
        else info = dijkstra(g_rev, sink);
        std::tie(d, best) = info;
        if (d[source] == MAX_DISTANCE)
            return std::vector<Distance>{};
        std::vector<std::basic_string<int>> tree(n);
        for (int u = 0; u < n; ++u)
            if (best[u] != -1)
                tree[best[u]].push_back(u); // u will adopt best[u]'s heap
        alloc = std::vector<heap_t>{};
        alloc.resize(n);
        pivot = 0;
        h = std::vector<int>(n, -1);
        {
            std::queue<int> q({sink});
            while (!q.empty()) {
                auto u = q.front();
                q.pop();
                bool seen_p = false;
                for (auto [v, w] : g[u]) {
                    if (d[v] == MAX_DISTANCE) continue;
                    auto c = w + d[v] - d[u];
                    if (not seen_p and v == best[u] and c == BASE_DISTANCE) {
                        seen_p = true; // we can only skip once! It's the exact same path.
                        continue;
                    }
                    h[u] = heap_insert(h[u], c, std::make_pair(u, v), alloc, pivot);
                }
                for (auto p : tree[u]) h[p] = h[u], q.push(p); // This works fast since we use index
            }
        }
        // min distance
        distances = std::vector<Distance>{d[source]};
        // distances.reserve(k);

        // last sidetrack index of min_path.
        path_nodes = std::vector<int>{-1};
        // path_nodes.reserve(k);

        // {Distance, sidetrack ptr}
        nodes = std::vector<std::pair<Distance, int>>{};
        // nodes.reserve(3 * k);

        // previous node for each node
        prev_node = std::vector<int>{};
        // prev_node.reserve(3 * k);

        if (h[source] == -1) return distances;

        {
            min_heap<std::tuple<Distance, int, int>> q;
            auto emplace = [&](const Distance& d_, int h_, int pre = -1){
                int cur = nodes.size();
                q.emplace(d_, h_, cur);
                nodes.emplace_back(d_, h_);
                // prev_node.push_back(pre);
            };
            emplace(d[source] + alloc[h[source]].key, h[source]);
            while (!q.empty() and (int) distances.size() < k) {
                auto [cd, ch, cur] = q.top();
                q.pop();
                distances.push_back(cd);
                path_nodes.push_back(cur);
                if (h[alloc[ch].value.second] != -1) emplace(cd + alloc[h[alloc[ch].value.second]].key, h[alloc[ch].value.second], cur); // add value
                if (alloc[ch].left != -1) emplace(cd + alloc[alloc[ch].left].key - alloc[ch].key, alloc[ch].left, prev_node[cur]); // same heap, add difference
                if (alloc[ch].right != -1) emplace(cd + alloc[alloc[ch].right].key - alloc[ch].key, alloc[ch].right, prev_node[cur]); // same heap, add difference
            }
        }
        return distances;
    }
    auto kth_shortest_full_path(int source, int sink, int k, bool call_k_paths = false) {
        if (call_k_paths) k_shortest_paths(source, sink, k + 1);
        std::vector<std::tuple<int, int, Distance>> path{};
        if (k < 0 or k >= (int) path_nodes.size())
            return path;
        std::vector<std::tuple<int, int, Distance>> sidetracks{};
        {
            int cur = path_nodes[k];
            while (cur != -1) {
                auto nd = nodes[cur];
                auto [u, v] = alloc[nd.second].value;
                // sidetrack = d[v] + w - d[u]
                // w = sidetrack + d[u] - d[v];
                auto w = alloc[nd.second].key + d[u] - d[v];
                sidetracks.emplace_back(u, v, w);
                cur = prev_node[cur];
            }
            std::reverse(sidetracks.begin(), sidetracks.end());
        }
        {
            int idx = 0;
            int cur = source;
            using std::get;
            while (cur != sink or idx < (int) sidetracks.size()){
                if(idx < (int) sidetracks.size() and cur == get<0>(sidetracks[idx])){
                    path.push_back(sidetracks[idx]);
                    cur = get<1>(sidetracks[idx]);
                    idx++;
                }else{
                    int nxt = best[cur];
                    path.emplace_back(cur, nxt, d[cur] - d[nxt]);
                    cur = nxt;
                }
            }
        }
        return path;
    }
};
