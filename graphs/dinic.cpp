int dfs(int v, int f) {
    if  (v == TT || !f) {
        return f;
    }
    for (int& i = ptr[v]; i < sz(g[v]); ++i) {
        const int id = g[v][i];
        const int to = es[id].to;
        const int can = es[id].cap - es[id].flow;
        if (level[to] != level[v] + 1 || can < f) {
            continue;
        }
        int pushed = dfs(to, min(can, f));
        if  (!pushed) {
            continue;
        }
        es[id].flow += pushed;
        es[id ^ 1].flow -= pushed;
        return pushed;
    }
    return 0;
}

bool bfs(const int lim) {
    forn(v, N) {
        level[v] = -1;
    }
    level[SS] = 0;
    vi q;
    q.pb(SS);
    forn(i, sz(q)) {
        const int v = q[i];
        for (int id : g[v]) {
            if  (level[es[id].to] == -1 && es[id].cap >= es[id].flow + lim) {
                level[es[id].to] = level[v] + 1;
                q.pb(es[id].to);
            }
        }
    }
    return level[TT] != -1;
}

int max_flow() {
    assert(S != -1);
    assert(T != -1);

    for (auto& e : es) {
        e.flow = 0;
    }

    int ans = 0;
    ford(l, 22) {
        while (bfs(1 << l)) {
            forn(v, N) {
                ptr[v] = 0;
            }
            while (1) {
                int cur = dfs(SS, 1 << l);
                if  (!cur) {
                    break;
                }
                ans += cur;
            }
        }
    }

    return ans;
}

