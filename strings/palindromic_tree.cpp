#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <memory.h>
#include <cassert>
#include <tuple>

using namespace std;

#define forn(i, n) for (int i = 0; i < int(n); ++i)
#define ford(i, n) for (int i = int(n) - 1; i >= 0; --i)
#define all(c) (c).begin(), (c).end()
#define sz(c) int((c).size())
#define fst first
#define snd second
#define pb push_back
#define mp make_pair

using ll = long long;
using vi = vector<int>;
using pii = pair<int, int>;

#ifdef LOCAL
#define eprintf(args...) fprintf(stderr, args), fflush(stderr)
#else
#define eprintf(args...) ;
#endif

#define FILE "a"

#define TEST 0

string S, T;

bool read() {
#if !TEST
    if  (!(getline(cin, S))) {
        return 0;
    }
    getline(cin, T);
#else
    int n = 1 + rand() % 20;
    S.clear();
    forn(i, n) {
        S += 'A' + rand() % 5;
    }

    int m = 1 + rand() % 20;
    T.clear();
    forn(i, m) {
        T += 'A' + rand() % 5;
    }
#endif
    return 1;
}

// -------------------------------------------

const int A = 26;

struct node;
using nodePtr = node*;
struct node {
    nodePtr to[A];
    nodePtr link;
    int len;
    int cnt;

    node() {
        memset (to, 0, sizeof to);
        link = 0;
        cnt = 0;
    }

    void* operator new(size_t);
};

const int MAXN = 5e4 + 10;
node pool[2 * MAXN];
int ptr_pool;

void* node::operator new(size_t) {
    pool[ptr_pool] = node();
    return pool + ptr_pool++;
}

nodePtr get_node(int id) {
    return pool + id;
}

int id_node(nodePtr v) {
    return v ? v - pool : -1;
}

void print(nodePtr v, string s = "") {
    eprintf("v=%d, link=%d, s=%s, cnt=%d\n", id_node(v), id_node(v->link), s.data(), v->cnt);
    forn(c, A) {
        nodePtr to = v->to[c];
        if (!to) {
            continue;
        }
        string C(1, 'A' + c);
        string ns = s;
        if  (v->len == -1) {
            ns = C;
        } else {
            ns = C + s + C;
        }
        print(to, ns);
    } 
}

// <= sz(s) nodes
pair<nodePtr, nodePtr> build(const string& s) {
    const int ptr_start = ptr_pool;

    nodePtr root_even = new node();
    root_even->len = 0;
    nodePtr root_odd = new node();
    root_odd->len = -1;
    root_even->link = root_odd;

    nodePtr last = root_even;
    forn(i, sz(s)) {
        const int c = s[i] - 'A';

        auto get_matching = [&](nodePtr v) {
            for (; v; v = v->link) {
                const int j = i - v->len - 1;
                if  (j >= 0 && s[i] == s[j]) {
                    return v;
                }
            }
            return v;
        };

        // new state
        last = get_matching(last);
        assert(last);
        nodePtr nw = 0;
        if  (last->to[c]) {
            nw = last->to[c];
        } else {
            nw = new node();
            nw->len = last->len + 2;
            last->to[c] = nw;
        }

        // link
        last = get_matching(last->link);
        if  (!last) {
            nw->link = root_even;
        } else {
            assert(last->to[c]);
            nw->link = last->to[c];
        }
        assert(nw->link);

        last = nw;
        last->cnt++;
    }

    vector<nodePtr> nodes;
    for (int i = ptr_start; i < ptr_pool; ++i) {
        nodes.pb(get_node(i));
    }
    sort(all(nodes), [&](const nodePtr& v, const nodePtr& u) { return v->len > u->len; });
    
    for (nodePtr v : nodes) {
        nodePtr u = v->link;
        if  (!u) {
            continue;
        }
        u->cnt += v->cnt;
    }

///    eprintf("even:\n");
///    print(root_even);
///    
///    eprintf("odd:\n");
///    print(root_odd);
///
///    eprintf("\n");

    return mp(root_even, root_odd);
}

// -------------------------------------------------------

void dfs(nodePtr v, nodePtr u, ll& ans) {
    if  (v->len > 0) {
        ll cur = v->cnt * 1ll * u->cnt;
        ans += cur;
//        eprintf("v=%d, u=%d\n", id_node(v), id_node(u));
//        eprintf("ans += %lld\n", cur);
    }

    forn(c, A) {
        nodePtr vv = v->to[c];
        nodePtr uu = u->to[c];

        if  (!vv || !uu) {
            continue;
        }

        dfs(vv, uu, ans);
    }
}

ll solve() {
    ptr_pool = 0;

    nodePtr r0_S, r1_S;
    tie(r0_S, r1_S) = build(S);

    nodePtr r0_T, r1_T;
    tie(r0_T, r1_T) = build(T);

    ll ans = 0;
    dfs(r0_S, r0_T, ans);
    dfs(r1_S, r1_T, ans);

    return ans;
}

bool pal(const string& s) {
    forn(i, sz(s) / 2) {
        if  (s[i] != s[sz(s) - i - 1]) {
            return 0;
        }
    }
    return 1;
}

ll brut() {
    ll ans = 0;

    forn(i, sz(S)) {
        for (int j = i; j < sz(S); ++j) {
            forn(k, sz(T)) {
                for (int l = k; l < sz(T); ++l) {
                    if  (j - i != l - k) {
                        continue;
                    }

                    string s = S.substr(i, j - i + 1);
                    string t = T.substr(k, l - k + 1);

                    if  (pal(s) && s == t) {
                        ++ans;
                    }
                }
            }
        }
    }

    return ans;
}

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(false);

#ifdef LOCAL
    assert(freopen(FILE ".in", "r", stdin));
    assert(freopen(FILE ".out", "w", stdout));
#endif

#if TEST
    while (1) {
        read();

        ll my = solve();

        ll br = brut();

        if  (my != br) {
            eprintf("my=%lld, br=%lld\n", my, br);
            eprintf("S=%s, T=%s\n", S.data(), T.data());
            return 0;
        } else {
            eprintf("OK S=%s, T=%s\n", S.data(), T.data());
        }
    }
#endif

    while (read()) {
        ll ans = solve();
        cout << ans << "\n";

//        break;
    }
}

