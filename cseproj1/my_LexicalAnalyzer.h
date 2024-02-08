//NAME: ISKANDER KUSHBAY
//CLASS: CSE340
//PROJECT 1
//my_LexcicalAnalyzer header file
#ifndef __MY_LEXICALANALYZER__H__
#define __MY_LEXICALANALYZER__H__

#include "structs.h"
//Match One Char function
std::unordered_set<REG_node, std::hash<REG_node>, std::equal_to<REG_node>> Match_One_Char(std::unordered_set<REG_node, std::hash<REG_node>, std::equal_to<REG_node>>S, std::string& c);

//Class my_lexcicalAnalyzer
class my_LexicalAnalyzer{
    public:
        my_LexicalAnalyzer(std::vector<TokenStruct> &tokensList, std::string text); //constructor
        void print_tokens();//print tokens
    private:
        std::vector<TokenStruct> tokens;
        std::string inputText;
        int pos;
        int match(REG reg, std::string text, int pos); //matching tokens
        Token my_GetToken(); //get max len token
        void error(); //error print
};


#endif