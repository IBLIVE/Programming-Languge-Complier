//NAME: ISKANDER KUSHBAY
//CLASS: CSE340
//PROJECT 1
//structs header file
#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <string>
#include <vector>
#include <unordered_set>
#include "lexer.h"

struct REG_node { //class for nodes
    REG_node* first_neighbour = nullptr;
    std::string first_label = "";
    REG_node* second_neighbour = nullptr;
    std::string second_label = "";
    int id = 0;
    REG_node();
    REG_node& operator=(const REG_node& rn); //operator overload
    bool operator==(const REG_node& two);//operator overload
    bool operator<(const REG_node& two);//operator overload
    size_t operator()(const REG_node& pointToHash) const;//operator overload
};

struct REG { //class for regular expression graphs
    REG_node* start = nullptr;
    REG_node* finish = nullptr;
    REG(REG_node* node, REG_node* node2); //constructor
    REG(const REG& other);//constructor
    REG(std::string s);//constructor
    REG();//constructor
};

//operator overload
bool operator==(const REG_node& one, const REG_node& two);

struct TokenStruct { //struct for storing tokens and their REG;
    Token token;
    REG reg;
};

//hashing function
size_t hash_value(const REG_node& node);

//hash struct for REG_node
namespace std {
    template<> struct hash<REG_node>
    {
        std::size_t operator()(const REG_node& p) const noexcept
        {
            return p.id;
        }
    };
}

#endif