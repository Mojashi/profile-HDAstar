#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <utility>
#include <ostream>
#include "hdastar.hpp"
#include "util.hpp"
using namespace std;


int main(int argc, const char** argv) {
    if(argc < 4) {
        cerr << "Usage: " << argv[0] << " [height] [width] [obstacleRatio]" << endl;
        return 1;
    }

    int height = stoi(argv[1]);
    int width = stoi(argv[2]);
    float obstacleRatio = stof(argv[3]);
    int processNum = 5;
    auto modHashFunc = [processNum](int nodeID){return nodeID%processNum;};
    auto superModHashFunc = [processNum](int nodeID){return (nodeID^1000000007)%processNum;};

    HDAstar hdastar(
        processNum, 
        superModHashFunc,
        [processNum,height,width](int nodeID){
            auto tmp = fromID(nodeID, width, height);
            int y = tmp.first, x = tmp.second;
            return abs(y - (height - 1)) + abs(x - (width - 1));
        }
    );
    
    GridGraph grid = generateRandomGridMaze(height, width, obstacleRatio, neighbors8);
    cout << grid << endl;
    const vector<Distance> dist = hdastar.run(
        gridToGraph(grid, neighbors8),
        toID(0, 0, width, height), toID(height - 1, width - 1, width, height)
    );

    for(int i = 0; height > i; i++) {
        for(int j = 0; width > j; j++){
            if(grid[i][j]) 
                printf("MM");
            else if(dist[toID(i, j, width, height)] == numeric_limits<Distance>::max())
                printf("__");
            else printf("%02d", dist[toID(i, j, width, height)]);
        }
        cout << endl;
    }

    cout << dist[toID(height - 1, width-1, width, height)] << endl;

    return 0;
}
