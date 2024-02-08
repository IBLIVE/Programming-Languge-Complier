//NAME: ISKANDER KUSHBAY
//CLASS: CSE340
//PROJECT 1
//my_LexicalAnalyzer class implementation
#include <unordered_set>
#include <iostream>
#include "my_LexicalAnalyzer.h"

//Match_One_Char function as described in project document
std::unordered_set<REG_node, std::hash<REG_node>, std::equal_to<REG_node>> 
Match_One_Char(std::unordered_set<REG_node, std::hash<REG_node>, std::equal_to<REG_node>>S, std::string& c){ //Matching chars
    std::unordered_set<REG_node, std::hash<REG_node>, std::equal_to<REG_node>> S_temp;
    S_temp.clear();
    bool addedLetter = true;
    for(auto it : S){
        if (it.first_label == c){
            //addedLetter = true;
            S_temp.emplace(*it.first_neighbour);
        }
        if (it.second_label == c){
            //addedLetter = true;
            S_temp.emplace(*it.second_neighbour);
        }
    }

    if(S_temp.empty()){
        return S_temp;
    }

    bool changed = true;
    std::unordered_set<REG_node, std::hash<REG_node>, std::equal_to<REG_node>> S_newtemp;
    S_newtemp.clear();

    while (changed) {
        changed = false;
        for (auto it: S_temp) {
            S_newtemp.insert(it);
            if (it.first_label == "_" && (S_newtemp.find(*it.first_neighbour) == S_newtemp.end())) {
                S_newtemp.insert(*it.first_neighbour);
            }
            if (it.second_label == "_" && (S_newtemp.find(*it.second_neighbour) == S_newtemp.end())){
                S_newtemp.emplace(*it.second_neighbour);
            }
        }
        if (S_temp.size() != S_newtemp.size()){
            changed = true;
            S_temp = S_newtemp;
            S_newtemp.clear();
        }
    }

    return S_temp;
}

//printing errors
void my_LexicalAnalyzer::error()
{
    std::cout << "ERROR" << "\n";
    exit(1);
}

//class constructor
my_LexicalAnalyzer::my_LexicalAnalyzer(std::vector<TokenStruct> &tokensList, std::string text){
    this->tokens = tokensList;
    this->inputText = text;
    this->pos = 0;
}

//matching regs function match
int my_LexicalAnalyzer::match(REG reg, std::string text, int pos){
    std::unordered_set<REG_node, std::hash<REG_node>, std::equal_to<REG_node>> resNodes;
    std::string s;
    std::unordered_set<REG_node, std::hash<REG_node>, std::equal_to<REG_node>> inputNodes;
    std::unordered_set<REG_node, std::hash<REG_node>, std::equal_to<REG_node>> inputNodes_t;
    inputNodes.emplace(*reg.start);
    std::string epsilon = "_";
    inputNodes_t = Match_One_Char(inputNodes, epsilon);
    for (auto i: inputNodes_t){
        inputNodes.emplace(i);
    }
    int n = text.size();
    int pos_t = 0;
    for (int i = pos; i < n; i++){
        s = "";
        s += text[i];
        if (isalnum(text[i])){
            resNodes = Match_One_Char(inputNodes, s);
            if (resNodes.find(*reg.finish) != resNodes.end()){
                pos_t = i;
            }
        } else if (s == "\"") {
            continue;
        } else if (s == " ") {
            break;
        } else {
            error();
        }
        inputNodes = resNodes;
    }
    return pos_t;
}

//my_GetToken function to get maximum length word token
Token my_LexicalAnalyzer::my_GetToken(){
    int pos = this->pos;
    while (pos < inputText.size() && !isalnum(inputText[pos])){
        pos+=1;
    }
    int pos_t = 0;
    pos_t *=0;
    int new_pos = 0;
    new_pos *= 0;
    Token maxToken = tokens[0].token;
    for (auto i: tokens){
        pos_t = this->match(i.reg, this->inputText, pos);
        if (pos_t > new_pos){
            maxToken = i.token;
            new_pos = pos_t;
        }
    }
    if (new_pos < pos && pos < this->inputText.size()){
        error();
    }
    this->pos = new_pos + 1;
    return maxToken;
}

// finding tokens and printing them function
void my_LexicalAnalyzer::print_tokens(){
    int n = this->inputText.size();
    while(this->pos < n){
        int t_pos = pos;
        Token t = my_GetToken();
        while (!isalnum(this->inputText[t_pos]) && t_pos <= this->pos){
            t_pos += 1;
        }
        std::string word = this->inputText.substr(t_pos, this->pos-t_pos);
        std::cout << t.lexeme << ", \"" << word << "\"\n";
        while (!isalnum(this->inputText[this->pos]) && this->pos < n){
            this->pos += 1;
        }
    }
}