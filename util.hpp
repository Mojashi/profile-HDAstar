#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <utility>
#include <ostream>
#include "hdastar.hpp"

typedef vector<vector<bool>> GridGraph;
ostream& operator<<(ostream& os, const GridGraph& g);
Graph gridToGraph(const GridGraph& g, const vector<pair<int, int>>& neighbors);
GridGraph generateRandomGridMaze(int height, int width, float obstacleRatio, const vector<pair<int, int>>& neighbors);
float calcObstacleRatio(const GridGraph& g);


const vector<pair<int, int>> neighbors8 = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
const vector<pair<int, int>> neighbors4 = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};
int toID(int y, int x, int width, int height);
pair<int,int> fromID(int id, int width, int height);