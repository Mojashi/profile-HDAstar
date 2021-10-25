#include "hdastar.hpp"

HDAstar::Process::Process(
    vector<Distance>* gvalues, 
    const Graph* g, 
    function<Distance(int)> heuristicFunc) :
    gvalues(gvalues), g(g), heuristicFunc(heuristicFunc) {}

int HDAstar::Process::insert(const vector<Node> &insertList) {
    for(const auto& node : insertList)
        pq.push(node);
    return pq.size();
}

vector<Node> HDAstar::Process::run() {
    vector<Node> ret;
    if(pq.empty()) 
        return ret;
    
    Node cur = pq.top();
    pq.pop();

    Distance gvalue = gvalues->at(cur.id);
    for(const Edge& e : g->at(cur.id)) {
        Distance nextg = gvalue + e.weight;
        if(gvalues->at(e.to) > nextg) {
            ret.push_back({e.to, nextg, nextg + heuristicFunc(e.to)});
        }
    }

    for(const Node& node : ret) 
        gvalues->at(node.id) = node.gvalue;

    return ret;
}

HDAstar::HDAstar(
    int processNum,
    function<int(int)> hashFunc,
    function<Distance(int)> heuristicFunc) : 
    processNum(processNum), hashFunc(hashFunc), heuristicFunc(heuristicFunc) {}

#include <limits>
vector<Distance> HDAstar::run(const Graph &g, int startPos, int goalPos) {
    const Distance MAX = numeric_limits<Distance>::max();
    vector<Distance> gvalues(g.size(), MAX);
    vector<Process> procs(processNum);
    for(int i = 0; processNum > i; i++)
        procs[i] = Process(&gvalues, &g, heuristicFunc);
    
    gvalues[0] = 0;
    procs[hashFunc(startPos)].insert({{0, 0}});

    while(gvalues[goalPos] == MAX) {
        vector<vector<Node>> insertLists(processNum);
        for(auto& proc : procs) {
            vector<Node> buf = proc.run();
            for(const Node& node : buf) {
                insertLists[hashFunc(node.id)].push_back(node);
            }
        }

        bool existInsetNode = false;
        for(int i = 0; processNum > i; i++) {
            existInsetNode |= !insertLists.empty();
            procs[i].insert(insertLists[i]);
        }

        if(!existInsetNode) break;
    }

    return gvalues;
}
