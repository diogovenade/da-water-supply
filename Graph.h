#ifndef DA2024_PRJ1_G85_GRAPH_H
#define DA2024_PRJ1_G85_GRAPH_H

#include <iostream>
#include <vector>
#include <queue>

class Edge;

class Vertex {
public:
    Vertex(std::string c);
    std::string getCode() const;
    std::string getType() const;
    int getId() const;
    std::vector<Edge *> getAdj();
    bool isVisited() const;
    bool isProcessing() const;
    unsigned int getIndegree() const;
    double getDist() const;
    Edge *getPath() const;
    std::vector<Edge *> getIncoming() const;

    void setCode(std::string code);
    void setId(int id);
    void setVisited(bool visited);
    void setProcesssing(bool processing);
    void setIndegree(unsigned int indegree);
    void setDist(double dist);
    void setPath(Edge *path);
    Edge * addEdge(Vertex* dest, int capacity);
    bool removeEdge(std::string code);
    void removeOutgoingEdges();

    // Reservoirs
    void setReservoirData(std::string reservoir, std::string municipality, int maxDelivery);
    std::string getReservoir() const;
    std::string getMunicipality() const;
    int getMaxDelivery() const;
    int getCurrDelivery() const;

    // Cities
    void setCityData(std::string city, double demand, int population);
    void setSupplyDeficit(int supplyDeficit);
    void setMaxFlow(double maxFlow);
    void setTempFlow(double tempFlow);
    std::string getCity() const;
    double getDemand() const;
    void setDemand(double demand);
    int getPopulation() const;
    double getCurrFlow() const;
    int getSupplyDeficit() const;
    double getMaxFlow() const;
    double getTempFlow() const;

    bool operator==(const Vertex& other) const {
        return (this->code == other.code);
    }


protected:
    std::string code;
    int id;
    std::vector<Edge*> adj;

    bool visited = false;
    bool processing = false;
    unsigned int indegree;
    double dist = 0;
    Edge *path = nullptr;

    std::vector<Edge *> incoming;

    // Reservoirs
    std::string reservoir;
    std::string municipality;
    int maxDelivery;

    // Cities
    std::string city;
    double demand;
    int population;
    int supplyDeficit;
    double maxFlow;
    double tempFlow;
    double deliveryCapacity;

    void deleteEdge(Edge *edge);
};

class Edge {
public:
    Edge(Vertex *orig, Vertex *dest, int capacity);

    Vertex *getDest() const;
    int getCapacity() const;
    bool isSelected() const;
    Vertex * getOrig() const;
    Edge *getReverse() const;
    double getFlow() const;

    void setSelected(bool selected);
    void setReverse(Edge *reverse);
    void setFlow(double flow);
    void setCapacity(int capacity);
    bool isAnalyzed() const;
    void setAnalyzed(bool analyzed);

protected:
    Vertex * dest;
    int capacity;

    bool selected = false;

    Vertex *orig;
    Edge *reverse = nullptr;

    double flow;
    bool analyzed;
};

class Graph {
public:
    ~Graph();
    Vertex *findVertex(const std::string &code) const;
    bool addVertex(Vertex *vertex);
    bool removeVertex(const std::string code);

    bool addEdge(const std::string &source, const std::string &dest, int capacity);
    bool removeEdge(const std::string &source, const std::string &dest);
    bool addBidirectionalEdge(const std::string &source, const std::string &dest, int capacity);

    int getNumVertex() const;
    std::vector<Vertex *> getVertexSet() const;

    double getMaxFlow() const;
    void setMaxflow(double maxFlow);
    double getTotalDemand() const;
    void setTotalDemand(double totalDemand);
    double getAvgCapFlowDif() const;
    void setAvgCapFlowDif(double avgCapFlowDif);

    std::vector<std::string> dfs() const;
    std::vector<std::string> dfs(const std::string & source) const;
    void dfsVisit(Vertex *v,  std::vector<std::string> & res) const;
    std::vector<std::string> bfs(const std::string & source) const;

    bool isDAG() const;
    bool dfsIsDAG(Vertex *v) const;
    std::vector<std::string> topsort() const;

protected:
    std::vector<Vertex *> vertexSet;
    double maxFlow;
    double totalDemand;
    double avgCapFlowDif;

    int findVertexIndex(const std::string &code) const;
};

#endif DA2024_PRJ1_G85_GRAPH_H