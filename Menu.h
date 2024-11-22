#ifndef DA2024_PRJ1_G85_MENU_H
#define DA2024_PRJ1_G85_MENU_H


#include "Application.h"
#include <cstdlib>
#include <functional>

/**
 * @brief The Menu class represents the interface of the application.
 *
 * This class provides functionality to enable interaction between the user and the application.
 */
class Menu {
public:

    /**
     * @brief Clears the terminal.
     *
     * Uses the platform-specific command to clear the terminal.
     *
     * @note If the execution of the commands fails it outputs multiple newline characters.
     *
    */
    void clearTerminal();

    /**
     * @brief Runs the main menu loop.
     *
     * This function continuously displays the main menu, processes user input, and performs corresponding actions
     * until the user chooses to exit by entering 0. The function chosen by the user is stored using a function
     * pointer to enable simpler repetition if desired.
     *
     */
    void run();

    /**
     * @brief Converts user input from string to integer.
     *
     * This function only allows the conversion of numbers from 0 to 99, inclusive.
     *
     * @param input The user input string to process.
     * @return An integer representing the processed input option, or -1 if the input is invalid.
     *
     * @note Relies on std::stoi for string-to-integer conversion.
     */
    int processInput(const std::string input);

    /**
     * @brief Repeats the last executed menu function or returns to the main menu.
     *
     * This function prompts the user to choose weather to repeat the last executed function or to go back to the main
     * menu. Allows for faster repetitions of the same function.
     *
     * @note The last executed function is stored as a function pointer in 'currFunction'.
     */
    void repeatLastFunction();


private:
    using MenuFunction = void (Menu::*)();
    MenuFunction currFunction = nullptr;
    Application waterSupplyManager = Application();
    int key = -1;

    /**
     * @brief Displays the main menu.
     */
    void mainMenu();

    /**
     * @brief Displays the maximum amount of water menu option.
     *
     * Task T2.1
     */
    void maxAmountWater();

    /**
     * @brief Displays the water deficit menu option.
     *
     * Task T2.2
     */
    void waterDeficit();

    /**
     * @brief Displays the balance network load menu option.
     *
     * Task T2.3
     */
    void balanceNetworkLoad();

    /**
     * @brief Displays the remove reservoirs menu option.
     *
     * Task T3.1
     */
    void removeReservoirs();

    /**
     * @brief Displays the remove pumping stations menu option.
     *
     * Task T3.2
     */
    void removePumpingStations();

    /**
     * @brief Displays the remove pipelines menu option.
     *
     * Task T3.3
     */
    void removePipelines();
};


#endif //DA2024_PRJ1_G85_MENU_H
