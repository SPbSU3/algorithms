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

const int mod = (int)1e9 + 7;

void add (int &a, int b)
{
    a += b;
    if (a >= mod)
        a -= mod;
}
void sub (int &a, int b)
{
    a -= b;
    if (a < 0)
        a += mod;
}
int mult (int a, int b)
{
    return (ll)a * b % mod;
}

vi brut_conv (const vi &a, const vi &b)
{
    vi ans(sz(a));
    forn (mask, sz(a)) 
    {
        int &cur = ans[mask];
        for (int i = mask; i > 0; i = ((i - 1) & mask))
            add(cur, mult(a[i], b[mask ^ i]));
        add(cur, mult(a[0], b[mask]));
    }
    return ans;
}

// algorithm itself starts here
void mobius (int* a, int n, int sign)
{
    forn (i, n)
    {
        int free = ((1 << n) - 1) ^ (1 << i);
        for (int mask = free; mask > 0; mask = ((mask - 1) & free))
            (sign == +1 ? add : sub)(a[mask ^ (1 << i)], a[mask]);
        add(a[1 << i], a[0]);
    }
}

// maximum number of bits allowed
const int B = 20;

vi fast_conv (vi a, vi b)
{
    assert(!a.empty());
    const int bits = __builtin_ctz(sz(a));
    assert(sz(a) == (1 << bits) && sz(a) == sz(b));

    static int trans_a[B + 1][1 << B];
    static int trans_b[B + 1][1 << B];
    static int trans_res[B + 1][1 << B];

    forn (cnt, bits + 1)
    {
        for (auto cur : {trans_a, trans_b, trans_res})
            fill(cur[cnt], cur[cnt] + (1 << bits), 0);
    }

    forn (mask, 1 << bits)
    {
        const int cnt = __builtin_popcount(mask);
        trans_a[cnt][mask] = a[mask];
        trans_b[cnt][mask] = b[mask];
    }

    forn (cnt, bits + 1)
    {
        mobius(trans_a[cnt], bits, +1);
        mobius(trans_b[cnt], bits, +1);
    }

    // Not really a valid ranked mobius transform! But algorithm works anyway

    forn (i, bits + 1) forn (j, bits - i + 1) forn (mask, 1 << bits)
        add(trans_res[i + j][mask], mult(trans_a[i][mask], trans_b[j][mask]));

    forn (cnt, bits + 1)
        mobius(trans_res[cnt], bits, -1);

    forn (mask, 1 << bits)
    {
        const int cnt = __builtin_popcount(mask);
        a[mask] = trans_res[cnt][mask];
    }

    return a;
}
// algorithm itself ends here

void print (const vi &v)
{
    for (int x : v)
        eprintf("%d ", x);
    eprintf("\n");
}

double time ()
{
    return (double)clock() / CLOCKS_PER_SEC;
}

int main() {
#ifdef LOCAL
    freopen(FILE_NAME ".in", "r", stdin);
    freopen(FILE_NAME ".out", "w", stdout);
#endif

    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);

    mt19937 rng;
    forn (it, 10)
    {
        const int n = rng() % 10;
        vi a(1 << n), b(1 << n);
        forn (i, 1 << n)
        {
            a[i] = rng() % mod;
            b[i] = rng() % mod;
        }

        if (it == 0)
            a = {1, 2, 3, 4}, b = {4, 3, 2, 1};

        vi brut = brut_conv(a, b);
        vi smart = fast_conv(a, b);

//        print(brut);
//        print(smart);

        assert(brut == smart);
    }

    int ans = 0;
    forn (n, 21)
    {
        const int iters = 2;

        double sum = 0;
        vi a(1 << n), b(1 << n);

        forn (it, iters)
        {
            forn (i, 1 << n)
            {
                a[i] = rng() % mod;
                b[i] = rng() % mod;
            }

            double start = time();

            auto res = fast_conv(a, b);
            for (int &x : res)
                add(ans, x);     

            double finish = time();

            sum += finish - start;
        }            

        eprintf("Average time for size = (1 << %d) is %.3f\n", n, double(sum / iters));
    } 

    cerr << "Control sum : " << ans << endl;
 
    return 0;
}
