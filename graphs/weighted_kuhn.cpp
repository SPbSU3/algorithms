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

using ll = long long;
using vi = vector<int>;
using pii = pair<int, int>;

#ifdef LOCAL
#define eprintf(args...) fprintf(stderr, args), fflush(stderr)
#else
#define eprintf(args...) ;
#endif

#define FILE "a"

const int MAXN = (int)111;

int n, m;
int a[MAXN][MAXN];
bool bad[MAXN][MAXN];

bool read() {
    if  (!(cin >> n >> m)) {
        return 0;
    }
    forn(i, n) {
        forn(j, m) {
            cin >> a[i][j];
        }
    }
    return 1;
}

vi g[MAXN];
int cost[MAXN];
int mxc[MAXN], mxr[MAXN];

//---------------------
int tmr, r[MAXN], l[MAXN], used[MAXN];

bool dfs(int v) {
    if (used[v] == tmr) {
        return 0;
    }

    used[v] = tmr;
    for (auto to: g[v]) {
        if (l[to] == -1 || dfs(l[to])) {
            r[v] = to;
            l[to] = v;
            return 1; 
        }
    }

    return 0;
}
//---------------------

ll solve() {
    ll sum = 0;

    ll ans = 0;
    forn(i, n) {
        forn(j, m) {
            sum += a[i][j];
            if (a[i][j] == 0) {
                bad[i][j] = 1;
            } else {
                a[i][j]--;
                ans++;
                bad[i][j] = 0;
            }
        }
    }

    forn(i, n) {
        g[i].clear();
    }

    forn(i, n) {
        mxc[i] = 0;
    }
    forn(i, m) {
        mxr[i] = 0;
    }

    forn(i, n) {
        forn(j, m) {
            mxc[i] = max(mxc[i], a[i][j]);
            mxr[j] = max(mxr[j], a[i][j]); 
        }
    }

    forn(i, n) {
        ans -= mxc[i];
    }
    forn(i, m) {
        ans -= mxr[i];
    }

    forn(i, n) {
        cost[i] = mxc[i];
    }
/*
    forn(i, n) {
        forn(j, m) {
            eprintf("%d%c", a[i][j], " \n"[j == m - 1]);
        }
    }
*/
    forn(i, n) {
        forn(j, m) {
            if (mxc[i] == mxr[j] && !bad[i][j]) {
                g[i].pb(j);
            }
        }
    }
//---------------------
    vi id(n);
    iota(all(id), 0);
    sort(all(id), [&](int i1, int i2) -> bool {
            return cost[i1] > cost[i2];
        });

    forn(i, n) {
        used[i] = 0;
        r[i] = -1;
    }
    forn(i, m) {
        l[i] = -1;
    }

    tmr = 1;
    for (auto i: id) {
        tmr++;
        if (r[i] == -1) {
            dfs(i);
        }
    }

    forn(i, n) {
        if (r[i] != -1) {
            ans += cost[i];
        }
    }
//---------------------
    return sum + ans;
}

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(false);

#ifdef LOCAL
    assert(freopen(FILE ".in", "r", stdin));
    assert(freopen(FILE ".out", "w", stdout));
#endif

    while (read()) {
        cout << solve() << '\n';
    }
}
