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
