// here we use T = __int128 to store set
// calling expand k times to empty set finds maximum cost set of size exactly k, that is independent in blue and red matroids
// m is number of elements in matroid, e[i].x - their costs
// first return value is new independent set
// second return value is difference between new and old costs
// oracle(set, red) and oracle(set, blue) check whether set lies in red or blue matroid respectively

pair<T, int> expand (T cur_set)
{
    vi in(m);
    forn (i, m)
        in[i] = ((cur_set >> i) & 1);

    vvi g(m);
    forn (i, m) forn (j, m) 
    {
        T swp_mask = (cur_set ^ (T(1) << i) ^ (T(1) << j));
        if (!in[i] && in[j]) 
        {
            if (oracle(swp_mask, red))
                g[i].pb(j);
            if (oracle(swp_mask, blue))
                g[j].pb(i);
        }
    }

    vi from, to;
    forn (i, m) if (!in[i])
    {
        T add_mask = cur_set ^ (T(1) << i);
        if (oracle(add_mask, blue))
            from.pb(i);
        if (oracle(add_mask, red))
            to.pb(i);
    }

    auto get_cost = [&] (int x)
    {       
        const int cost = (!in[x] ? e[x].w : -e[x].w);
        return (m + 1) * cost - 1; 
    };


    vi dist(m, -inf), prev(m, -1);
    for (int x : from)
        dist[x] = get_cost(x);

    queue<int> q;

    vi used(m);
    for (int x : from)
    {
        q.push(x);
        used[x] = 1;
    }

    while (!q.empty())
    {
        int cur = q.front(); used[cur] = 0; q.pop();

        for (int to : g[cur])
        {
            int cost = get_cost(to);
            if (dist[to] < dist[cur] + cost)
            {
                dist[to] = dist[cur] + cost;
                prev[to] = cur;
                if (!used[to])
                {
                    used[to] = 1;
                    q.push(to);
                }
            }
        }
    }

    int best = -inf, where = -1;
    for (int x : to)
    {
        if (dist[x] > best)
        {
            best = dist[x];
            where = x;
        }
    }

    if (best == -inf)
        return mp(cur_set, best);

    while (where != -1)
    {
        cur_set ^= (T(1) << where);
        where = prev[where];
    }

    while (best % (m + 1))
        best++;
    best /= (m + 1);

    assert(oracle(cur_set, red) && oracle(cur_set, blue));
    return mp(cur_set, best);
}
