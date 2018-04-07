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

#define FILE "a"

#ifdef LOCAL
#define eprintf(args...) fprintf(stderr, args), fflush(stdout)
#else
#define eprintf(args...) ;
#endif

using ll = long long;
using vi = vector<int>;
using pii = pair<int, int>;

using ld = long double;
using T = ld; // or ll
const ld eps = 1e-9;

bool eq(ld a, ld b) {
    return fabs(a - b) < eps;
}

#define less sdfsdgsdgsdfg
bool less(ld a, ld b) {
    return a <= b - eps;
}

bool lessE(ld a, ld b) {
    return a < b + eps;
}

struct pt {
    T x, y;
    
    pt(): x(0), y(0) {}

    pt(T x_, T y_): x(x_), y(y_) {}

    void read() {
        cin >> x >> y;
    }

    void print() const {
        eprintf("%.6lf %.6lf", (double)x, (double)y);
    }

    pt operator+(const pt &p) const {
        return pt(x + p.x, y + p.y);
    }

    pt operator-(const pt &p) const {
        return pt(x - p.x, y - p.y);
    }

    pt operator*(T k) const {
        return pt(x * k, y * k);
    }

    bool operator==(const pt &p) const {
        return x == p.x && y == p.y;
    }

    T operator*(const pt &a) const {
        return x * a.x + y * a.y;
    }

    T operator%(const pt &a) const {
        return x * a.y - y * a.x;
    }

    bool operator<(const pt &p) const {
        if (x != p.x) {
            return x < p.x;
        }
        return y < p.y;
    }

    T sqlen() const {
        return x * x + y * y;
    }

    ld len() const {
        return sqrt(sqlen());
    }

    ld dist(const pt &p) const {
        return (*this - p).len();
    }

    int half() const {
        return (less(0, y) || (eq(0, y) && less(0, x))) ? 0 : 1;
    }

    bool cmp(const pt &p) const {
        T prod = *this % p;
        if (fabs(prod) <= eps) {
            return prod > 0;
        }
        return this->sqlen() < p.sqlen();
    }

// only for doubles
    pt norm() {
        ld L = len();
        assert(L > eps);
        return pt(x / L, y / L);
    }
};

//---------------------
int cmp_v(const pt &a, const pt &b) {
    int ah = a.half();
    int bh = b.half();
    if (ah != bh) {
        return (ah < bh ? -1 : +1);
    }
    if (!eq(a % b, 0)) {
        return (less(0, a % b) ? -1 : +1);
    }
    return 0;
}

struct directed_line {
    pt o, v;
    int id;

    directed_line() {}

    directed_line(pt _o, pt _v): o(_o), v(_v) {}

    // converts semiplane Ax+By+C>0 to semiplane v%(P-o)>0
    directed_line(ld A, ld B, ld C) {
        if (!eq(B, 0)) {
            o = pt(0, -C / B); 
        } else {
            o = pt(-C / A, 0);
        }
        v = pt(B, -A).norm();
    } 

    ld dist(const pt &p) const {
        return v % (p - o);
    }

    bool operator<(const directed_line& L) const {
        int r = cmp_v(v, L.v);
        if (r != 0) {
            return r < 0;
        }
        ld d = dist(L.o);

        return less(0, d);
    }

    pt intersect(const directed_line &L) const {
        ld a11 = v.x, a12 = -L.v.x, b1 = L.o.x - o.x;
        ld a21 = v.y, a22 = -L.v.y, b2 = L.o.y - o.y;
        ld det = a11 * a22 - a12 * a21;
        ld t = (b1 * a22 - a12 * b2) / det;
        return o + v * t;
	}
};

ld plane_int(vector<directed_line> a) {
    sort(all(a));

    int n = sz(a);
    int ptr = 0;
    for (int i = 0; i < n; ) {
        int j = i;
        for (; i < n && cmp_v(a[j].v, a[i].v) == 0; i++);
        a[ptr++] = a[i - 1];
    }

    n = ptr;
    a.resize(n);
    forn(i, n) {
        a[i].id = i;
    }

    bool flag[2];
    flag[0] = flag[1] = 0;
    forn(i, n) {
        flag[a[i].v.half()] = 1;
    }
    if (!flag[0] || !flag[1]) {
        return -1;
    }

    forn(i, n) {
        pt v = a[i].v;
        pt u = a[(i + 1) % n].v;
        if (eq(v % u, 0) && less(v * u, 0)) {
            if (lessE(a[i].dist(a[(i + 1) % n].o), 0)) {
                return 0;
            }
            return -1;
        }
        if (less(v % u, 0)) {
            return -1;
        }
    }

    ptr = 0;
    vector<directed_line> st(n * 2);
    forn(it, 2) {
        forn(i, n) {
            for (; ptr >= 2; ptr--) {
                pt P = st[ptr - 1].intersect(a[i]);
                if (less(0, st[ptr - 2].dist(P))) {
                    break;
                }
            }
            st[ptr++] = a[i];

            if (ptr >= 2 && lessE(st[ptr - 2].v % st[ptr - 1].v, 0)) {
                return 0;
            }
        }
    }

    vi used(n, -1);
    int lf = -1, rg = -1;
    forn(i, ptr) {
        if (used[st[i].id] == -1) {
            used[st[i].id] = i;
        } else {
            lf = used[st[i].id];
            rg = i;
            break;
        }
    }

    a.clear();
    for (int i = lf; i < rg; i++) {
        a.pb(st[i]);
    }

    vector<pt> res;
    forn(i, sz(a)) {
        res.pb(a[i].intersect(a[(i + 1) % sz(a)]));
    }

    ld area = 0;
    forn(i, sz(res)) {
        area += res[i] % res[(i + 1) % sz(res)];
    }
    return area / 2;
}
//---------------------

int n;
vector<directed_line> L;

bool read() {
    if (!(cin >> n)) {
        return 0;
    }
    L.clear();
    L.resize(n);
    forn(i, n) {
        ld A, B, C;
        cin >> A >> B >> C;
        L[i] = directed_line(A, B, C);
    }
    return 1;
}

ld solve() {
    return plane_int(L);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

#ifdef LOCAL
    freopen(FILE ".in", "r", stdin);
    freopen(FILE ".out", "w", stdout);
#endif

    cout.precision(30);
    while (read()) {
        cout << (double)solve() << '\n';
    }

#ifdef LOCAL
    eprintf("Time %.5f\n", clock() * 1.0 / CLOCKS_PER_SEC);
#endif
}
