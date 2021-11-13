#include <vector>
#include <iostream>
#include <tuple>
#include <bitset>
struct residual_graph {
    using cap_type = long long;
    struct edge {
        int to;
        cap_type cap;
        int rev;
        edge(int t = 0, cap_type c = 0, int r = 0): to(t), cap(c), rev(r) {}
    };
    int N;
    std::vector<int> iter;
    std::vector<std::tuple<int, int, cap_type>> pool;
    std::vector<edge> edges;
    residual_graph(int N): N(N), iter(N + 1) {}

    void add_edge(int from, int to, cap_type cap, cap_type rev_cap) {
        iter[from]++;
        iter[to]++;
        pool.emplace_back(from, to, cap);
    }
    void build() {
        for (int i = 0; i < N; i++) {
            iter[i + 1] += iter[i];
        }

        edges.resize(pool.size() * 2);

        for (auto &&p : pool) {
            int fi = --iter[std::get<0>(p)];
            int ti = --iter[std::get<1>(p)];
            edges[fi] = edge(std::get<1>(p), std::get<2>(p), ti);
            edges[ti] = edge(std::get<0>(p), 0, fi);
        }
    }
    inline edge &operator[](int i) {
        return edges[i];
    }
};

struct goldberg_tarjan {
    using cap_type = long long;
    struct edge {
        int to;
        cap_type cap;
        edge(int t, cap_type c, int r): to(t), cap(c) {}
    };

    int N;
    residual_graph G;
    std::vector<cap_type> exc;
    std::vector<int> h;
    std::vector<std::vector<int>> que;
    std::vector<int> qi;
    std::vector<int> ei;
    std::vector<int> hcnt;
    std::bitset<10002> inque;
    const cap_type INF = 1e18;

    goldberg_tarjan(int n): N(n), G(n) {}

    void add_edge(int from, int to, cap_type cap, cap_type rev_cap = 0) {
        G.add_edge(from, to, cap, rev_cap);
    }
    void build() {
        G.build();
    }

    void push(int from, int ei) {
        cap_type fl = std::min(exc[from], G[ei].cap);
        G[ei].cap -= fl;
        G[G[ei].rev].cap += fl;
        exc[from] -= fl;
        exc[G[ei].to] += fl;
    }

    void relabel(int v) {
        hcnt[h[v]]--;
        h[v] = N;

        for (int i = G.iter[v]; i < G.iter[v + 1]; i++) {
            auto &e = G[i];

            if (e.cap > 0 && h[v] > h[e.to] + 1) {
                ei[v] = i;
                h[v] = h[e.to] + 1;
            }
        }

        hcnt[h[v]]++;
    }

    int global_relabeling(int t) {
        std::fill(std::begin(h), std::end(h), N);
        std::fill(std::begin(hcnt), std::end(hcnt), 0);

        for (int i = 0; i < N; i++) {
            que[i].clear();
        }

        inque.reset();
        int i = 0, qr = 0;
        std::vector<int> &Q = qi;
        Q[qr++] = t;
        h[t] = 0;
        int hi = 0;

        while (i < qr) {
            int v = Q[i++];
            hi = h[v];
            hcnt[hi]++;

            if (exc[v] > 0 && v != t) {
                que[h[v]].emplace_back(v);
                inque.set(v);
            }

            for (int gi = G.iter[v]; gi < G.iter[v + 1]; gi++) {
                auto &e = G[gi];

                if (G[e.rev].cap > 0 && h[v] + 1 < h[e.to]) {
                    h[e.to] = h[v] + 1;
                    Q[qr++] = e.to;
                }
            }
        }

        std::copy(std::begin(G.iter), std::begin(G.iter) + N, std::begin(ei));
        std::fill(std::begin(qi), std::end(qi), 0);
        return hi;
    }

    void gap_relabeling(int g) {
        for (int v = 0; v < N; v++) {
            if (g < h[v] && h[v] < N) {
                hcnt[h[v]]--;
                h[v] = N;
            }
        }
    }


    cap_type max_flow(int s, int t) {
        exc.assign(N, 0);
        exc[s] = INF;
        h.assign(N, 0);
        int cnt = 0;

        que.resize(N);
        qi.assign(N, 0);
        ei.resize(N);
        hcnt.assign(N + 1, 0);

        global_relabeling(t);

        if (h[s] == N)
            return 0;

        for (int di = h[s]; di >= 0;) {
            if (qi[di] == que[di].size()) {
                di--;
                continue;
            }

            int v = que[di][qi[di]++];
            inque.reset(v);

            if (exc[v] == 0 || v == t)
                continue;

            for (int &i = ei[v]; i < G.iter[v + 1]; i++) {
                auto &e = G[i];

                if (e.cap > 0 && h[v] == h[e.to] + 1) {
                    push(v, i);

                    if (exc[e.to] > 0 && e.to != t && !inque.test(e.to)) {
                        que[h[e.to]].emplace_back(e.to);
                        inque.set(e.to);
                    }

                    if (exc[v] == 0)
                        break;
                }
            }

            if (exc[v] == 0)
                continue;

            relabel(v);

            if (h[v] < N) {
                di = h[v];
                que[h[v]].emplace_back(v);
                inque.set(v);
            }

            if (hcnt[di] == 0) {
                gap_relabeling(di);
                di--;
            }

            if (++cnt % (N * 4) == 0) {
                di = global_relabeling(t);
            }
        }

        return exc[t];
    }
};

