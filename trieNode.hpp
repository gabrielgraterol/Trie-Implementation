#include <iostream>

class trieNode;

class trieNode {

    public:
        std::string class_type{};
        trieNode* subclasses[15]{}; 
        bool isterminal{};
        trieNode* parent;
    private:
};