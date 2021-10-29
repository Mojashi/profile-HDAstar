#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <functional>
using namespace std;

typedef int Distance;

struct Edge {
    int from, to, weight;
};
typedef vector<vector<Edge>> Graph;

struct Node {
    int id;
    Distance gvalue, fvalue;

    bool operator<(const Node& t) const {
        return fvalue < t.fvalue;
    }
    bool operator>(const Node& t) const {
        return fvalue > t.fvalue;
    }
};

class HDAstar {
    function<int(int)> hashFunc;
    function<Distance(int)> heuristicFunc;
    int processNum;

    struct Stats {
        vector<size_t> pqSizeLog;
        int iterNum = 0;
    } stats;

    class Process {
        priority_queue<Node, vector<Node>, greater<Node>> pq;
        function<Distance(int)> heuristicFunc;
        vector<Distance>* gvalues;
        const Graph* g;

    public:
        Process(){}
        Process(vector<Distance>* gvalues,const Graph* g, function<Distance(int)> heuristicFunc);
        int insert(const vector<Node>& insertList); //returns current heap size
        size_t size();
        vector<Node> run();
    };

public:
    HDAstar(int processNum, function<int(int)> hashFunc, function<Distance(int)> heuristicFunc);
    vector<Distance> run(const Graph& g, int startPos, int goalPos);
    Stats getStats() const;
};