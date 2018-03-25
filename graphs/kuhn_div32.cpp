//#define _GLIBCXX_DEBUG
#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define mp make_pair
#define fst first
#define snd second
#define forn(i, n) for (int i = 0; i < (int)(n); ++i)
typedef long long ll;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<ll> vll;
typedef vector<char> vc;
typedef pair<int, int> pii;
typedef vector<pii> vii;
#define sz(c) (int)(c).size()
#define all(c) (c).begin(), (c).end()

const int N = (((int)1e4 + 32) / 32) * 32;
bitset<N> is[N];
int mt[N], rev_mt[N], used[N];
int memn;

bitset<N> nusedmt;
typedef unsigned int uint;

#define ford(i, n) for (int i = n - 1; i >= 0; i--)
bool dfs (int pos)
{
    used[pos] = 1;
    if (rev_mt[pos] != -1)
        nusedmt[rev_mt[pos]] = 0;

    ford (i, N / 32)
    {
        uint *l = (uint*)(&nusedmt) + i;
        uint *r = (uint*)(&is[pos]) + i;
        uint cur = ((*l) & (*r));

        while (cur > 0)
        {
            int bt = __builtin_ctz(cur);
            cur -= (1U << bt);
            bt += i * 32;
            assert(is[pos][bt]);

            if (mt[bt] == -1)
            {
                rev_mt[pos] = bt;
                mt[bt] = pos;
                nusedmt[bt] = 0;
                return true;
            }
        }
    }

    ford (i, N / 32)
    {
        uint *l = (uint*)(&nusedmt) + i;
        uint *r = (uint*)(&is[pos]) + i;
        uint cur = ((*l) & (*r));

        while (cur > 0)
        {
            int bt = __builtin_ctz(cur);
            bt += i * 32;
            assert(is[pos][bt]);

            if (mt[bt] != -1)
            {
                if (!used[mt[bt]] && dfs(mt[bt]))
                {
                    nusedmt[bt] = 0;
                    rev_mt[pos] = bt;
                    mt[bt] = pos;
                    return true;
                }
            }

            cur = ((*l) & (*r));
        }
    }

    return false;
}

void solve (int n)
{
    memn = n;
    int q;
    cin >> q;

    forn (i, N)
        is[i] = ~bitset<N>();

    forn (i, q)
    {
        int x1, x2, y1, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        --x1, --x2, --y1, --y2;
        for (int x = x1; x <= x2; x++)
        for (int y = y1; y <= y2; y++)
            is[x][y] = 0;
    }

    int ans = 0;
    fill(rev_mt, rev_mt + N, -1);
    fill(mt, mt + N, -1);
    vi order(n);
    iota(all(order), 0);

    while (true)
    {
        bool upd = false;
        fill(used, used + N, 0);
        nusedmt.reset();
        forn (i, n)
            nusedmt[i] = 1;
        for (int i : order) if (!used[i] && rev_mt[i] == -1)
        {
            bool was = dfs(i);
            upd |= was;
            ans += was;
        }

        if (!upd)
            break;
    }

    cout << ans << endl;
}

int main()
{
#ifdef LOCAL
    freopen("a.in", "rt", stdin);
    freopen("a.out", "w", stdout);
#endif

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n)
        solve(n);
}
