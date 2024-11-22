#include <algorithm>
#include "Graph.h"

Vertex::Vertex(std::string c) : code(c) {}

Edge * Vertex::addEdge(Vertex *dest, int capacity) {
    auto newEdge = new Edge(this, dest, capacity);
    adj.push_back(newEdge);
    dest->incoming.push_back(newEdge);
    return newEdge;
}

bool Vertex::removeEdge(std::string code) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        Vertex *dest = edge->getDest();
        if (dest->getCode() == code) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true;
        }
        else {
            it++;
        }
    }
    return removedEdge;
}

void Vertex::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

std::string Vertex::getCode() const {
    return this->code;
}

std::string Vertex::getType() const {
    size_t pos = code.find('_');
    return code.substr(0, pos);
}

int Vertex::getId() const {
    size_t pos = code.find('_');
    std::string id = code.substr(pos + 1);
    return std::stoi(id);
}

std::vector<Edge *> Vertex::getAdj() {
    return this->adj;
}

bool Vertex::isVisited() const {
    return this->visited;
}

bool Vertex::isProcessing() const {
    return this->processing;
}

unsigned int Vertex::getIndegree() const {
    return this->indegree;
}

double Vertex::getDist() const {
    return this->dist;
}

Edge *Vertex::getPath() const {
    return this->path;
}

std::vector<Edge *> Vertex::getIncoming() const {
    return this->incoming;
}

void Vertex::setCode(std::string code) {
    this->code = code;
}

void Vertex::setId(int id) {
    this->id = id;
}

void Vertex::setVisited(bool visited) {
    this->visited = visited;
}

void Vertex::setProcesssing(bool processing) {
    this->processing = processing;
}

void Vertex::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

void Vertex::setDist(double dist) {
    this->dist = dist;
}

void Vertex::setPath(Edge *path) {
    this->path = path;
}

void Vertex::deleteEdge(Edge *edge) {
    Vertex *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getCode() == code) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

// Reservoirs
void Vertex::setReservoirData(std::string reservoir, std::string municipality, int maxDelivery) {
    this->reservoir = reservoir;
    this->municipality = municipality;
    this->maxDelivery = maxDelivery;
}

std::string Vertex::getReservoir() const {
    return this->reservoir;
}

std::string Vertex::getMunicipality() const {
    return this->municipality;
}

int Vertex::getMaxDelivery() const {
    return this->maxDelivery;
}


// Cities
void Vertex::setCityData(std::string city, double demand, int population) {
    this->city = city;
    this->demand = demand;
    this->population = population;
}

void Vertex::setSupplyDeficit(int supplyDeficit) {
    this->supplyDeficit = supplyDeficit;
}

void Vertex::setMaxFlow(double maxFlow) {
    this->maxFlow = maxFlow;
}

std::string Vertex::getCity() const {
    return this->city;
}

double Vertex::getDemand() const {
    return this->demand;
}
void Vertex::setDemand(double demand) {
    this->demand = demand;
}

void Vertex::setTempFlow(double tempFlow) {
    this->tempFlow = tempFlow;
}

double Vertex::getTempFlow() const {
    return tempFlow;
}

int Vertex::getPopulation() const {
    return this->population;
}

double Vertex::getCurrFlow() const {
    double res = 0.0;
    for (auto e : incoming) {
        res+=e->getFlow();
    }
    return res;
}

int Vertex::getCurrDelivery() const {
    int res = 0;
    for (auto e: adj) {
        res += e->getFlow();
    }
    return res;
}

int Vertex::getSupplyDeficit() const {
    return this->supplyDeficit;
}

double Vertex::getMaxFlow() const {
    return this->maxFlow;
}


/********************** Edge  ****************************/

Edge::Edge(Vertex *orig, Vertex *dest, int capacity) {
    this->orig = orig;
    this->dest = dest;
    this->capacity = capacity;
}

Vertex *Edge::getDest() const {
    return this->dest;
}

int Edge::getCapacity() const {
    return this->capacity;
}

bool Edge::isSelected() const {
    return this->selected;
}

Vertex *Edge::getOrig() const {
    return this->orig;
}

Edge *Edge::getReverse() const {
    return this->reverse;
}

double Edge::getFlow() const {
    return this->flow;
}

void Edge::setSelected(bool selected) {
    this->selected = selected;
}

void Edge::setReverse(Edge *reverse) {
    this->reverse = reverse;
}

void Edge::setFlow(double flow) {
    this->flow = flow;
}

void Edge::setCapacity(int capacity) {
    this->capacity = capacity;
}

bool Edge::isAnalyzed() const {
    return analyzed;
}

void Edge::setAnalyzed(bool analyzed) {
    Edge::analyzed = analyzed;
}

/********************** Graph  ****************************/

Vertex * Graph::findVertex(const std::string &code) const {
    for (auto v : vertexSet)
        if (v->getCode() == code)
            return v;
    return nullptr;
}

bool Graph::addVertex(Vertex *vertex) {
    if (findVertex(vertex->getCode()) != nullptr)
        return false;
    vertexSet.push_back(vertex);
    return true;
}


bool Graph::removeVertex(const std::string code) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getCode() == code) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getCode());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

bool Graph::addEdge(const std::string &source, const std::string &dest, int capacity) {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, capacity);
    return true;
}

bool Graph::removeEdge(const std::string &source, const std::string &dest) {
    Vertex * srcVertex = findVertex(source);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

bool Graph::addBidirectionalEdge(const std::string &source, const std::string &dest, int capacity) {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, capacity);
    auto e2 = v2->addEdge(v1, capacity);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

int Graph::getNumVertex() const {
    return vertexSet.size();
}

std::vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}

double Graph::getMaxFlow() const {
    return this->maxFlow;
}

void Graph::setMaxflow(double maxFlow) {
    this->maxFlow = maxFlow;
}

double Graph::getTotalDemand() const {
    return this->totalDemand;
}

void Graph::setTotalDemand(double totalDemand) {
    this->totalDemand = totalDemand;
}

/****************** DFS ********************/

std::vector<std::string> Graph::dfs() const {
    std::vector<std::string> res;
    for (auto v : vertexSet)
        v->setVisited(false);
    for (auto v : vertexSet)
        if (!v->isVisited())
            dfsVisit(v, res);
    return res;
}

std::vector<std::string> Graph::dfs(const std::string &source) const {
    std::vector<std::string> res;
    // Get the source vertex
    auto s = findVertex(source);
    if (s == nullptr) {
        return res;
    }
    // Set that no vertex has been visited yet
    for (auto v : vertexSet) {
        v->setVisited(false);
    }
    // Perform the actual DFS using recursion
    dfsVisit(s, res);

    return res;
}

void Graph::dfsVisit(Vertex *v, std::vector<std::string> &res) const {
    v->setVisited(true);
    res.push_back(v->getCode());
    for (auto & e : v->getAdj()) {
        auto w = e->getDest();
        if (!w->isVisited()) {
            dfsVisit(w, res);
        }
    }
}

/****************** BFS ********************/
std::vector<std::string> Graph::bfs(const std::string &source) const {
    std::vector<std::string> res;
    // Get the source vertex
    auto s = findVertex(source);
    if (s == nullptr) {
        return res;
    }

    // Set that no vertex has been visited yet
    for (auto v : vertexSet) {
        v->setVisited(false);
    }

    // Perform the actual BFS using a queue
    std::queue<Vertex *> q;
    q.push(s);
    s->setVisited(true);
    while (!q.empty()) {
        auto v = q.front();
        q.pop();
        res.push_back(v->getCode());
        for (auto & e : v->getAdj()) {
            auto w = e->getDest();
            if ( ! w->isVisited()) {
                q.push(w);
                w->setVisited(true);
            }
        }
    }
    return res;
}

/****************** isDAG  ********************/
bool Graph::isDAG() const {
    for (auto v : vertexSet) {
        v->setVisited(false);
        v->setProcesssing(false);
    }
    for (auto v : vertexSet) {
        if (! v->isVisited()) {
            if ( ! dfsIsDAG(v) ) return false;
        }
    }
    return true;
}

bool Graph::dfsIsDAG(Vertex *v) const {
    v->setVisited(true);
    v->setProcesssing(true);
    for (auto e : v->getAdj()) {
        auto w = e->getDest();
        if (w->isProcessing()) return false;
        if (! w->isVisited()) {
            if (! dfsIsDAG(w)) return false;
        }
    }
    v->setProcesssing(false);
    return true;
}

/****************** toposort ********************/
std::vector<std::string> Graph::topsort() const {
    std::vector<std::string> res;

    for (auto v : vertexSet) {
        v->setIndegree(0);
    }
    for (auto v : vertexSet) {
        for (auto e : v->getAdj()) {
            unsigned int indegree = e->getDest()->getIndegree();
            e->getDest()->setIndegree(indegree + 1);
        }
    }

    std::queue<Vertex *> q;
    for (auto v : vertexSet) {
        if (v->getIndegree() == 0) {
            q.push(v);
        }
    }

    while( !q.empty() ) {
        Vertex * v = q.front();
        q.pop();
        res.push_back(v->getCode());
        for(auto e : v->getAdj()) {
            auto w = e->getDest();
            w->setIndegree(w->getIndegree() - 1);
            if(w->getIndegree() == 0) {
                q.push(w);
            }
        }
    }

    if ( res.size() != vertexSet.size() ) {
        std::cout << "Impossible topological ordering!" << std::endl;
        res.clear();
        return res;
    }

    return res;
}

double Graph::getAvgCapFlowDif() const {
    return avgCapFlowDif;
}

void Graph::setAvgCapFlowDif(double avgCapFlowDif) {
    Graph::avgCapFlowDif = avgCapFlowDif;
}
