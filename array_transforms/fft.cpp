#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <complex>

using namespace std;

#define forn(i, n) for (int i = 0; i < int(n); ++i)
#define ford(i, n) for (int i = int(n) - 1; i >= 0; --i)
#define all(c) (c).begin(), (c).end()
#define sz(c) int((c).size())
#define pb push_back
#define mp make_pair
#define fst first
#define snd second

#define FILE "a"

#ifdef LOCAL
#define eprintf(args...) fprintf(stderr, args), fflush(stdout)
#else
#define eprintf(args...) ;
#endif

using ll = long long;
using vi = vector<int>;
using pii = pair<int, int>;
using vll = vector<ll>;

// --------------------------------------------

namespace FFT {
    using ld = double; // long double
    using T = complex<ld>;

    const ld PI = acosl(-1.0);

    const int MAXB = 21;
    const int MAXN = 1 << MAXB;

    int rev[MAXN];
    T roots[MAXN];

    bool was_precalc{false};
    void precalc() {
        if  (was_precalc) {
            return;
        }
        was_precalc = 1;

        roots[0] = 1;
        int j = -1;
        for (int i = 1; i < MAXN; ++i) {
            if  ((i & (i - 1)) == 0) { // || true (always use exact angle)
                ld ang = 2 * PI / MAXN * i;
                roots[i] = T(cos(ang), sin(ang));
                ++j;
            } else {
                roots[i] = roots[1 << j] * roots[i ^ (1 << j)];
            }
        }
    }

    T get_root(int k, int n) {
        return roots[k * (MAXN / n)];
    }

    void fft(T* a, int K, bool inv) {
        precalc();

        const int n = 1 << K;

        rev[0] = 0;
        for (int i = 1; i < n; ++i) {
            rev[i] = (rev[i >> 1] >> 1) ^ ((i & 1) << (K - 1)); 
        }

        forn(i, n) {
            if (i < rev[i]) {
                swap(a[i], a[rev[i]]);
            }
        }

        for (int len = 1; len < n; len *= 2) {
            vector<T> rs(len);
            forn(i, len) {
                rs[i] = get_root(i, len * 2);
            }
            for (int i = 0; i < n; i += len * 2) {
                forn(j, len) {
                    const auto& w = rs[j];
                    const auto x = a[i + j];
                    const auto y = a[i + len + j];
                    a[i + j] = x + w * y;
                    a[i + len + j] = x - w * y;
                    if  (inv) {
                        a[i + j] /= 2;
                        a[i + len + j] /= 2;
                    }
                }
            }
        }

        if  (inv) {
            reverse(a + 1, a + n);
        }
    }

    vll mult(const vi& a, const vi& b) {
        int K = 0;
        while ((1 << K) < max(sz(a), sz(b))) {
            ++K;
        }
        ++K;

        vector<T> A(1 << K);
        vector<T> B(1 << K);

        forn(i, sz(a)) {
            A[i] = a[i];
        }
        forn(i, sz(b)) {
            B[i] = b[i];
        }

        fft(A.data(), K, false);
        fft(B.data(), K, false);
        forn(i, sz(A)) {
            A[i] *= B[i];
        }
        fft(A.data(), K, true);

        vll res(sz(A));
        forn(i, sz(A)) {
            res[i] = round(A[i].real());
        }
        while (!res.empty() && !res.back()) {
            res.pop_back();
        }
        return res;
    }

    vll brut_mult(const vi& a, const vi& b) {
        vll c(sz(a) + sz(b));
        forn(i, sz(a)) {
            forn(j, sz(b)) {
                c[i + j] += a[i] * b[j];
            }
        }
        return c;
    }
};
// -------------------------------------------------

double cur_time() {
    return clock() * 1.0 / CLOCKS_PER_SEC;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

#ifdef LOCAL
    freopen(FILE ".in", "r", stdin);
    freopen(FILE ".out", "w", stdout);
#endif

    {
        vi a{1, 2, 3, 4, 5};
        vi b{5, 4, 3, 2, 1};

        vll my = FFT::mult(a, b);
        vll br = FFT::brut_mult(a, b);

        eprintf("\nmy:\n");
        forn(i, sz(my)) {
            eprintf("%lld ", my[i]);
        }
        eprintf("\n");
        
        eprintf("br:\n");
        forn(i, sz(br)) {
            eprintf("%lld ", br[i]);
        }
        eprintf("\n\n");
    }

    {
        srand(123);
        const int MAX = 1e5;
        const int B = 18;
        vi a(1 << B);
        vi b(1 << B);
        forn(i, 1 << B) {
            a[i] = rand() % MAX;
            b[i] = rand() % MAX;
        }

        auto res = FFT::mult(a, b);

        ll h = 0;
        for (auto x : res) {
            h ^= x;
        }
        eprintf("h = %lld\n", h);
        return 0;
    }

    {
        for (int B = 0; B < 21; ++B) {
            const int N = 1 << B;
            const int iters = 4;
            double sum = 0;
            forn(it, iters) {
                vi a(N);
                vi b(N);
                forn(i, N) {
                    a[i] = rand() % 50;
                    b[i] = rand() % 50;
                }

                auto st = cur_time();
                auto c = FFT::mult(a, b);
                sum += cur_time() - st;
            }

            eprintf("B=%d, avg=%.5f\n", B, double(sum / iters));
        }
    }


#ifdef LOCAL
    eprintf("Time %.5f\n", clock() * 1.0 / CLOCKS_PER_SEC);
#endif
}

