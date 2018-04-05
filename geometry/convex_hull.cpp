#include "geometry.h"

vector<pt> convex_hull(vector<pt> pts) {
    vector<pt> hull;

    int n = sz(pts);
    forn(i, n) {
        if (pts[i] < pts[0]) {
            swap(pts[i], pts[0]);
        }
    }

    pt O = pts[0];
    sort(pts.begin() + 1, pts.end(), [&](const pt &a, const pt &b) -> bool {
        pt Oa = a - O, Ob = b - O;
        T prod = Oa % Ob;
        if (fabs(prod) <= eps) {
            return prod > 0;
        }
        return Oa.sqlen() < Ob.sqlen();
    });
    
    forn(i, n) {
        while (sz(hull) >= 2 && (hull[sz(hull) - 1] - hull[sz(hull) - 2]) % (pts[i] - hull[sz(hull) - 1]) <= 0) {
            hull.pop_back();
        }
        hull.pb(pts[i]);
    }

    return hull;
}

