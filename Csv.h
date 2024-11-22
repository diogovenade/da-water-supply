#ifndef DA2024_PRJ1_G85_CSV_H
#define DA2024_PRJ1_G85_CSV_H

#include "Graph.h"
#include <unordered_map>
#include <unordered_set>
#include <fstream>

std::unordered_map<std::string, Vertex*> csvReservoir(Graph& graph, const std::string& filename);
std::unordered_map<std::string, Vertex*> csvStations(Graph& graph, const std::string& filename);
std::unordered_map<std::string, Vertex*> csvCities(Graph& graph, const std::string& filename);
void csvPipes(std::unordered_map<std::string, Vertex*> reservoirs,
              std::unordered_map<std::string, Vertex*> stations,
              std::unordered_map<std::string, Vertex*> cities,
              const std::string& filename);

#endif //DA2024_PRJ1_G85_CSV_H
