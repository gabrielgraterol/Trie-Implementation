#include <iostream>
#include <fstream>
#include "trie.hpp"
using namespace std;

int main() {
    string cmd;
    trie trieOne;
    string file;
    string classification;
    string input;
    while (cin >> cmd) {
        if (cmd == "LOAD") {
            cin >> file;
            trieOne.initTrie(file);
           
        }
       else if (cmd == "INSERT") {
            //cin >> classification;
            //trieOne.insert(classification);
            getline(cin >> ws, classification);
           try {
                trieOne.insert(classification);
            }
            catch (const illegal_exception& e) {
                std::cerr << e.what() << std::endl;
            } 
            
        }
        else if (cmd == "CLASSIFY") {
            cin >> input;            
            try {
                trieOne.classify(input);
            }
            catch (const illegal_exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        else if (cmd == "ERASE") {
            //cin >> classification;
            getline(cin >> ws, classification);
            try {
                trieOne.erase(classification);
            }
            catch (const illegal_exception& e) {
                std::cerr << e.what() << std::endl;
            }
            
        } 
        else if (cmd == "PRINT") {
            trieOne.print();
        }
        else if (cmd == "EMPTY") {
            trieOne.empty();
           
        }

        else if (cmd == "CLEAR") {
            trieOne.clear();
           
        }

        else if (cmd == "SIZE") {
            trieOne.printsize();

        }
           
        else if (cmd == "EXIT") {
            break;
        }

    }
}