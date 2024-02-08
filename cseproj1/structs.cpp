//NAME: ISKANDER KUSHBAY
//CLASS: CSE340
//PROJECT 1
//REG and REG_node implementation
#include "structs.h"
#include <string>
#include <vector>
#include <unordered_set>

//operator overload
REG_node& REG_node::operator=(const REG_node& rn){
        first_neighbour = rn.first_neighbour;
        second_neighbour = rn.second_neighbour;
        first_label = rn.first_label;
        second_label = rn.second_label;
        return *this;
    }  
//operator overload
bool REG_node::operator==(const REG_node& two){
        return this->first_label == two.first_label && this->second_label == two.second_label && this->first_neighbour == two.first_neighbour &&
        this->second_neighbour == two.second_neighbour;
    }
//operator overload
bool REG_node::operator<(const REG_node& two) {
    if (id < two.id )
        return true;
    else if (id == two.id)
        return true;
    return false;
};
//operator overload
size_t REG_node::operator()(const REG_node& pointToHash) const  {
    size_t hash = pointToHash.id;
    return hash;
};    
//operator overload
bool operator==(const REG_node& one, const REG_node& two){
        return one.first_label == two.first_label && one.second_label == two.second_label && one.first_neighbour == two.first_neighbour &&
        one.second_neighbour == two.second_neighbour;
}

//REG constructor
REG::REG(REG_node* start, REG_node* fin) {
    this->start = new REG_node();
    this->finish = new REG_node();
    this->start->first_label = start->first_label;
    this->start->first_neighbour = start->first_neighbour;
    this->start->second_label = start->second_label;
    this->start->second_neighbour = start->second_neighbour;
    this->start->first_label = start->first_label;
    this->start->first_neighbour = start->first_neighbour;
    this->start->first_label = start->first_label;
    this->start->first_neighbour = start->first_neighbour;
}   
//REG constructor
REG::REG(const REG& other) {
    this->start = other.start;
    this->finish = other.finish;
}
//REG constructor
REG::REG(){
    this->start = nullptr;
    this->finish = nullptr;
}
//REG constructor
REG::REG(std::string s){
    REG_node* node1 = new REG_node();
    REG_node* node2 = new REG_node();
    node1->first_label = s;
    node1->first_neighbour = node2;
    this->start = node1;
    this->finish = node2;
}
//REG_node constructor
REG_node::REG_node(){
    this->first_neighbour = nullptr;
    this->first_label = "";
    this->second_neighbour = nullptr;
    this->second_label = "";
}
//hashing function for REG_node
size_t hash_value(const REG_node& node) {
  std::size_t seed = 0;
  std::hash<std::string> string_hasher;

  seed ^= string_hasher(node.first_label);
  seed ^= string_hasher(node.second_label);

  return seed;
}
