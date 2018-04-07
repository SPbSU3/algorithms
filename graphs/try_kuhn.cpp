bool dfs(int v) {
    if (used[v] == tmr) {
        return 0;
    }

    used[v] = tmr;
    for (auto to: g[v]) {
        if (l[to] == -1 || dfs(l[to])) {
            r[v] = to;
            l[to] = v;
            return 1; 
        }
    }

    return 0;
}
