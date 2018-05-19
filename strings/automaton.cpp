#include <cstdio>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <memory.h>
#include <algorithm>
#include <cassert>

#define forn(i, n) for (int i = 0; i < int(n); ++i)
#define ford(i, n) for (int i = int(n) - 1; i >= 0; --i)
#define all(c) (c).begin(), (c).end()
#define sz(c) int((c).size())
#define mp(x, y) make_pair(x, y)
#define pb(x) push_back(x)
#define fst first
#define snd second

using namespace std;

using ll = long long;
using vi = vector<int>;
using vll = vector<ll>;
using vvi = vector<vi>;

#define FNAME "a"

#ifdef LOCAL
#define eprintf(args...) fprintf(stderr, args), fflush(stderr);
#else
#define eprintf(args...) ;
#endif

#define TEST 1

string S;

bool read() {
#if !TEST
    if  (!(getline(cin, S))) {
        return 0;
    }
#else
    int n = 1 + rand() % 50;
    S.clear();
    forn(i, n) {
        S += 'a' + rand() % 10;
    }
#endif
    return 1;
}

// ------------------------------------------

const int A = 26;

struct node;
using nodePtr = node*;
struct node {
    nodePtr to[A];
    nodePtr link;
    int len;

    node() {
        memset (to, 0, sizeof to);
        link = 0;
        len = 0;
    }

    void* operator new(size_t);
};

const int MAXN = 1e5 + 10;
node pool[MAXN * 3];
int ptr_pool;

void* node::operator new(size_t) {
    pool[ptr_pool] = node();
    return pool + ptr_pool++;
}

nodePtr get_node(int id) {
    assert(0 <= id && id < ptr_pool);
    return pool + id;
}

int id_node(nodePtr v) {
    return v ? v - pool : -1;
}

void add_c(const nodePtr& root, nodePtr& last, int c) {
    nodePtr cur = new node();
    cur->len = last->len + 1;
    nodePtr p = last;
    last = cur;
    for (; p && !p->to[c]; p = p->link) {
        p->to[c] = cur;
    }
     
    if  (!p) {
        cur->link = root;
        return;
    }

    nodePtr q = p->to[c];
    if  (q->len == p->len + 1) {
        cur->link = q;
        return;
    }

    nodePtr clone = new node(*q);
    clone->len = p->len + 1;
    cur->link = q->link = clone;
    for (; p && p->to[c] == q; p = p->link) {
        p->to[c] = clone;
    }
}

vector<nodePtr> build(const string& s) {
    const int ptr_start = ptr_pool;
    nodePtr root = new node();
    auto last = root;
    for (const auto& ch : s) {
        add_c(root, last, ch - 'a');
    }    
    vector<nodePtr> nodes;
    for (int i = ptr_start; i < ptr_pool; ++i) {
        nodes.pb(get_node(i));
    }
    return nodes;
}
// ----------------------------------------------------

ll solve() {
    ptr_pool = 0;
    auto nodes = build(S);
    assert(nodes.front()->len == 0);
    ll ans = 0;
    for (const auto& v : nodes) {
        if  (v->link) {
            ans += v->len - v->link->len;
        }
    }
    return ans;
}

ll brut() {
    set<string> all;
    forn(i, sz(S)) {
        for (int j = i; j < sz(S); ++j) {
            all.insert(S.substr(i, j - i + 1));
        }
    }
    return sz(all);
}

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(false);

#ifdef LOCAL
    freopen(FNAME ".in", "r", stdin);
    freopen(FNAME ".out", "w", stdout);
#endif

#if TEST
    while (1) {
        read();

        ll my = solve();

        ll br = brut();

        if  (my != br) {
            eprintf("my=%lld, br=%lld\n", my, br);
            eprintf("S=%s\n", S.data());
            return 0;
        }
        eprintf("OK len=%d\n", sz(S));
    }
#endif

    while (read()) {
        solve();
    }

    return 0;
}

