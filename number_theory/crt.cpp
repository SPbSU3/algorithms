#ifdef LOCAL
#define _GLIBCXX_DEBUG
#endif
#include <bits/stdc++.h>

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
using vii = vector<pii>;

#ifdef LOCAL
#define eprintf(args...) fprintf(stderr, args), fflush(stderr)
#else
#define eprintf(args...) ;
#endif

#define FILE "a"


int n, md;
vi a;

bool read() {
    if  (!(cin >> n >> md)) {
        return 0;
    }

    a = vi(n);
    forn (i, n)
        cin >> a[i];

    return 1;
}

vii fact (int x)
{
    vii ans;
    for (int i = 2; i * i <= x; i++)
    if (x % i == 0)
    {
        int cnt = 0;
        while (x % i == 0)
            cnt++, x /= i;
        ans.pb(mp(i, cnt));
    }

    if (x > 1)
        ans.pb(mp(x, 1));

    return ans;
}

int mod = 0, phi_mod = 0, p_mod = 0;

void add (int &a, int b)
{
    a += b;
    if (a >= mod)
        a -= mod;
}
int mult (int a, int b)
{
    return (ll)a * b % mod;
}
int powmod (int a, int pw)
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

int inv (int a)
{
    assert(__gcd(mod, a) == 1);
    int x = powmod(a, phi_mod - 1);
    assert(mult(x, a) == 1);
    return x;
}

//---------------------------------------------------------------
// Finds x, y such that ax + by = gcd(a, b).
// or any other type
using T = int;

T gcdext (T a, T b, T &x, T &y)
{
    if (b == 0)
    {
        x = 1, y = 0;
        return a;
    }

    T res = gcdext (b, a % b, y, x);
    y -= x * (a / b);
    return res;
}

// Returns true if system x = r1 (mod m1), x = r2 (mod m2) has solutions
// false otherwise. In first case we know exactly that x = r (mod m)

bool crt (T r1, T m1, T r2, T m2, T &r, T &m)
{
    if (m2 > m1)
    {
        swap(r1, r2);
        swap(m1, m2);
    }

    T g = __gcd(m1, m2);
    if ((r2 - r1) % g != 0)
        return false;

    T c1, c2;
    assert(gcdext(m1 / g, m2 / g, c1, c2) == 1);
    assert(c1 * (m1 / g) + c2 * (m2 / g) == 1);
    T a = c1;
    a *= (r2 - r1) / g;
    a %= (m2 / g);
    m = m1 / g * m2;
    r = a * m1 + r1;
    r = r % m;
    if (r < 0)
        r += m;

    assert(r % m1 == r1 && r % m2 == r2);
    return true;
}
//-------------------------------------------------------------------

/*
pii crt (int p, int rp, int q, int rq)
{
    assert(__gcd(p, q) == 1);
    if (p < q)
        swap(p, q), swap(rp, rq);

    forn (i, q) if ((rp + i * p) % q == rq)
        return mp(p * q, rp + i * p);

    assert(false);
    return mp(-1, -1);
}
*/
int get_mod (pii pw)
{
    int res = 1;
    forn (i, pw.snd)
        res *= pw.fst;
    return res;
}

struct num
{
    int coef, rem;

    num (int x, int y) : coef(x), rem(y) {}

    num (ll x)
    {
        assert(0 < x);
        coef = 0;
        while (x % p_mod == 0)
            x /= p_mod, coef++;
        rem = x % mod;
    }

    int getrem ()
    {
        int ans = rem;
        forn (i, coef)
        {
            ans = mult(ans, p_mod);
            if (!ans)
                break;
        }
        return ans;
    }

    void print ()
    {
//        eprintf("pw = %d, rem = %d\n", coef, rem);
    }
};

num operator * (num l, num r)
{
    return num (l.coef + r.coef, mult(l.rem, r.rem));
}

num operator / (num l, num r)
{
    return num(l.coef - r.coef, mult(l.rem, inv(r.rem)));
}

struct segtree
{ 
    vi vals;
    int tsz;

    segtree (vi ms)
    {
        tsz = 1;
        while (tsz < sz(ms))
            tsz *= 2;

        vals.resize(2 * tsz);
        forn (i, sz(ms))
            vals[i + tsz] = ms[i];
        ford (i, tsz) if (i)
            vals[i] = vals[2 * i] + vals[2 * i + 1];
    }

    void put (int pos, int what)
    {
        for (pos += tsz; pos > 0; pos >>= 1)
            vals[pos] += what;
    }

    int get (int l, int r)
    {
        int ans = 0;
        for (l += tsz, r += tsz; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1)
                ans += vals[l++];
            if (r & 1)
                ans += vals[--r];
        }

        return ans;
    }
};

vi all_a;
int get (vi m, pii pw)
{
    const int p = pw.fst;
    mod = get_mod(pw);
    p_mod = p;
    assert(mod % p == 0);
    phi_mod = mod - mod / p;
    assert(phi_mod > 0);

    int s = accumulate(all(m), 0);
    const int bnd = s + 3;
    
    vector<num> fact(bnd, num(1)), ifact(bnd, num(1));
    forn (i, bnd) if (i)
    {
        fact[i] = fact[i - 1] * num(i);
        ifact[i] = ifact[i - 1] / num(i);
        num x = fact[i] * ifact[i];
        assert(x.coef == 0 && x.rem == 1);
    }

    num binom = fact[s - 1];
    forn (i, sz(m))
        binom = binom * ifact[m[i]];

    int ans = 0;

    segtree data(m);

    forn (i, n)
    {
        int sum = data.get(0, a[i]);

        if (sum)
        {
            num xadd = num(sum) * binom;
            assert(xadd.coef >= 0);
       
            xadd.print();
            binom.print(); 
            add(ans, xadd.getrem());
        }

        binom = binom * ifact[s - 1];
        if (s != 1)
            binom = binom * fact[s - 2];
        else
            assert(i == n - 1);
        --s;

        assert(m[a[i]] >= 1);
        binom = binom * num(m[a[i]]);
        m[a[i]]--;
        data.put(a[i], -1);
    }
    
    return ans;
}

int pos_at (const vi &v, int what)
{
    int pos = lower_bound(all(v), what) - v.begin();
    assert(pos < sz(v) && v[pos] == what);
    return pos;
}

int solve() {
    vi sort_a = a;
#define a sort_a
    sort(all(a));
    all_a.clear();

    vi m;
    
    for (int i = 0; i < n; )
    {
        int j = i;
        while (j < n && a[j] == a[i])
            j++;

        all_a.pb(a[i]);
        m.pb(j - i);
        i = j;
    }
#undef a

    forn (i, n)
        a[i] = pos_at(all_a, a[i]);

    vii pws = fact(md);
   
    vi rems(sz(pws));
    forn (i, sz(pws))
        rems[i] = get(m, pws[i]);

    assert(!rems.empty());

    int cur_mod = get_mod(pws[0]);
    int cur_rem = rems[0];

    for (int i = 1; i < sz(rems); i++)
    {
        int next_mod = get_mod(pws[i]);
        int next_rem = rems[i];

        int res_mod, res_rem;
       	assert(crt(cur_rem, cur_mod, next_rem, next_mod, res_rem, res_mod));
        
        cur_rem = res_rem;
        cur_mod = res_mod;
    }

/*	
    for (int i = 1; i < sz(rems); i++)
    {
        int next_mod = get_mod(pws[i]);
        int next_rem = rems[i];

        int res_mod, res_rem;
        tie(res_mod, res_rem) = crt(cur_mod, cur_rem, next_mod, next_rem);
        
        assert(res_mod == cur_mod * next_mod);
        assert(res_rem % cur_mod == cur_rem);
        assert(res_rem % next_mod == next_rem);
    
        cur_mod = res_mod;
        cur_rem = res_rem;
    }
*/
    assert(cur_mod == md);
    return (cur_rem + 1) % md;
}

int32_t main() {
    cin.tie(0);
    ios_base::sync_with_stdio(false);

#ifdef LOCAL
    assert(freopen(FILE ".in", "r", stdin));
    assert(freopen(FILE ".out", "w", stdout));
#endif

#ifdef LOCAL
    for (int a = 1; a <= 100; a++)
    for (int ra = 0; ra < a; ra++)
    for (int b = 1; b <= 100; b++)
    for (int rb = 0; rb < b; rb++)
    {
        int rem, msd;
        assert(crt(ra, a, rb, b, rem, msd) == brt);
    } 

    mt19937 rng;
    for (int a = int(1e6); a <= int(1e6) + 100; a++)
    {
        int b = int(1e3) + 1;
        int ra = rng() % a, rb = rng() % b;
        int resr, resm;
        crt(rb, b, ra, a, resr, resm);
        eprintf("here\n");
    }
#endif

    while (read()) {
        int ans = solve();
        cout << ans << endl;
    }
}
