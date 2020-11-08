/******************************************************************************
*
*  Emmanuel Ndubuisi
*  CSCI 337 - Foundations of Computing
*  November 01, 2020
*
*  Compilation: clang++ -pthread -std=c++17 -o DFASimulator DFASimulator.cpp
*  Execution: ./DFASimulator
*
******************************************************************************/

#include <iostream>
#include <sstream>
#include <cstring>   // std::strcat()
#include <algorithm> // std::all_of
#include <map>
#include <cassert>
#include <vector>
#include <fstream>
using namespace std;

/******************************************************************************
* Implementation of a DFA simulator using the C++ programming language.
*
* Reads DFA from a textfile. 
* First line contains a list of the final states (as integers), separated by a space. 
* Rest of file contains the transitions in form: startstate, blank, symbol read, blank, tostate
* The program prompts user for the name of text file, then prompts the user for strings to test against the DFA.
* The program then prints the transitions through the machine along with whether the string is accepted or rejected.
* The DFA simulator would continue reading strings until user enters "quit."
********************************************************************************/
class DFASimulator
{
    string filename;
    int startState;
    map<string, int> Transitions;
    vector<int> FinalStates;

public:
    DFASimulator(string);
    void getFinalStates(string firstLine);
    void getTransitions(string fileContent);
    bool readFile();
    void checkDFAInput(string str);
};

// constructor - creates an instance of DFASimulator with file input
DFASimulator::DFASimulator(string filename)
{
    this->filename = filename;
}

/**
 * Get the final states from the text file.
 * This scans the first line of the DFA text file and stores them in a list as integers.
 *
 * @param firstLine String that contains a list of final states separated by a whitespace.
 */
void DFASimulator::getFinalStates(string firstLine)
{
    std::istringstream iss(firstLine);
    for (std::string s; iss >> s;)
        FinalStates.push_back(stoi(s));
}

/**
 * Get transition from the DFA text file.
 * This reads a line from the text file and stores transition.
 *
 * @param fileContent String that contains DFA transition.
 */
void DFASimulator::getTransitions(string filecontent)
{
    // ignore empty lines in text file
    if (filecontent.find_first_not_of(" \t\f\v\n\r") == std::string::npos)
        return;

    int index = 0;
    char key[2];

    // scan each file and store transition where "prev_state + input" maps to the next_state.
    for (char s : filecontent)
    {
        switch (index)
        {
        case 0:
            key[index++] = s;
            break;
        case 1:
            key[index++] = s;
            break;
        case 2:
            Transitions[string(1, key[0]) + key[1]] = s - '0';
            break;
        default:
            cout << "There is an error in the DFA file.\n";
        }
    }
}

/**
 * Read DFA text file.
 * This reads DFA text file and calls helper method to get final states and transitions.
 *
 * @return `true` if the file is valid and `false` if the file does not exist.
 */
bool DFASimulator::readFile()
{
    int lineNumber = 1;

    std::ifstream file(filename);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            // read the first line for final states, the second line for the start state, and others for transitions
            if (lineNumber == 1)
            {
                getFinalStates(line);
                lineNumber++;
                continue;
            }
            else if (lineNumber == 2)
            {
                startState = stoi(string(1, line[0]));
                lineNumber++;
            }

            getTransitions(line);
        }
        file.close();
        return true;
    }
    else
    {
        cout << "The file '" << filename << "' does not exist.\n";
        return false;
    }
}

/**
 * Checks input string for validity.
 * This checks if the user input string would be accepted or rejected by the DFASimulator.
 *
 * @param inputString User input that would be checked against the DFA.
 */
void DFASimulator::checkDFAInput(string inputString)
{
    int currentState = startState;
    string transition = "";

    for (int i = 0; i < inputString.length(); i++)
    {
        char alphabet = inputString[i];

        // combine the current state and the input to determine the next state
        string key = std::to_string(currentState) + alphabet;

        // check if the input string contains any invalid/unknown input alphabet.
        if (Transitions.find(key) != Transitions.end())
        {
            // input string is valid
            // replace currentState with the next state.
            currentState = Transitions[key];
            transition += key + std::to_string(currentState) + " ";
        }
        else
        {
            // invalid input string
            // print error message.
            cout << "The input string is invalid (i.e. it contains one or more character(s) that does not exist in the alphabet for the DFA)\n";
            return;
        }
    }

    // print transition trace.
    cout << "Trace of the transitions through the machine: " << transition << "\n";

    // check if the final state for the input string is part of the list of final states for the DFA.
    if (std::binary_search(FinalStates.begin(), FinalStates.end(), currentState))
    {
        cout << "Accepted.\n";
    }
    else
    {
        cout << "Rejected.\n";
    }
}

// main method
int main()
{
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    // create instance of DFASimulator class.
    DFASimulator dfaSimulator = DFASimulator(filename);

    // stop program if file is invalid
    if (!dfaSimulator.readFile())
        return 0;

    string inputString = "";

    // get inputString from user and check if it was accepted/rejected by the DFA.
    while (inputString != "quit")
    {
        cout << "Enter string to test against DFA: ";
        cin >> inputString;
        if (inputString != "quit")
        {
            dfaSimulator.checkDFAInput(inputString);
            cout << "\n";
        }
    }
}
