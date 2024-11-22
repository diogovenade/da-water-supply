#include "Csv.h"

std::unordered_map<std::string, Vertex*> csvReservoir(Graph& graph, const std::string& filename) {
    std::unordered_map<std::string, Vertex*> reservoirs;
    std::ifstream in(filename);
    std::string res, municipality, id, code, maxDelivery;

    std::getline(in, res); // ignore the first line

    while (std::getline(in, res, ',')) {
        std::getline(in, municipality, ',');
        std::getline(in, id, ',');
        std::getline(in, code, ',');
        std::getline(in, maxDelivery);

        Vertex *reservoir = new Vertex(code);
        reservoir->setId(std::stoi(id));
        reservoir->setReservoirData(res, municipality, std::stoi(maxDelivery));

        reservoirs.insert(std::make_pair(code, reservoir));
        graph.addVertex(reservoir);
    }

    return reservoirs;
}

std::unordered_map<std::string, Vertex*> csvStations(Graph& graph, const std::string& filename) {
    std::unordered_map<std::string, Vertex*> stations;
    std::ifstream in(filename);
    std::string id, code;

    std::getline(in, id); // ignore the first line

    while (std::getline(in, id, ',')) {
        std::getline(in, code);

        Vertex *station = new Vertex(code);
        station->setId(std::stoi(id));

        stations.insert(std::make_pair(code, station));
        graph.addVertex(station);
    }

    return stations;
}

std::unordered_map<std::string, Vertex*> csvCities(Graph& graph, const std::string& filename) {
    std::unordered_map<std::string, Vertex*> cities;
    std::ifstream in(filename);
    std::string c, id, code, demand, population;

    std::getline(in, c); // ignore the first line

    while (std::getline(in, c, ',')) {
        std::getline(in, id, ',');
        std::getline(in, code, ',');
        std::getline(in, demand, ',');
        std::getline(in, population);

        Vertex *city = new Vertex(code);
        city->setId(std::stoi(id));
        city->setCityData(c, std::stod(demand), std::stoi(population));

        cities.insert(std::make_pair(code, city));
        graph.addVertex(city);
    }

    return cities;
}

void csvPipes(std::unordered_map<std::string, Vertex*> reservoirs,
                                         std::unordered_map<std::string, Vertex*> stations,
                                         std::unordered_map<std::string, Vertex*> cities,
                                         const std::string& filename) {
    //std::unordered_set<std::string> pipes;
    std::ifstream in(filename);
    std::string a, b, capacity, direction;

    std::getline(in, a);

    while (std::getline(in, a, ',')) {
        std::getline(in, b, ',');
        std::getline(in, capacity, ',');
        std::getline(in, direction);

        std::string typeA = a.substr(0, a.find('_'));
        std::string typeB = b.substr(0, b.find('_'));

        std::unordered_map<std::string, Vertex*> nodes; // includes all nodes of the graph
        nodes.insert(reservoirs.begin(), reservoirs.end());
        nodes.insert(stations.begin(), stations.end());
        nodes.insert(cities.begin(), cities.end());


        if (std::stoi(direction) == 1) { // pipe is unidirectional
            Edge *pipe = nodes.at(a)->addEdge(nodes.at(b), std::stoi(capacity));
        }

        else {
            Edge *pipeAB = nodes.at(a)->addEdge(nodes.at(b), std::stoi(capacity)); // Edge from A to B
            Edge *pipeBA = nodes.at(b)->addEdge(nodes.at(a), std::stoi(capacity)); // Edge from B to A
            pipeAB->setReverse(pipeBA); // Set the reverse edge
            pipeBA->setReverse(pipeAB); // Set the reverse edge
        }
    }

}