#ifndef DA2024_PRJ1_G85_APPLICATION_H
#define DA2024_PRJ1_G85_APPLICATION_H

#include "Csv.h"
class Application {

public:
    Application(); // parse data
    void testData(); // para testar se o parse foi feito corretamente

    /**
     * @brief Calculates the max-Flow of the network.
     *
     * This function finds the maximum flow from a source node to a sink node in a flow network, the algorithm performs a
     * bfs to find augmenting paths, finds the bottleneck of each path and increments the flow while there are still available
     * paths.
     * Max-flow and water supply deficit is stored on each city.
     *
     * **Complexity**: O(V * E^2), where V is the number of vertice and E the number of Edges.
     *
      */
    void edmondsKarp();

    /**
     * @brief Removes a reservoir from the water supply network.
     *
     * This function removes a reservoir from the water supply network based on the provided reservoir code and re-runs the
     * Edmonds-Karp algorithm to see if any city was affected by the removal.
     *
     * **Complexity**: O(V * E^2), which is the complexity of Edmonds-Karp.
     *
     * @param code The code of the reservoir to be removed.
     * @return True if the reservoir is successfully removed, false otherwise.
      */
    bool removeReservoir(const std::string& code);
    /**
     * @brief Removes a pumping station from the water supply network.
     *
     * This function removes a pumping station from the water supply network based on the provided pumping station code and re-runs the
     * Edmonds-Karp algorithm to see if any city was affected by the removal.
     *
     * **Complexity**: O(V * E^2), which is the complexity of Edmonds-Karp.
     *
     * @param code The code of the pumping station to be removed.
     * @return True if the pumping station is successfully removed, false otherwise.
      */
    bool removePumpingStation(const std::string& code);
    /**
     * @brief Removes a pipeline from the water supply network.
     *
     * This function removes a pipeline from the water supply network based on the provided source and destination codes and re-runs the
     * Edmonds-Karp algorithm to see if any city was affected by the removal. The removal is done by setting the capacity of the
     * pipeline to 0. If the pipeline is bidirectional, the same is done in the other direction.
     *
     * **Complexity**: O(V * E^2), which is the complexity of Edmonds-Karp.
     *
     * @param pipe A pair of codes of the source and destination vertices.
     * @return True if the pipeline is successfully removed, false otherwise.
      */
    bool removePipeline(const std::pair<std::string, std::string>& pipe);
    void pipesStats();

    /**
     * @brief Displays the water deficit of each city.
     *
     * Outputs statistics on water deficit for cities.
     * If the user inputs 'A' (case-insensitive), it prints the supply deficit for every city. If the user inputs 'C'
     * (case-insensitive), it prompts the user to input a city code and prints its supply statistics.
     *
     * **Complexity**: O(n) where n is a city in cities map or O(1) if city specific
     *
     * @param in User input to choose all or city specific statistics.
     */
   void waterDeficitStats(std::string in) const;

    /**
     * @brief Calls the auxiliary function for all edges of the graph
     *
     * The function calls the pipesStats function, in order to determine if there was a change in the statistics of the graph.
     *
     * **Complexity**: O(E^2), in which E is the number of edges in the graph.
     *
      */
    void balanceFlow();
    Graph * getGraph() const;
    std::unordered_map<std::string, Vertex*> getCities() const;


private:
    Graph *graph; // mapa
    std::unordered_map<std::string, Vertex*> reservoirs;
    std::unordered_map<std::string, Vertex*> stations;
    std::unordered_map<std::string, Vertex*> cities;

    /**
     * @brief Finds augmenting paths in the network
     *
     * Performs a modified BFS to find out if there exist residual paths.
     *
     * **Complexity**: O(V+E), in which E is the number of edges in the graph and V the number of vertices
     *
     * @param superSource Source vertex, connects to every reservoir.
     * @param sink Destination vertex.
     *
     * @return 1 if the sink is visited or 0 if not
     */
    int findAugmentingPath(Vertex *superSource, Vertex *sink);

    /**
     * @brief Finds the bottleneck value along a residual path
     *
     * It goes along a residual path and returns the bottleneck (minimum) value of the path.
     *
     * **Complexity**: O(V), in which V the number of vertices
     *
     * @param superSource Source vertex, connects to every reservoir.
     * @param sink Destination vertex.
     *
     * @return bottleneck value of the path
     */
    double findMinResidualAlongPath(Vertex *superSource, Vertex *sink);

    /**
     * @brief Augments the flow along a certain path
     *
     * It goes along a residual path and increments/decrements the flow along all edges of the residual path
     *
     * **Complexity**: O(V), in which V the number of vertices
     *
     * @param superSource Source vertex, connects to every reservoir.
     * @param sink Destination vertex.
     *
     */
    void augmentFlowAlongPath(Vertex *source, Vertex *sink, double f);

    /**
     * @brief Adjusts the flow of the received edge
     *
     * Analyzes the intermediate pipes of the graph (not connected to cities or reservoirs) with a flow occupation rate smaller than 30%;
     * Calculates the amount of flow that can be added to the pipe being analyzed. This number is limited by the pipe's capacity,
     * the amount of flow that enters the pipe and the capacity of the pipes that "go out" of the current pipe.
     * If one of the latter pipes directly services a city, and the city's demand is smaller or equal to the capacity of that pipe,
     * then we take the city's demand into consideration;
     * Proportionally removes flow from the pipes with the same origin as the current pipe, unless the pipe services a city
     * (we want the flow that reaches the cities to stay the same) or has an occupation rate inferior to the current pipe's rate.
     *
     * **Complexity**: O(E), in which E is the number of edges in the graph
     *
     * @param e Edge to have its flow adjusted
      */
    void balanceFlowAux(Edge *e);
};


#endif //DA2024_PRJ1_G85_APPLICATION_H
