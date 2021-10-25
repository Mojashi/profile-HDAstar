#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <utility>
#include <ostream>
#include "hdastar.hpp"
using namespace std;

typedef vector<vector<bool>> GridGraph;

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

Graph gridToGraph(const GridGraph& g);
GridGraph generateRandomGridMaze(int height, int width, float obstacleRatio);

int main(int argc, const char** argv) {
    if(argc < 4) {
        cerr << "Usage: " << argv[0] << " [height] [width] [obstacleRatio]" << endl;
        return 1;
    }

    int height = stoi(argv[1]);
    int width = stoi(argv[2]);
    float obstacleRatio = stof(argv[3]);
    int processNum = 5;

    HDAstar hdastar(
        processNum, 
        [processNum](int nodeID){return nodeID%processNum;},
        [processNum,height,width](int nodeID){
            int y = nodeID / width;
            int x = nodeID % width;
            return abs(y - (height - 1)) + abs(x - (width - 1));
        }
    );
    
    GridGraph grid = generateRandomGridMaze(height, width, obstacleRatio);
    cout << grid << endl;
    const vector<Distance> dist = hdastar.run(
        gridToGraph(grid),
        0, height * width - 1
    );

    for(int i = 0; height > i; i++) {
        for(int j = 0; width > j; j++){
            if(dist[i * width + j] == numeric_limits<Distance>::max())
                printf("MM");
            else printf("%02d", dist[i * width + j]);
        }
        cout << endl;
    }

    cout << dist[height * width - 1] << endl;
}


//0がスタート、height*width-1がゴール
GridGraph generateRandomGridMaze(int height, int width, float obstacleRatio) {
    random_device seed_gen;
    mt19937 engine(seed_gen());
    uniform_real_distribution<float> dist;

    GridGraph obstacle(height, vector<bool>(width, false));
    for(int i = 0; height > i; i++) 
        for(int j = 0; width > j; j++) 
            obstacle[i][j] = dist(engine) < obstacleRatio;

    obstacle[0][0] = false;
    obstacle[height-1][width-1]=false;
    return obstacle;
}

Graph gridToGraph(const GridGraph& g) {
    if(g.empty() || g[0].empty()) return {};

    const pair<int,int> neighbors[] = {{-1,-1}, {-1,0}, {-1, 1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
    int height = g.size();
    int width = g[0].size();
    Graph ret(height * width);

    for(int i = 0; height > i; i++) 
        for(int j = 0; width > j; j++) 
            if(!g[i][j])
            for(auto v : neighbors) {
                int y = i + v.first;
                int x = j + v.second;
                if(y >= 0 && y < height && x >= 0 && x < width && !g[y][x]) {
                    ret[i * width + j].push_back({
                        i * width + j, y * width + x, 1
                    });
                }
            }

    return ret;
}
