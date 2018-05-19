#include <iostream>
#include <memory.h>
#include <vector>
#include <numeric>
#include <cassert>

using namespace std;

#define forn(i, n) for (int i = 0; i < int(n); ++i)
#define ford(i, n) for (int i = int(n) - 1; i >= 0; --i)
#define all(c) (c).begin(), (c).end()
#define sz(c) int((c).size())
#define pb push_back
#define mp make_pair
#define fst first
#define snd second

using ll = long long;
using vi = vector<int>;
using pii = pair<int, int>;
using vll = vector<ll>;

#ifdef LOCAL
#define eprintf(args...) fprintf(stderr, args), fflush(stderr)
#else
#define eprintf(args...) ;
#endif

#define FILE "a"

#define TEST 0

string s;

struct query {
    int l, r;
    int id;

    void read(int i) {
        id = i;
#if !TEST
        cin >> l >> r;
        --l;
        --r;
#else
        l = rand() % sz(s);
        r = rand() % sz(s);
        if  (l > r) {
            swap(l, r);
        }
#endif
    }

    bool odd() const {
        return (r - l + 1) % 2 == 1;
    }
};

vector<query> qs;

bool read() {
#if !TEST
    if  (!(getline(cin, s))) {
        return 0;
    }
#else
    int n = 1 + rand() % 10;
    s.clear();
    forn(i, n) {
        s += 'a' + rand() % 3;
    }
#endif

    int m;
#if !TEST
    cin >> m;
#else
    m = 1 + rand() % 10;
#endif

    qs.resize(m);
    forn(i, m) {
        qs[i].read(i);
    }
    return 1;
}


vi d0;
vi d1;

// --------------------------------------
void manacher() {
    d0.assign(sz(s), 0);
    int l = -1;
    int r = -1;
    forn(i, sz(s)) {
        if  (i < r) {
            d0[i] = min(r - i, d0[l + (r - i) - 1]);
        }
        while (i - d0[i] >= 0 && i + d0[i] + 1 < sz(s) && s[i - d0[i]] == s[i + d0[i] + 1]) {
            ++d0[i];
        }
        if (i + d0[i] > r) {
            l = i - d0[i] + 1;
            r = i + d0[i];
        }
    }
     
    d1.assign(sz(s), 0);
    l = -1;
    r = -1;
    forn(i, sz(s)) {
        if  (i < r) {
            d1[i] = min(r - i + 1, d1[l + (r - i)]);
        }
        while (i - d1[i] >= 0 && i + d1[i] < sz(s) && s[i - d1[i]] == s[i + d1[i]]) {
            ++d1[i];
        }
        if  (i + d1[i] - 1 > r) {
            l = i - d1[i] + 1;
            r = i + d1[i] - 1;
        }
    }
}
// --------------------------------------------------------

struct segm_tree {
    vll t;
    vll to_push;
    int sz;

    segm_tree(int S = 0) {
        sz = 1;
        while (sz < S) {
            sz *= 2;
        }
        t.assign(sz * 2, 0);
        to_push.assign(sz * 2, 0);
    }

    void apply(int v, int tl, int tr, ll val) {
        t[v] += val * (tr - tl + 1);
        to_push[v] += val;
    }

    void push(int v, int tl, int tr) {
        if  (!to_push[v]) {
            return;
        }
        int tm = (tl + tr) / 2;
        apply(v * 2, tl, tm, to_push[v]);
        apply(v * 2 + 1, tm + 1, tr, to_push[v]);
        to_push[v] = 0;
    }

    void upd(int v) {
        assert(!to_push[v]);
        t[v] = t[v * 2] + t[v * 2 + 1];
    }

    ll get(int v, int tl, int tr, int l, int r) {
        l = max(l, tl);
        r = min(r, tr);

        if  (l > r) {
            return 0;
        }

        if  (l == tl && r == tr) {
            return t[v];
        }

        push(v, tl, tr);

        int tm = (tl + tr) / 2;
        ll ans = get(v * 2, tl, tm, l, r);
        ans += get(v * 2 + 1, tm + 1, tr, l, r);
        
        upd(v);

        return ans;
    }

    ll get(int l, int r) {
        return get(1, 0, sz - 1, l, r);
    }

    void go_add(int v, int tl, int tr, int l, int r, int val) {
        l = max(l, tl);
        r = min(r, tr);

        if  (l > r) {
           return;
        }

        if  (l == tl && r == tr) {
            apply(v, tl, tr, val);
            return;
        }

        push(v, tl, tr);

        int tm = (tl + tr) / 2;
        go_add(v * 2, tl, tm, l, r, val);
        go_add(v * 2 + 1, tm + 1, tr, l, r, val);

        upd(v);
    }

    void go_add(int l, int r, int val) {
//        eprintf("l=%d, r=%d, val=%d\n", l, r, val);
        go_add(1, 0, sz - 1, l, r, val);
    }
};

vll solve() {
    manacher();

    eprintf("d0:\n");
    forn(i, sz(s)) {
        eprintf("%d ", d0[i]);
    }
    eprintf("\n");

    eprintf("d1:\n");
    forn(i, sz(s)) {
        eprintf("%d ", d1[i]);
    }
    eprintf("\n");

    vll ans(sz(qs));
        
    {
        vector<vector<query>> who_qs(sz(s));
        for (const auto& q : qs) {
            int m = (q.l + q.r) / 2;
            if  (q.odd()) {
                --m;
            }
            if  (m >= 0) {
                who_qs[m].pb(q);
            }
        }

        segm_tree T(sz(s));

        forn(i, sz(s)) {
            if  (d0[i] > 0) {
                int l = i - d0[i] + 1;
                T.go_add(l, i, +1);
            }
            {
                int l = i - d1[i] + 1;
                T.go_add(l, i, +1);
            }

            for (const auto& q : who_qs[i]) {
                ans[q.id] += T.get(q.l, q.r);
            }
        }
    }

    {
        vector<vector<query>> who_qs(sz(s));
        for (const auto& q : qs) {
            int m = (q.l + q.r) / 2;
            ++m;
            if  (m < sz(s)) {
                who_qs[m].pb(q);
            }
        }

        segm_tree T(sz(s));
        ford(i, sz(s)) {
            {
                int r = i + d1[i] - 1;
                T.go_add(i, r, +1);
            } 
            if  (d0[i] > 0) {
                int r = i + d0[i];
                T.go_add(i + 1, r, +1);
            }

            for (const auto& q : who_qs[i]) {
                ans[q.id] += T.get(q.l, q.r);
            }
        }
    }

    for (const auto& q : qs) {
        if  (q.odd()) {
            int m = (q.l + q.r) / 2;
            
            int cur = min(q.r - m + 1, d1[m]);
            ans[q.id] += cur;

            cur = min(q.r - m, d0[m]);
            ans[q.id] += cur;
        }
    }

    return ans;
}

bool pal(string t) {
    forn(i, sz(t) / 2) {
        if  (t[i] != t[sz(t) - i - 1]) {
            return 0;
        }
    }
    return 1;
}

vll brut() {
    vll ans(sz(qs));
    for (const auto& q : qs) {
        for (int a = q.l; a <= q.r; ++a) {
            for (int b = a; b <= q.r; ++b) {
                ans[q.id] += pal(s.substr(a, b - a + 1));
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

        auto my = solve();

        auto br = brut();

        if (my != br) {
            eprintf("FAIL sz(s)=%d, s=%s\n", sz(s), s.data());
            return 0;
        }

        eprintf("OK sz(s)=%d\n", sz(s));
    }
#endif

    while (read()) {
        vll ans = solve();
        for (ll x : ans) {
            cout << x << "\n";
        }
        cout << endl;

        break;
    }
}

