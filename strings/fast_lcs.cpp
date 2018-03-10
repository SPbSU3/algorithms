//#pragma GCC optimize("O3")
//#pragma GCC target("sse4.1")
#ifdef LOCAL
#define _GLIBCXX_DEBUG
#endif
#include <bits/stdc++.h>
 
using namespace std;
 
#define forn(i, n) for (int i = 0; i < int(n); ++i)
#define forab(i, a, b) for (int i = int(a); i < int(b); ++i)
#define ford(i, n) for (int i = int(n) - 1; i >= 0; --i)
#define sz(c) int((c).size())
#define all(c) (c).begin(), (c).end()
#define mp(x, y) make_pair(x, y)
#define pb push_back
#define fst first
#define snd second
 
using ll = long long;
using vi = vector<int>;
using vll = vector<ll>;
using pii = pair<int, int>;
using vvi = vector<vi>;
using vii = vector<pii>;

#ifdef LOCAL
#define eprintf(args...) fprintf(stderr, args), fflush(stderr)
#else
#define eprintf(...) ;
#endif

#define FILE_NAME "a"

void remax (int &x, int y)
{
    x = max(x, y);
}

int brut_lcs (const string &s, const string &t)
{
    vvi dp(sz(s) + 1, vi(sz(t) + 1));
    forn (i, sz(s) + 1) forn (j, sz(t) + 1)
    {
        if (i < sz(s))
            remax(dp[i + 1][j], dp[i][j]);
        if (j < sz(t))
            remax(dp[i][j + 1], dp[i][j]);
        if (i < sz(s) && j < sz(t) && s[i] == t[j])
            remax(dp[i + 1][j + 1], dp[i][j] + 1);
    }
    
    return dp[sz(s)][sz(t)];
}

// algorithm itself starts here
// assumes that strings consist of lowercase latin letters
const int M = ((int)1e5 + 64) / 32 * 32;
// maximum value of m
using bs = bitset<M>;
using uint = unsigned int;
const ll bnd = (1LL << 32);

// WARNING: invokes undefined behaviour of modifying ans through pointer to another data type (uint)
// seems to work, but be wary
bs sum (const bs &bl, const bs &br)
{
    const int steps = M / 32;
    const uint* l = (uint*)&bl;
    const uint* r = (uint*)&br;
    
    bs ans;
    uint* res = (uint*)&ans;

    int carry = 0;
    forn (i, steps)
    {
        ll cur = ll(*l++) + ll(*r++) + carry;
        carry = (cur >= bnd);
        cur = (cur >= bnd ? cur - bnd : cur);
        *res++ = uint(cur); 
    }

    return ans;
}

int fast_lcs (const string &s, const string &t)
{
    const int m = sz(t);
    const int let = 26;

    vector<bs> has(let);
    vector<bs> rev = has;

    forn (i, m)
    {
        const int pos = t[i] - 'a';
        has[pos].set(i);
        forn (j, let) if (j != pos)
            rev[j].set(i);
    }

    bs row;
    forn (i, m)
        row.set(i);

    int cnt = 0;
    for (char ch : s)
    {
        const int pos = ch - 'a';
        
        bs next = sum(row, row & has[pos]) | (row & rev[pos]);
        cnt += next[m];
        next[m] = 0;

        row = next;
    }

    return cnt;
}
// algorithm itself ends here

double time ()
{
    return (double)clock() / CLOCKS_PER_SEC;
}

string rand_string (int len)
{
    static mt19937 rng;
    string ans;
    forn (i, len)
        ans += rng() % 4 + 'a';
    return ans;
}

int main() {
#ifdef LOCAL
    freopen(FILE_NAME ".in", "r", stdin);
    freopen(FILE_NAME ".out", "w", stdout);
#endif

    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);

    mt19937 rng;
    forn (it, 50)
    {
        const int n = rng() % 100 + 1;
        const int m = rng() % 100 + 1;

        const string s = rand_string(n);
        const string t = rand_string(m);

        int br = brut_lcs(s, t);
        int my = fast_lcs(s, t);

        eprintf("s = %s, t = %s\n", s.c_str(), t.c_str());
        eprintf("br = %d, my = %d\n", br, my);

        assert(br == my); 
    }

    const int step = (int)1e4;
    int ans = 0;
    for (int n = step; n <= step * 10; n += step)
    {
        const int iters = 2;
        double sum = 0;

        forn (it, iters)
        {
            string s = rand_string(n);
            string t = rand_string(n);        

            double start = time ();
            ans += fast_lcs(s, t); 
            sum += time() - start;
        }

        eprintf("Average time for size = %d is %.3f\n", n, double(sum / iters));
    } 

    cerr << "Control sum : " << ans << endl;
 
    return 0;
}
