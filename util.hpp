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
Graph gridToGraph(const GridGraph& g);
GridGraph generateRandomGridMaze(int height, int width, float obstacleRatio);
