#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <utility>
#include <ostream>
#include <array>
#include <cassert>
#include <algorithm>
#include <fstream>
#include "hdastar.hpp"
#include "util.hpp"
using namespace std;

struct Result {
    int processNum, height, width, obstacleRatio, neighborsNum;
    array<int,100> PQSizePercentile;

    string toJSON() {
        string ret;
        ret = ret + "{"
        + "\"processNum\": " + to_string(processNum) + ","
        + "\"height\": " + to_string(height) + ","
        + "\"width\": " + to_string(width) + ","
        + "\"obstacleRatio\": " + to_string(obstacleRatio) + ","
        + "\"neighborsNum\": " + to_string(neighborsNum) + ",";
        ret += "\"PQSizePercentile\": [";
        for(int i = 0; PQSizePercentile.size() > i; i++) {
            ret += to_string(PQSizePercentile[i]);
            if(i < PQSizePercentile.size() - 1) 
                ret += ",";
        }
        ret += "]}";

        return ret;
    }
};

Result averageResults(const vector<Result>& ress) {
    Result sum;

    for(int i =0; sum.PQSizePercentile.size() > i; i++) {
        for(int j = 0; ress.size() > j; j++) 
            sum.PQSizePercentile[i] += ress[j].PQSizePercentile[i];
        sum.PQSizePercentile[i] /= ress.size();
    }
    
    return sum;
}

array<int,100> summarizePQSize(const HDAstar& hdastar) {
    auto stats = hdastar.getStats();
    sort(stats.pqSizeLog.begin(), stats.pqSizeLog.end());
    array<int,100> ret{0};

    int sz = stats.pqSizeLog.size();
    for(int i = 1; 100 > i; i++) {
        ret[i] = stats.pqSizeLog[(int)min(max(0.0, sz * i * 0.01 - 1), sz*1.0)];
    }
    return ret;
}

Result test(int processNum, int height, int width, int obstacleRatio, int neighborsNum){
    assert(neighborsNum == 4 || neighborsNum == 8);
    const auto neighbors = neighborsNum == 8 ? neighbors8 : neighbors4; 
    GridGraph grid = generateRandomGridMaze(height, width, obstacleRatio / 100.0, neighbors);

    HDAstar hdastar(
        processNum, 
        [processNum](int nodeID){return nodeID%processNum;},
        [processNum,height,width](int nodeID){
            int y = nodeID / width;
            int x = nodeID % width;
            return abs(y - (height - 1)) + abs(x - (width - 1));
        }
    );
    const vector<Distance> dist = hdastar.run(
        gridToGraph(grid, neighbors8),
        0, height * width - 1
    );

    return Result {
        processNum, height, width, obstacleRatio, neighborsNum,
        summarizePQSize(hdastar)
    };
}

vector<string> split(const string &s, string delim) {
    vector<string> elems;
    auto itr = s.begin();
    while(true) {
        auto next = find_first_of(itr, s.end(), delim.begin(), delim.end());
        elems.push_back(string(itr, next));
        if(next == s.end()) break;
        itr = ++next;
    }
    return elems;
}

int main(int argc, const char** argv) {
    if(argc < 8) {
        cerr << "Usage: " << argv[0] << " [output filename] [TryTimes] [neighborsNum(4 or 8)] [MinProcessNum-MaxProcessNum-Step] [MinHeight-MaxHeight-Step] [MinWidth-MaxWidth-Step] [MinObstacle(in Percent)-MaxObstacle(in Percent)-Step]" << endl;
        cerr << "Example: " << argv[0] << " out.log 5 8 3-3-1 1-100-2 1-50-2 10-70-5" << endl;
        return 1;
    }

    string outputFilename = argv[1];
    ofstream ofs(outputFilename);
    if(ofs.fail()) {
        cerr << "failed to open file" << endl;
        return 1;
    }

    int tryTimes = stoi(argv[2]);
    int neighborsNum = stoi(argv[3]);
    if(neighborsNum != 4 && neighborsNum != 8) {
        cerr << "neighborsNum must be 4 or 8" << endl;
        return 1;
    }

    const int dimension = 4;
    array<array<int, 3>, dimension> args;
    for(int i = 0; dimension > i; i++) {
        auto buf = split(argv[i + 4], "-");
        if(buf.size() != args[0].size()) {
            cerr << "invalid input format" << endl;
            return 1;
        }

        for(int j = 0; args[0].size() > j; j++) {
            args[i][j] = stoi(buf[j]);
        }
    }


    int MinProcessNum = args[0][0];
    int MaxProcessNum = args[0][1];
    int ProcessNumStep = args[0][2];

    int MinHeight = args[1][0];
    int MaxHeight = args[1][1];
    int HeightStep = args[1][2];

    int MinWidth = args[2][0];
    int MaxWidth = args[2][1];
    int WidthStep = args[2][2];

    int MinObstacle = args[3][0];
    int MaxObstacle = args[3][1];
    int ObstacleStep = args[3][2];

    ofs << "{\"conditions\": \"";
    for(int i = 1; 8 > i; i++) {
        ofs << argv[i] << " ";
    }
    ofs << "\", \"results\": [\n";
    bool havePrev = false;
    for (int processNum = MinProcessNum; MaxProcessNum >= processNum; processNum += ProcessNumStep)
        for (int height = MinHeight; MaxHeight >= height; height += HeightStep)
            for (int width = MinWidth; MaxWidth >= width; width += WidthStep)
                    for (int obstacle = MinObstacle; MaxObstacle >= obstacle; obstacle += ObstacleStep)
                        for (int i = 0; tryTimes > i; i++) {
                            printf("TEST>> processNum:%d height:%d width:%d obstacle:%d%% neighborsNum:%d iter:%d\n", processNum, height, width, obstacle, neighborsNum, i);
                            Result result = test(processNum, height, width, obstacle, neighborsNum);

                            if (havePrev)
                                ofs << "," << endl;
                            havePrev = true;
                            ofs << result.toJSON();
                            ofs.flush();
                        }

    ofs << "]}";

    ofs.close();
    cout << "test finished" << endl;
    return 0;
}

