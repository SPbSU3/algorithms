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

vi brut_mult (const vi &a, const vi &b)
{
    vi ans(sz(a));
    forn (i, sz(a)) forn (j, sz(b))
        add(ans[i ^ j], mult(a[i], b[j]));
    return ans;
}

// -------------------------------------------
// algorithm itself starts here
void hadamard (int *a, int len)
{
    if (len == 1)
        return;

    assert(len % 2 == 0);
    const int half = len / 2;
    hadamard(a, half);
    hadamard(a + half, half);

    forn (i, half)
    {
        int x = a[i], y = a[i + half];
        add(a[i], y);
        a[i + half] = x;
        sub(a[i + half], y);
    }
}

vi had_mult (vi a, vi b)
{
    hadamard(a.data(), sz(a));
    hadamard(b.data(), sz(b));
    forn (i, sz(a))
        a[i] = mult(a[i], b[i]);

    int pw = 1;
    {
        const int inv2 = (mod + 1) / 2;
        for (int i = 1; i < sz(a); i *= 2)
            pw = mult(pw, inv2);
    }
    // or just pw = inv(sz(a)), if you have inv function. For integers, just divide by sz(a) in the end, all numbers will divide exactly

    hadamard(a.data(), sz(a));
    for (int &x : a)
        x = mult(x, pw);

    return a;
}
// algorithm itself ends here
// ------------------------------------------------------------

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

        vi brut = brut_mult(a, b);
        vi smart = had_mult(a, b);

//        print(brut);
//        print(smart);

        assert(brut == smart);
    }

    int ans = 0;
    forn (n, 23)
    {
        const int iters = 10;

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

            auto res = had_mult(a, b);
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
