// classes example
#include <iostream>
#include <sstream>
#include <cstring>  // std::strcat()
#include <algorithm>    // std::all_of
#include <map>
#include <cassert>
#include <vector>
#include <fstream>
using namespace std;

// todo: file validation
// todo: comments
class DFASimulator {
    string filename;
    int startState;
    map<string, int> Transitions;
    vector<int> FinalStates;
  public:
    DFASimulator(string);
    void getFinalStates(string firstLine);
    void getTransitions(string fileContent);
    void readFile();
    void checkDFAInput(string str);
};

DFASimulator::DFASimulator (string filename) {
  this->filename = filename;
}

void DFASimulator::getFinalStates(string firstLine) {
  std::istringstream iss(firstLine); 
  for(std::string s; iss >> s;) 
      FinalStates.push_back(stoi(s));
}

void DFASimulator::getTransitions(string filecontent) {
  // ignore empty lines
  if (filecontent.find_first_not_of(" \t\f\v\n\r") == std::string::npos) return;

  int index = 0;
  char key[2];

  for (char s : filecontent) {
    switch (index) {
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

void DFASimulator::readFile () {
  int lineNumber = 1;

  std::ifstream file(filename);
  if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        if (lineNumber == 1) {
          getFinalStates(line);
          lineNumber++;
          continue;
        } else if (lineNumber == 2) {
          startState = stoi(string(1, line[0]));
          lineNumber++;
        }
        
        getTransitions(line);
      }
      file.close();
  }
}

void DFASimulator::checkDFAInput(string str) {
  int currentState = startState;
  string transition = "";

  for (int i = 0; i < str.length(); i++) {
    char alphabet = str[i];

    string key = std::to_string(currentState) + alphabet;

    if (Transitions.find(key) != Transitions.end()) {
      // input string is valid
      currentState = Transitions[key];
      transition += key + std::to_string(currentState) + " ";
    } else {
      // invalid input string
      cout << "The input string is invalid (i.e. it contains one or more character(s) that does not exist in the alphabet for the DFA\n";
      return;
    }
  }

  cout << "Trace of the transitions through the machine: " << transition << "\n";

  if (std::binary_search(FinalStates.begin(), FinalStates.end(), currentState)) {
      cout << "Accepted.\n";
  } else {
      cout << "Rejected.\n";
  }
}

int main() {
  string filename;
  cout << "Enter filename: ";
  cin >> filename;

  DFASimulator dfaSimulator = DFASimulator(filename);
  dfaSimulator.readFile();
  string str = "";
  while(! (str == "quit")) {
      cout << "Enter string to test against DFA: ";
      cin >> str;
      if (!(str == "quit")) {
        dfaSimulator.checkDFAInput(str);
      }
  }
}
