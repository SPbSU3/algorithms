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
 
const int mod = 998244353;
const int root = 3;

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
int powmod (int a, ll pw)
{
    int res = 1;
    while (pw)
    {
        if (pw & 1)
            res = mult(res, a);
        a = mult(a, a);
        pw >>= 1;
    }
    return res;
}
int modinv (int x)
{
    assert(0 < x && x < mod);
    return powmod(x, mod - 2);
}

//--------------------------------------------
void fft (vi &v, bool inv)
{
    for (int i = 1, j = 0; i < sz(v); i++)
    {
        int bit = (sz(v) >> 1);
        while (j & bit)
            j ^= bit, bit >>= 1;
        j ^= bit;
        
        if (i < j)
            swap(v[i], v[j]);
    }

    for (int len = 1; len < sz(v); len *= 2)
    {
        vi pws(len + 1);
        pws[0] = 1;
        // root is primitive root
        pws[1] = powmod(root, (mod - 1) / (2 * len));

        for (int j = 2; j < sz(pws); j++)
            pws[j] = mult(pws[j - 1], pws[1]);

        for (int i = 0; i < sz(v); i += 2 * len)
        {
            forn (j, len)
            {
                int x = v[i + j], y = mult(v[i + j + len], pws[j]);
                v[i + j] = x; 
                add(v[i + j], y);
                v[i + j + len] = x;
                sub(v[i + j + len], y);
            }
        }
    }

    if (inv)
    {
        const int div = modinv(sz(v));
        for (int &x : v)
            x = mult(x, div);
        reverse(v.begin() + 1, v.end());
    }
}

vi fft_prod (vi a, vi b)
{
    int bit = 1;
    while (bit < max(sz(a), sz(b)))
        bit *= 2;
    bit *= 2;

    a.resize(bit);
    b.resize(bit);
    fft(a, false);
    fft(b, false);
    forn (i, sz(a))
        a[i] = mult(a[i], b[i]);
    fft(a, true);
    
    return a;
}
//--------------------------------------------------------------
 
int main() {
#ifdef LOCAL
    freopen(FILE_NAME ".in", "r", stdin);
    freopen(FILE_NAME ".out", "w", stdout);
#endif

    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
 
    return 0;
}
