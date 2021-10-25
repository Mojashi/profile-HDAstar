#include "util.hpp"


class DisjointSet {
    vector<int> parent, sz;
    #define NO_PARENT -1

public:
    DisjointSet(int n) : parent(n, NO_PARENT), sz(n, 1) {}

    //頂点aと頂点bの属する集合を合体
    void merge(int a, int b) {
        a = root(a);
        b = root(b);
        if(a == b) return;
        if(size(a) < size(b)) 
            swap(a,b);
        
        sz[a] += sz[b];
        parent[b] = a;
    }

    //頂点aと頂点bの属する集合が一緒か判定
    bool same(int a, int b) {
        return root(a) == root(b);
    } 

    //頂点aの属する集合を代表する頂点をなにか返す（実際は、内部で集合をTreeとして管理しているのでそのTreeのrootを返す）
    int root(int a) {
        if(parent[a] == NO_PARENT) return a;
        return parent[a] = root(parent[a]);
    }
    //頂点aの属する集合のサイズを返す
    int size(int a) {
        return sz[root(a)];
    }
};

ostream& operator<<(ostream& os, const GridGraph& g) {
    if(g.empty() || g[0].empty()) {
        os << "empty graph";
        return os;
    }
    os << g.size() << " x " << g[0].size() << endl;
    for(int i = 0; g.size() > i; i++) {
        for(int j = 0; g[i].size() > j; j++) {
            os << (g[i][j] ? "MM" : "__");
        }
        os << endl;
    }
    return os;
}

bool connected(const GridGraph& g, int fromY, int fromX, int toY, int toX, const vector<pair<int, int>>& neighbors) {
    if(g.empty() || g[0].empty()) return {};
    int height = g.size();
    int width = g[0].size();
    DisjointSet ds(height * width);

    for (int i = 0; height > i; i++)
        for (int j = 0; width > j; j++)
            if (!g[i][j])
                for (auto v : neighbors)
                {
                    int y = i + v.first;
                    int x = j + v.second;
                    if (y >= 0 && y < height && x >= 0 && x < width && !g[y][x])
                        ds.merge(i * width + j, y * width + x);
                }
    return ds.same(fromY*width + fromX, toY*width + toX);
}

//0がスタート、height*width-1がゴール
GridGraph generateRandomGridMaze(int height, int width, float obstacleRatio, const vector<pair<int, int>>& neighbors) {
    random_device seed_gen;
    mt19937 engine(seed_gen());
    uniform_real_distribution<float> dist;

    while(true) {
        GridGraph obstacle(height, vector<bool>(width, true));

        for (int i = 0; height > i; i++)
            for (int j = 0; width > j; j++)
                obstacle[i][j] = !(dist(engine) > obstacleRatio);

        obstacle[0][0] = false;
        obstacle[height - 1][width - 1] = false;

        if(connected(obstacle, 0, 0, height-1, width-1, neighbors))
            return obstacle;
        // cerr << "failed to generate maze..." << endl;
    }
}

Graph gridToGraph(const GridGraph& g, const vector<pair<int, int>>& neighbors) {
    if(g.empty() || g[0].empty()) return {};

    int height = g.size();
    int width = g[0].size();
    Graph ret(height * width);

    for(int i = 0; height > i; i++) 
        for(int j = 0; width > j; j++) 
            if(!g[i][j])
            for(auto v : neighbors) {
                int y = i + v.first;
                int x = j + v.second;
                if(y >= 0 && y < height && x >= 0 && x < width && !g[y][x]) 
                    ret[i * width + j].push_back({
                        i * width + j, y * width + x, 1
                    });
            }

    return ret;
}
