#include <limits>
#include "Menu.h"

void Menu::mainMenu() {
    std::cout << "================Water Supply Manager================" << std::endl;
    std::cout << "1. Maximum Flow" << std::endl;
    std::cout << "2. Water flow in deficit" << std::endl;
    std::cout << "3. Balance the load across the network" << std::endl;
    std::cout << "4. Remove water reservoirs" << std::endl;
    std::cout << "5. Remove pumping stations" << std::endl;
    std::cout << "6. Remove pipelines" << std::endl;
    std::cout << "0. Exit" << std::endl;

    std::string in;
    std::cout << "> ";
    std::cin >> in;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    key = processInput(in);
}

void Menu::clearTerminal() {
#ifdef _WIN32
    if (system("cls") == -1) std::cout << "\n\n\n\n\n";
#else
    if (system("clear") == -1) std::cout << "\n\n\n\n\n";
#endif
}

void Menu::run() {
    while (key != 0) {
        clearTerminal();
        mainMenu();
        clearTerminal();
        switch (key) {
            case 1:
                maxAmountWater();
                currFunction = &Menu::maxAmountWater;
                break;
            case 2:
                waterDeficit();
                currFunction = &Menu::waterDeficit;
                break;
            case 3:
                balanceNetworkLoad();
                currFunction = &Menu::balanceNetworkLoad;
                break;
            case 4:
                removeReservoirs();
                currFunction = &Menu::removeReservoirs;
                break;
            case 5:
                removePumpingStations();
                currFunction = &Menu::removePumpingStations;
                break;
            case 6:
                removePipelines();
                currFunction = &Menu::removePipelines;
                break;
            case 0:
                clearTerminal();
                return;
            default:
                break;
        }

        if (key != -1) {
            repeatLastFunction();
            key = -1; // Reset key for the next iteration
        }

    }
}


int Menu::processInput(const std::string input) {
    try {
        int option = std::stoi(input);
        if (option < 0 || input.size() > 2) {
            throw std::invalid_argument("");
        }
        return option;
    }
    catch (const std::invalid_argument& ia) {
        key = -1;
        std::cout << "\nError, please input a valid option." << std::endl;
        return -1;
    }
}

// application functions
void Menu::maxAmountWater() {
    waterSupplyManager.edmondsKarp();

    std::string response;
    std::cout << "Total flow or city flow? (T/C)" << std::endl;
    std::cin >> response;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (response == "T" || response == "t") {
        std::cout << "\nTOTAL FLOW = " << waterSupplyManager.getGraph()->getMaxFlow() << " m3/sec"
                  << "\nTOTAL DEMAND = " << waterSupplyManager.getGraph()->getTotalDemand() << " m3/sec"
                  << "\nOVERALL DEFICIT = " << waterSupplyManager.getGraph()->getTotalDemand() -waterSupplyManager.getGraph()->getMaxFlow() << " m3/sec\n" << std::endl;

        for (auto p : waterSupplyManager.getCities()) {
            Vertex* v = p.second;
            std::cout << v->getCode() << " | " << v->getCity() << " | " << v->getMaxFlow()  << " m3/sec\n";
        }
    }
    else if (response == "C" || response == "c") {
        std::string city;
        std::cout << "City code:" << std::endl;
        std::cin >> city;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << '\n';
        try {
            Vertex* v = waterSupplyManager.getCities().at(city);
            std::cout << v->getCode() << " | " << v->getCity() << " | " << v->getMaxFlow()  << " m3/sec\n";

        }
        catch (const std::out_of_range& e) {
            std::cout << "\nInvalid city code." << std::endl;
        }

    }

    else std::cout << "\nInvalid input." << std::endl;
}

void Menu::waterDeficit() {
    std::string response;
    std::cout << "Show all cities or choose city? (A/C)" << std::endl;
    std::cin >> response;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << '\n';
    waterSupplyManager.waterDeficitStats(response);
}

void Menu::balanceNetworkLoad() {
    waterSupplyManager.balanceFlow();
    waterSupplyManager = Application();
}

void Menu::removeReservoirs() {
    std::string code;
    std::cout << "Reservoir code:" << std::endl;
    std::cin >> code;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (!waterSupplyManager.removeReservoir(code)) {
        std::cout << "\nInvalid reservoir code." << std::endl;
        return;
    }

    waterSupplyManager = Application();
}

void Menu::removePumpingStations() {
    std::string code;
    std::cout << "Pumping station code:" << std::endl;
    std::cin >> code;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (!waterSupplyManager.removePumpingStation(code)) {
        std::cout << "\nInvalid pumping station code." << std::endl;
        return;
    }
    waterSupplyManager = Application();
}

void Menu::removePipelines() {
    std::pair<std::string, std::string> pipe;
    std::string servicePointA, servicePointB;

    std::cout << "Service Point A: ";
    std::cin >> servicePointA;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Service Point B: ";
    std::cin >> servicePointB;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    pipe.first = servicePointA;
    pipe.second = servicePointB;

    if (!waterSupplyManager.removePipeline(std::make_pair(servicePointA, servicePointB))) {
        std::cout << "\nInvalid service point codes combination.\n";
        return;
    }

    waterSupplyManager = Application();
}

void Menu::repeatLastFunction() {

    std::cout << "\n==============================\n";
    std::cout << "0. Repeat last function\n1. Main Menu\n\n";
    std::cout << "> ";

    std::string in;
    std::cin >> in;

    int processedIn = processInput(in);

    if (!processedIn) {
        clearTerminal();
        (this->*currFunction)();
        repeatLastFunction();
    }

    else currFunction = nullptr;

}
