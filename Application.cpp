#include "Application.h"
#include <climits>
#include <algorithm>
#include <cmath>
#include <map>

Application::Application()
{
    this->graph = new Graph();
    this->reservoirs = csvReservoir(*graph, "../data/Reservoir.csv");
    this->stations = csvStations(*graph, "../data/Stations.csv");
    this->cities = csvCities(*graph, "../data/Cities.csv");
    csvPipes(reservoirs, stations, cities, "../data/Pipes.csv");

    edmondsKarp();
}

void Application::testData() {
    int i = 0;

    for(auto element : graph->getVertexSet())
    {
        if(element->getCity() != "")
        {
            std::cout << "City: " << element->getCity()<<std::endl;
        }
        else
        {
            std::cout << element->getCode() << ":" <<std::endl;
            for(auto path: element->getAdj())
            {
                i++;
                if(path->getDest()->getCity() != "")
                {
                    std::cout << i << " Path goes from " << path->getOrig()->getCode() << " to " << path->getDest()->getCity()
                              << " has a capacity of " << path->getCapacity() << " and a current flow of " << path->getFlow() <<std::endl;

                }
                else
                {
                    std::cout << i << " Path goes from " << path->getOrig()->getCode() << " to " << path->getDest()->getCode()
                              << " has a capacity of " << path->getCapacity() << " and a current flow of " << path->getFlow() <<std::endl;
                }

            }

        }

    }
}

Graph * Application::getGraph() const {
    return this->graph;
}

int Application::findAugmentingPath(Vertex* superSource, Vertex* sink) {
    for (auto vertex : graph->getVertexSet()) {
        vertex->setVisited(false);
    }

    superSource->setVisited(true);
    std::queue<Vertex*> q;
    q.push(superSource);

    while (!q.empty() && !sink->isVisited()) {
        auto current = q.front();
        q.pop();

        for (auto edge : current->getAdj()) {
            if (edge->getCapacity() - edge->getFlow() > 0 && !edge->getDest()->isVisited()) {
                edge->getDest()->setVisited(true);
                edge->getDest()->setPath(edge); // Set the path for backtracking
                q.push(edge->getDest());
            }
        }

        for (auto edge : current->getIncoming()) {
            if (edge->getFlow() > 0 && !edge->getOrig()->isVisited()) {
                edge->getOrig()->setVisited(true);
                edge->getOrig()->setPath(edge); // Set the path for backtracking
                q.push(edge->getOrig());
            }
        }
    }

    return sink->isVisited();
}

double Application::findMinResidualAlongPath(Vertex* superSource, Vertex* sink) {
    double f = INT_MAX;

    for (auto v = sink; v != superSource; ) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, e->getCapacity() - e->getFlow());
            v = e->getOrig();
        } else {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }

    return f;
}

void Application::augmentFlowAlongPath(Vertex* source, Vertex* sink, double f) {
    for (auto current = sink; current != source; ) {
        auto e = current->getPath();
        double flow = e->getFlow(); // current flow

        if (e->getDest() == current) {
            e->setFlow(flow + f);
            current = e->getOrig();
        }
        else {
            e->setFlow(flow - f);
            current = e->getDest();
        }
    }
}


void Application::edmondsKarp() {
    Vertex *superSink = new Vertex("superSink");
    double demand = 0.0;

    for (auto city : cities) {
        city.second->addEdge(superSink, city.second->getDemand());
        demand += city.second->getDemand();
    }
    graph->setTotalDemand(demand);

    superSink->setDemand(demand);
    graph->addVertex(superSink);
    auto sink = superSink; // temporary

    Vertex *superSource = new Vertex("superSource");

    for (auto reservoir : reservoirs) {
        superSource->addEdge(reservoir.second, reservoir.second->getMaxDelivery());
    }

    graph->addVertex(superSource);

    for (auto vertex : graph->getVertexSet()) {
        for (auto edge : vertex->getAdj()) {
            edge->setFlow(0);
        }
    }

    double maxFlow = 0.0;
    while (findAugmentingPath(superSource, sink)) {
        double f = findMinResidualAlongPath(superSource, sink);
        maxFlow += f;
        augmentFlowAlongPath(superSource, sink, f);
    }

    graph->setMaxflow(maxFlow);

     for (auto cityPipe: sink->getIncoming()) {
        double flow = 0.0;
        for (auto e: cityPipe->getOrig()->getIncoming()) {
            flow += e->getFlow();
        }
        cityPipe->getOrig()->setMaxFlow(flow);
        cityPipe->getOrig()->setSupplyDeficit(cityPipe->getOrig()->getDemand() - flow);
    }

    graph->removeVertex("superSource");
    graph->removeVertex("superSink");
}

bool Application::removeReservoir(const std::string& code) {
    Vertex *r = graph->findVertex(code);

    if (r == nullptr) {
        return false;
    }


    for (auto p : cities) {
        Vertex* vertex = p.second;
        vertex->setTempFlow(vertex->getMaxFlow());
    }

    graph->removeVertex(code);
    edmondsKarp();

    std::map<Vertex*, std::vector<int>> affected;

    for (auto p : cities) {
        Vertex* vertex = p.second;
        double oldFlow = vertex->getTempFlow();
        double newFlow = vertex->getMaxFlow();
        if (newFlow < oldFlow) {
            std::vector<int> flows;
            flows.push_back(oldFlow);
            flows.push_back(newFlow);
            affected[vertex] = flows;
        }
    }

    if (affected.empty()) {
        std::cout << "\nNo cities affected by removal of reservoir " << code << std::endl;
        return true;
    }

    std::cout << "\nCities affected by removal of reservoir " << code << "\n\n";
    for (auto it = affected.begin(); it != affected.end(); it++) {
        std::cout << "----- " << it->first->getCode() << " " << it->first->getCity() << std::endl;
        std::cout << "Old Flow: " << std::to_string(it->second[0]) << std::endl;
        std::cout << "New Flow: " << std::to_string(it->second[1]) << std::endl;
    }

    return true;
}

bool Application::removePumpingStation(const std::string& code) {
    Vertex *ps = graph->findVertex(code);
    if (ps == nullptr) {
        return false;
    }

    for (auto p : cities) {
        Vertex* vertex = p.second;
        vertex->setTempFlow(vertex->getMaxFlow());
    }

    graph->removeVertex(code);
    edmondsKarp();

    std::map<Vertex*, std::vector<int>> affected;

    for (auto p : cities) {
        Vertex* vertex = p.second;
        double oldFlow = vertex->getTempFlow();
        double newFlow = vertex->getMaxFlow();
        if (newFlow < oldFlow) {
            std::vector<int> flows;
            flows.push_back(oldFlow);
            flows.push_back(newFlow);
            affected[vertex] = flows;
        }
    }

    if (affected.empty()) {
        std::cout << "\nNo cities affected by removal of pumping station " << code << std::endl;
        return true;
    }

    std::cout << "\nCities affected by removal of pumping station " << code << "\n\n";
    for (auto it = affected.begin(); it != affected.end(); it++) {
        std::cout << "----- " << it->first->getCode() << " " << it->first->getCity() << std::endl;
        std::cout << "Old Flow: " << std::to_string(it->second[0]) << std::endl;
        std::cout << "New Flow: " << std::to_string(it->second[1]) << std::endl;
    }

    return true;
}

bool Application::removePipeline(const std::pair<std::string, std::string>& pipe) {
    Vertex *a = graph->findVertex(pipe.first);
    if (a == nullptr)
        return false;

    Edge *edge = nullptr;
    Edge *reverseEdge = nullptr;

    for (auto e : a->getAdj()) {
        if (e->getDest()->getCode() == pipe.second) {
            edge = e;
            edge->setCapacity(0);
            reverseEdge = edge->getReverse();
            if (reverseEdge != nullptr)
                reverseEdge->setCapacity(0);
            break;
        }
    }

    if (edge == nullptr)
        return false;

    for (auto p : cities) {
        Vertex* vertex = p.second;
        vertex->setTempFlow(vertex->getMaxFlow());
    }


    edmondsKarp();

    std::map<Vertex*, std::vector<int>> affected;

    for (auto p : cities) {
        Vertex* vertex = p.second;
        double oldFlow = vertex->getTempFlow();
        double newFlow = vertex->getMaxFlow();
        if (newFlow < oldFlow) {
            std::vector<int> flows;
            flows.push_back(oldFlow);
            flows.push_back(newFlow);
            affected[vertex] = flows;
        }
    }

    if (affected.empty()) {
        std::cout << "\nNo cities affected by removal of pipe " << pipe.first << '-' << pipe.second << std::endl;
    }

    else {
        std::cout << "\nCities affected by removal of pipe " << pipe.first << '-' << pipe.second << "\n" << std::endl;
        for (auto it = affected.begin(); it != affected.end(); it++) {
            std::cout << "----- " << it->first->getCode() << " " << it->first->getCity() << std::endl;
            std::cout << "Old Flow: " << std::to_string(it->second[0]) << std::endl;
            std::cout << "New Flow: " << std::to_string(it->second[1]) << std::endl;
        }
    }

    return true;
}

void Application::pipesStats() {
    // total flow
    double sumFlow = 0.0;
    for (auto v : graph->getVertexSet()) {
        for (auto e : v->getAdj()) {
            sumFlow += e->getFlow();
        }
    }

    // total flow into cities
    double flowIntoCitiesBefore = 0.0;
    for (auto it = cities.begin(); it != cities.end(); it++) {
        for (auto e : it->second->getIncoming()) {
            flowIntoCitiesBefore += e->getFlow();
        }
    }

    // maximum difference between capacity and flow
    double maxDiff = 0.0;
    for (auto v : graph->getVertexSet()) {
        for (auto e : v->getAdj()) {
            if (e->getCapacity() - e->getFlow() > maxDiff) {
                maxDiff = e->getCapacity() - e->getFlow();
            }
        }
    }


    double numPipes = 0.0;

    // average of differences between capacity and flow
    double avgCapFlowDif = 0;
    for (auto v : graph->getVertexSet()) {
        for (auto e : v->getAdj()) {
            double diff = (double)e->getCapacity() - e->getFlow();
            avgCapFlowDif += diff;
            numPipes++;

        }
    }
    avgCapFlowDif /= numPipes;
    graph->setAvgCapFlowDif(avgCapFlowDif);

    // variance of differences between capacity and flow
    double variance = 0.0;
    for (auto v : graph->getVertexSet()) {
        for (auto e : v->getAdj()) {
            variance += std::pow(((double)e->getCapacity() - e->getFlow()) - avgCapFlowDif,2);
        }
    }
    variance /= (numPipes - 1.0);

    std::cout << "Average Difference between Capacity and Flow: " << avgCapFlowDif << std::endl;
    std::cout << "Variance of Differences between Capacity and Flow: " << variance << std::endl;
    std::cout << "Total Flow: " << sumFlow << std::endl;
    std::cout << "Flow into Cities: " << flowIntoCitiesBefore << std::endl;
    std::cout << "Maximum Difference between Capacity and Flow: " << maxDiff << "\n" << std::endl;


}

void Application::balanceFlow() {
    std::cout << "-----BEFORE BALANCING-----\n" << std::endl;
    pipesStats();

    for (auto v : graph->getVertexSet()) {
        for (auto e : v->getAdj()) {
            balanceFlowAux(e);
        }
    }

    std::cout << "-----AFTER BALANCING-----\n" << std::endl;
    pipesStats();
}

void Application::balanceFlowAux(Edge *e) {
    double occupation = e->getFlow() / e->getCapacity();
    if (occupation > 0.30 || e->getOrig()->getType() == "R"  || e->getDest()->getType() == "C") {
        return;
    }

    double totalOrigFlow = 0.0;
    double totalDestCapacity = 0.0;

    for (auto pipe : e->getOrig()->getIncoming()) {
        if (pipe == e->getReverse())
            continue;
        totalOrigFlow += pipe->getFlow();
    }

    if (totalOrigFlow == 0) {
        return;
    }

    for (auto destPipe : e->getDest()->getAdj()) {
        if (destPipe->getDest()->getType() == "C" && destPipe->getDest()->getDemand() < destPipe->getCapacity()) {
            totalDestCapacity += destPipe->getDest()->getDemand();
            continue;
        }
        totalDestCapacity += destPipe->getCapacity();
    }

    double maxFlowAdjustment = std::min(e->getCapacity() - e->getFlow(), std::min(totalDestCapacity - e->getFlow(), totalOrigFlow - e->getFlow()));

    for (auto edge : e->getOrig()->getAdj()) {
        double tempOccupation = edge->getFlow() / edge->getCapacity();
        if (edge != e && edge->getDest()->getType() != "C" && tempOccupation >= occupation) {
            double proportion = (edge->getFlow() / totalOrigFlow) * maxFlowAdjustment;
            e->setFlow(e->getFlow() + proportion);
            edge->setFlow(edge->getFlow() - proportion);
        }
    }
}

void Application::waterDeficitStats(std::string in) const {
    if (in == "A" || in == "a") {
        for (auto p : cities) {
            Vertex* v = p.second;
            std::cout << v->getCode() << " | " << v->getCity() << " | " << v->getSupplyDeficit() << " m3/sec\n";
        }
        return;
    }

    if (in != "C" && in != "c") {
        std::cout << "Invalid input." << std::endl;
        return;
    }

    std::string city;
    std::cout << "City code" << std::endl;
    std::cin >> city;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    try {
        Vertex* v = cities.at(city);
        std::cout << v->getCode() << " | " << v->getCity() << " | " << v->getSupplyDeficit() << " m3/sec\n";

    }
    catch (const std::out_of_range& e) {
        std::cout << "\nInvalid city code." << std::endl;
    }
}

std::unordered_map<std::string, Vertex *> Application::getCities() const {
    return cities;
}
