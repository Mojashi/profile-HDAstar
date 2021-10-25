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

    HDAstar hdastar(
        processNum, 
        [processNum](int nodeID){return nodeID%processNum;},
        [processNum,height,width](int nodeID){
            int y = nodeID / width;
            int x = nodeID % width;
            return abs(y - (height - 1)) + abs(x - (width - 1));
        }
    );
    
    GridGraph grid = generateRandomGridMaze(height, width, obstacleRatio, neighbors8);
    cout << grid << endl;
    const vector<Distance> dist = hdastar.run(
        gridToGraph(grid, neighbors8),
        0, height * width - 1
    );

    for(int i = 0; height > i; i++) {
        for(int j = 0; width > j; j++){
            if(grid[i][j]) 
                printf("MM");
            else if(dist[i * width + j] == numeric_limits<Distance>::max())
                printf("__");
            else printf("%02d", dist[i * width + j]);
        }
        cout << endl;
    }

    cout << dist[height * width - 1] << endl;

    return 0;
}

