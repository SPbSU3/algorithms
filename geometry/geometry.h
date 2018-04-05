using T = long double; // or long long
const long double eps = 1e-9;

struct pt {
    T x, y;
    
    pt(): x(0), y(0) {}

    pt(T x_, T y_): x(x_), y(y_) {}

    void read() {
        cin >> x >> y;
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

    bool cmp(const pt &p) const {
        T prod = *this % p;
        if (fabs(prod) <= eps) {
            return prod > 0;
        }
        return this->sqlen() < p.sqlen();
    }
};


