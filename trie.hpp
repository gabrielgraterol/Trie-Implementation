#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "ece250_socket.h"
//#include "ece250_socket.cpp"
#include "trieNode.hpp"
#include "illegal_exception.hpp"


class trie {
    public:
        trie();
        ~trie();
        void initTrie(std::string filename);
        void insert(std::string classification);
        void classify(std::string input);
        void erase(std::string classification);
        void print();
        void empty();
        void printsize();
        void clear();

    private:
        std::ifstream class_id;
        trieNode* root = new trieNode();
        int size{};
        bool is_upper(std::string line);
        std::string candidate_labels{};
        bool no_subclasses(trieNode* class_type);
        bool is_empty();
        void display_path(trieNode* root, trieNode* current, std::string currpath, int level_trie);
        void clear_trie(trieNode* current);

};

trie::trie() {
    root -> isterminal = true;
}

void trie::initTrie(std::string filename) {
    class_id.open(filename);
    std::string line;
    /*
        CITATION:
        
        I referenced a website (https://www.geeksforgeeks.org/cpp/read-a-file-line-by-line-in-cpp/) to understand how to read a file line by line, in order to 
        properly read the txt files.
    */
    while (std::getline(class_id, line)) {
        /*
            CITATION:

            I referenced a website (https://stackoverflow.com/questions/1474790/how-to-read-write-into-from-text-file-with-comma-separated-values) to understand
            how to retrieve strings within a line of text, seperated by a certain character.
        */
        trieNode* temp = root;
        //temp = root;
        std::istringstream singleline(line);
        std::string word{};
        while(std::getline(singleline, word, ',')) {
            bool exists = false;
            int pos{};
            for (int k{}; k < 15; ++k) {
                if (temp -> subclasses[k] != nullptr) {
                    if (((temp -> subclasses[k]) -> class_type) == word) {
                    exists = true;
                    pos = k;
                    if ((temp -> subclasses[k]) -> isterminal == true) {
                        --size;
                    }
                    break;
                    }
                }
                else {
                    break;
                }
                /*if (((temp -> subclasses[k]) -> class_type) == word) {
                    exists = true;
                    pos = k;
                    break;
                }*/
            }
            if (exists == true) {
                temp = temp -> subclasses[pos];
            }
            else {
                trieNode* node = new trieNode();
                node -> class_type = word;
                node -> isterminal = true;
                int i{};
                while (temp -> subclasses[i] != nullptr) {
                    ++i;
                }
                //trieNode* temp2 = new trieNode();
                //temp2 = temp -> subclasses[i];
                temp -> subclasses[i] = node;
                temp -> isterminal = false;
                node -> parent = temp;
                temp = node;
                ++size; //Using this to add everytime because every node added will be terminal when added.
                //delete temp2;
                //temp2 = nullptr;
                node = nullptr;
                delete node;
            }
        }
        //++size; //it only account for terminal nodes being added (always one terminal node per line)
        temp = nullptr;
        delete temp;
    }
    std::cout << "success" << std::endl;
}

bool trie::is_upper(std::string line) {
    /*
        CITATION:
        
        I reference a website (https://www.geeksforgeeks.org/dsa/check-whether-the-given-character-is-in-upper-case-lower-case-or-non-alphabetic-character/), to 
        understand how to properly identify whether a character is an uppercase letter or not.
    */
    for (std::size_t k{}; line[k] != '\0'; ++k) {
        if (line[k] >= 'A' && line[k] <= 'Z') {
            return true;
        }
    }
    return false;
}

void trie::insert(std::string classification) {
    bool inc_upper = is_upper(classification);
    if (inc_upper == true) {
        //in case: std::any_of(classification[0], classification[classification.length() - 1], is_upper(classification))
        throw illegal_exception(classification);
    }
    
    else {
        //trieNode* temp = new trieNode();
        trieNode* temp = root;
        //temp = root;
        bool inserted = false;
        std::stringstream line(classification);
        std::string word;
        while(std::getline(line, word, ',')) {
            bool exists = false;
            int pos{};
            for (int k{}; k < 15; ++k) {
                if (temp -> subclasses[k] != nullptr) {
                    if (((temp -> subclasses[k]) -> class_type) == word) {
                    exists = true;
                    pos = k;
                    //Adding his to accurately calculate size
                    if ((temp -> subclasses[k]) -> isterminal == true) { //Because if along path and terminal, will no longer be terminal
                        --size;
                    }
                    break;
                    }
                }
                else {
                    break;
                } 
                //DONT UN COMMENT THIS IF
                /*if ((temp -> subclasses[k]) -> class_type == word) {
                    exists = true;
                    pos = k;
                    break;
                }*/
           } 
            if (exists == true) {
                temp = temp -> subclasses[pos];
            }
           else {
                trieNode* node = new trieNode();
                node -> class_type = word;
                node -> isterminal = true;
                int i{};
                while (temp -> subclasses[i] != nullptr) {
                    ++i;
                }
                temp -> subclasses[i] = node;
                temp -> isterminal = false;
                node -> parent = temp;
                temp = node;
                inserted = true;
                //++size;
                node = nullptr;
                delete node;
            }
        }
        if (inserted == false) {
            std::cout << "failure" << std::endl;
        }
        else {
            ++size; //If succesfully inserted, then would add one to size(one new terminal node)
            std::cout << "success" << std::endl;
        } 
        //temp -> parent = nullptr;
        //delete temp -> parent;
        temp = nullptr;
        delete temp;
    }
}

void trie::classify(std::string word_class) {
    if (is_upper(word_class) == true) {
        //In case cant use.begin() and .end(), std::any_of(input[0], input[input.length() - 1], is_upper(input))
        throw illegal_exception(word_class);
    }
    else{
    trieNode* temp = root;
        //temp = root;
        std::string curr_label{};
        while (temp -> isterminal != true) {
            for (std::size_t i{}; (temp -> subclasses[i]) != nullptr; ++i) {
                if (temp -> subclasses[i+1] == nullptr) {
                    if (i == 0) {
                         candidate_labels = ((temp -> subclasses[i]) -> class_type);
                    }
                    else {
                        candidate_labels += ((temp -> subclasses[i]) -> class_type);
                    }
                }
                else if (i == 0) {
                    candidate_labels = ((temp -> subclasses[i]) -> class_type) + ',';
                }
                else {
                    candidate_labels += (temp -> subclasses[i] -> class_type) + ',';
                }

            }
            std::string label = labelText(word_class, candidate_labels); //How to update this every time? - figured out
            //std::cout << curr_label << ",";
            if (curr_label == "") {
                curr_label = label + ',';
            }
            else {
                curr_label += label + ',';
            }
            for (std::size_t k{}; (temp -> subclasses[k]) != nullptr; ++k) {
                if ((temp -> subclasses[k]) -> class_type == label) {
                    temp = temp -> subclasses[k];
                    break;
                }
            }
        }
        curr_label.back() = '\0';
        std::cout << curr_label << std::endl;
        //std::cout << std::endl;
        temp = nullptr;
        delete temp;
    }
}

bool trie::no_subclasses(trieNode* class_type) {
    for (std::size_t k{}; k < 15; ++k) {
        if (class_type -> subclasses[k] != nullptr) {
            return false;
        }
    }
    return true;
}

void trie::erase(std::string classification) {
    if (is_upper(classification) == true) {
        //In case cant use .begin, and .end: : std::any_of(classification[0], classification[classification.length() - 1], is_upper(classification))
        throw illegal_exception(classification);
    }
    else {
        std::istringstream line(classification);
        std::string word;
        trieNode* temp = root;
        //temp = root;
        bool is_empty = true;
        bool success_erase = false;
        while(std::getline(line, word, ',')) {
            //while (temp -> isterminal != true) {
                for (std::size_t k{}; k < 15; ++k) {
                    if (temp -> subclasses[k] != nullptr) {
                        if ((temp -> subclasses[k]) -> class_type == word) {
                            temp = (temp -> subclasses[k]);
                            is_empty = false;
                            break;
                        }
                    }
                }
                if (is_empty == true) {
                    break;
                }
                else {
                    if (no_subclasses(temp) == true) {
                        for (std::size_t k{}; k < 15; ++k) { 
                            if ((temp -> parent) -> subclasses[k] == temp) {
                                (temp -> parent) -> subclasses[k] = nullptr;
                                if (no_subclasses(temp -> parent) == true && (temp-> parent != root)) {
                                    ++size;
                                }
                            }
                        }
                        trieNode* temp_delete = temp;
                        //temp_delete = temp;
                        temp = temp -> parent;
                        delete temp_delete;
                        temp_delete = nullptr;
                        //--size;
                        bool nowempty = no_subclasses(temp);
                        if (nowempty == true) {
                            temp -> isterminal = true;
                        }
                            
                    }
                }
            //}
            /*if (is_empty == true) {
                std::cout << "failure" << std::endl;
                break;
            }*/
        }
        if (is_empty == true) {
            std::cout << "failure" << std::endl;
            return;
        }
        temp = nullptr;
        delete temp;
        --size; //Since all erase going from root to terminal node, a terminal node is always removed, so decrement size by 1.
    }
    std::cout << "success" << std::endl;
}

bool trie::is_empty() {
    for (std::size_t k{}; k < 15; ++k) {
        if (root -> subclasses[k] != nullptr) {
            return false;
        }
    }
    return true;
}

void trie::display_path(trieNode* root, trieNode* current, std::string currpath, int level_trie) {
    /*
        CITATION:

        I reference a webiste (https://www.tutorialspoint.com/print-all-possible-joints-of-a-trie-constructed-from-a-given-list-of-string), to understand how to
        properly print all the possible paths for a trie data structure.
    */
    int stringlength = currpath.size();
    for (std::size_t k{}; k < 15; ++k) {
        if ((current -> subclasses[k]) != nullptr) {
            if (level_trie == 0) {
                currpath = (((current -> subclasses[k]) -> class_type) + ',');
            }
            else {
                currpath += ((current -> subclasses[k]) -> class_type) + ',';
            }
            display_path(root, current -> subclasses[k], currpath, ++level_trie);
        }
        else if (current -> isterminal == true) {
            currpath.back() = '\0';
            currpath.back() = '_';
            //std::cout << currpath << std::endl;
            std::cout << currpath;
            /*
                CITATION:

                I referenced a website (https://www.techiedelight.com/print-all-paths-from-root-to-leaf-nodes-binary-tree/) to understand how to 
                properly print all possible paths, as I encountred an error in my printing where it would include a previous classifcation due to 
                recursion, and learnt how to remove a previous classifcation to properly print the other paths when using recursion.
            */
            /*for (int k{stringlength}; currpath[k] != '\0'; ++k) {
                currpath[k] = '\0';
            }*/
            break; 
        }
        /*
            CITATION:

            I referenced a website (https://www.techiedelight.com/print-all-paths-from-root-to-leaf-nodes-binary-tree/) to understand how to 
            properly print all possible paths, as I encountred an error in my printing where it would include a previous classifcation due to 
            recursion, and learnt how to remove a previous classifcation to properly print the other paths when using recursion.
        */
        /*for (int k{stringlength}; currpath[k] != '\0'; ++k) {
                currpath[k] = '\0';
        }*/
       /*
            CITATION:

            I referenced a website (https://www.geeksforgeeks.org/cpp/stdstringerase-in-cpp/) to understand how to 
            properly remove the end of a string, without simply insertin '\0', as this would cause an error when the paths
            get large.
        */
       //for (int k{stringlength}; k < (currpath.length()); ++k) {
            currpath.erase(stringlength, (currpath.length()) - 1);
            if (currpath.length() == 1) {
                currpath.erase(0,1);
            }
       //}
    }
}

void trie::print() {
    if (is_empty() == true) {
        std::cout << "trie is empty" << std::endl;
    }
    else {
       std::string currpath{};
       display_path(root, root, currpath, 0);
       std::cout << std::endl;
    }
}

void trie::empty() {
    if (is_empty() == true) {
        std::cout << "empty 1" << std::endl;
    }
    else {
        std::cout << "empty 0" << std::endl;
    }
}

void trie::printsize() {
    std::cout << "number of classifications is " << size << std::endl;
}

void trie::clear_trie(trieNode* current) {
    for (std::size_t k{}; k < 15; ++k) {
        if (current -> subclasses[k] != nullptr) {
            clear_trie(current -> subclasses[k]);
        }
    }
    if (current != root) { //Because want to keep root if after clear begin building tree again
        for (std::size_t k{}; k < 15; ++k) {
            if ((current -> parent) -> subclasses[k] == current) {
                //delete ((current -> parent) -> subclasses[k]);
                ((current -> parent) -> subclasses[k]) = nullptr;
                delete ((current -> parent) -> subclasses[k]);
                break;
            }
        }
        current -> parent = nullptr;
        delete current -> parent;
        for (std::size_t k{}; k < 15; ++k) {
            delete current -> subclasses[k];
            current -> subclasses[k] = nullptr;
        }
        delete current;
        current = nullptr;
    }
}

void trie::clear() {
    if (is_empty() == false) {
        clear_trie(root);
    }
    root -> isterminal = true;
    for (std::size_t k{}; k < 15; ++k) {
        root -> subclasses[k] = nullptr;
    }
    size = 0; //Since after clear, there is no classifications in the trie
    std::cout << "success" << std::endl;

}

trie::~trie() {
    if (is_empty() == false) {
        clear_trie(root);
    }
    root -> isterminal = true;
    for (std::size_t k{}; k < 15; ++k) {
        root -> subclasses[k] = nullptr;
    }
    //delete (root -> parent);
    //(root -> parent) = nullptr;
    delete root;
    root = nullptr;
}