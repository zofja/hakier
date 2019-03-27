#include <iostream>
#include <set>
#include <utility>

using namespace std;
using Pair = pair<int, int>;

const int MAXN = 100 * 1000 + 5;
const int MAXT = 1 << 17;
const int INF = MAXN;
int Base = MAXT;

struct Node {

    int id;
    int end;

    bool operator==(const Node &other) const {
        return end == other.end;
    }

    bool operator<(const Node &other) const {
        return end < other.end;
    }

    bool operator>(const Node &other) const {
        return end > other.end;
    }

    Node &operator=(const Node &other) = default;

};

int Begs[MAXN];
set<Pair> Ends[MAXN];
Node Tree[2][2 * MAXT]; // 0 – MAX, 1 – MIN

int n;

void update(int t, int v, int x, int id) {

    v += Base - 1;

    Tree[t][v] = {id, x};

    v /= 2;
    while (v) {
        if (t == 0) {
            Tree[t][v] = max(Tree[t][v << 1], Tree[t][(v << 1) + 1]);
        } else {
            Tree[t][v] = min(Tree[t][v << 1], Tree[t][(v << 1) + 1]);
        }
        v /= 2;
    }
}

Node query(int t, int v, int lo, int hi, int l, int r) {

    if (lo > r or hi < l) {
        return {-1, (t == 0 ? -INF : INF)};
    }

    if (lo >= l and hi <= r) {
        return Tree[t][v];
    }

    int mid = (lo + hi) / 2;
    Node q1 = query(t, v << 1, lo, mid, l, r);
    Node q2 = query(t, (v << 1) + 1, mid + 1, hi, l, r);

    if (t == 0) {
        return max(q1, q2);
    } else {
        return min(q1, q2);
    }
}

int main() {

    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    cin >> n;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < MAXT * 2; j++) {
            Tree[i][j].id = -1;
            Tree[i][j].end = i == 0 ? -INF : INF;
        }
    }

    for (int i = 1; i <= n; i++) {
        int a, b;
        cin >> a >> b;
        Begs[i] = a;
        Ends[a].insert({b, i});
        update(0, a, Ends[a].rbegin()->first, Ends[a].rbegin()->second);
        update(1, a, Ends[a].begin()->first, Ends[a].begin()->second);
    }

    int q;
    cin >> q;

    for (int i = 0; i < q; i++) {

        string t;
        int x, y;
        cin >> t >> x >> y;

        // in   x y czy istnieje przedział o początku >= x minimalny koniec <= y
        // over x y czy istnieje przedział o początku <= x maksymalny koniec >= y
        // none x y czy istnieje przedział o początku <= x - 1 o minimalnym końcu <= x - 1 lub czy istnieje przedział o początku >= y + 1
        // some x y czy istnieje przedział o początku <= y maksymalny koniec >= x

        Node que{};
        int res = -1;

        if (t == "in") {

            que = query(1, 1, 1, Base, x, Base);

            if (que.id > 0 and que.end <= y) {
                res = que.id;
            }

        } else if (t == "over") {

            que = query(0, 1, 1, Base, 1, x);

            if (que.id > 0 and que.end >= y) {
                res = que.id;
            }

        } else if (t == "none") {

            que = query(1, 1, 1, Base, 1, x - 1);

            if (que.id > 0 and que.end <= x - 1) {
                res = que.id;

            } else {

                que = query(1, 1, 1, Base, y + 1, Base);

                if (que.id > 0) {
                    res = que.id;
                }
            }

        } else {

            que = query(0, 1, 1, Base, 1, y);

            if (que.id > 0 and que.end >= x) {
                res = que.id;
            }
        }

        if (res > 0) {

            int beg = Begs[que.id];
            Ends[beg].erase(Ends[beg].find({que.end, que.id}));

            if (!Ends[beg].empty()) {

                update(0, beg, Ends[beg].rbegin()->first, Ends[beg].rbegin()->second);
                update(1, beg, Ends[beg].begin()->first, Ends[beg].begin()->second);

            } else {

                update(0, beg, -INF, -1);
                update(1, beg, INF, -1);

            }
        }
        cout << res << " ";
    }

    cout << "\n";
}
