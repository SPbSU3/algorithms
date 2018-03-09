// directed spanning tree in O(Elog^2{E} + V)

#include <bits/stdc++.h>

using namespace std;

#define forn(i, n) for (int i = 0; i < int(n); ++i)
#define ford(i, n) for (int i = int(n) - 1; i >= 0; --i)
#define all(c) (c).begin(), (c).end()
#define sz(c) int((c).size())
#define pb push_back
#define mp make_pair
#define fst first
#define snd second

#define FILE "a"

#ifdef LOCAL
#define eprintf(args...) fprintf(stderr, args), fflush(stdout)
#else
#define eprintf(args...) ;
#endif

using ll = long long;
using vi = vector<int>;
using pii = pair<int, int>;

struct Edge {
    int v, to, w, id;

    void read(int i) {
        cin >> v >> to >> w;
        --v;
        --to;
        id = i;
    }

    void add_w(int aux_w) {
        w += aux_w;
    }

    bool operator < (const Edge& e) const {
        return mp(w, id) < mp(e.w, e.id);
    }
};

int n, m;
vector<Edge> es;

bool read() {
    if  (!(cin >> n >> m)) {
        return 0;
    }
    es.resize(m);
    forn(i, m) {
        es[i].read(i);
    }
    return 1;
}

struct Edges {
    using Ptr = shared_ptr<set<Edge>>;

    Ptr es;
    int val;

    Edges() {
        es = Ptr(new set<Edge>());
        val = 0;
    }

    Edges merge(Edges& other) {
        if  (sz(*es) < sz(*other.es)) {
            return other.merge(*this);
        }
        for (auto e : *other.es) {
            e.add_w(other.val);
            insert(e);
        }
        return *this;
    }

    void insert(Edge e) {
        e.add_w(-val);
        es->insert(e);
    }

    void add_val(const int& aux) {
        val += aux;
    }

    Edge get_min() const {
        assert(!es->empty());
        Edge e = *es->begin();
        e.add_w(val);
        return e;
    }

    void erase_min() {
        assert(!es->empty());
        es->erase(es->begin());
    }
};

struct DSU {
    vi par;

    DSU(int n = 0) {
        par.assign(n, -1);
    }

    int get(int x) {
        return par[x] < 0 ? x : par[x] = get(par[x]);
    }

    bool unite(int x, int y) {
        x = get(x);
        y = get(y);
        if  (x == y) {
            return 0;
        }
        if  (-par[x] < -par[y]) {
            swap(x, y);
        }
        par[x] += par[y];
        par[y] = x;
        return 1;
    }
};

void solve() {
    vector<Edges> gt(n);
    DSU dsu(n);

    for (const auto& e : es) {
        gt[e.to].insert(e);
    }

    vi comp(n);
    iota(all(comp), 0);
    int comps = n;
    vi par_comp(2 * n, -1);
    vi when_v(n, -1);
    int timer = 0;
    vector<pii> ans; // edge_id, comp_to
    ford(vv, n) {
        int v = dsu.get(vv);
        if (dsu.get(0) == v) {
            continue;
        }

        ++timer;
        vi st;
        while (v != dsu.get(0)) {
            assert(dsu.get(v) == v);
            st.pb(v);
            when_v[v] = timer;

            int from = -1;
            while (1) {
                Edge best = gt[v].get_min();
                from = dsu.get(best.v);
                if  (from == v) {
                    gt[v].erase_min();
                    continue;
                }
                gt[v].add_val(-best.w);
                ans.pb({best.id, comp[v]});
                break;
            }
            assert(from != -1);
            assert(dsu.get(from) == from);

            if  (when_v[from] != timer) {
                v = from;
                continue;
            }

            vi us;
            do {
                assert(!st.empty());
                us.pb(st.back());
                st.pop_back();
            } while (us.back() != from);

            const int new_comp = comps++;
            for (int u : us) {
                par_comp[comp[u]] = new_comp;
            }
            forn(i, sz(us) - 1) {
                const int u = dsu.get(us[i]);
                if  (i + 1 < sz(us)) {
                    const int u1 = us[i + 1];
                    assert(dsu.unite(u, u1));
                    gt[dsu.get(u)] = gt[u].merge(gt[u1]);
                }
            }
            comp[dsu.get(us[0])] = new_comp;

            v = dsu.get(from);
        }

        for (int u : st) {
            dsu.unite(0, u);
        }
    }

    forn(v, n) {
        assert(dsu.get(v) == dsu.get(0));
    }

    vi used(2 * n, 0);
    vector<Edge> true_ans;
    ford(i, sz(ans)) {
        const int id = ans[i].fst;
        const int dest = ans[i].snd;
        if  (!used[dest]) {
            int c = es[id].to;
            while (c != dest) {
                used[c] = 1;
                c = par_comp[c];
                assert(c != -1);
            }
            used[dest] = 1;
            true_ans.pb(es[id]);
        }
    }

    ll w = 0;
    for (const auto& e : true_ans) {
        w += e.w;
    }

    eprintf("%lld\n", w);
    for (const auto& e : true_ans) {
        cout << e.id + 1 << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

#ifdef LOCAL
    freopen(FILE ".in", "r", stdin);
    freopen(FILE ".out", "w", stdout);
#endif

    while (read()) {
        solve();
    }

#ifdef LOCAL
    eprintf("Time %.5f\n", clock() * 1.0 / CLOCKS_PER_SEC);
#endif
}

