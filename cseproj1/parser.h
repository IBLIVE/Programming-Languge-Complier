/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
//NAME: ISKANDER KUSHBAY
//CLASS: CSE340
//PROJECT 1
//parser header file
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <vector>
#include <unordered_set>
#include "structs.h"
#include "lexer.h"
#include "my_LexicalAnalyzer.h"

class Parser {
  public:
    void parse_input(); //print tokens and lexemes
    void readAndPrintAllInput(); //print input
    int num = 0;
    std::vector<TokenStruct> parsedTokens; //storing tokens
    std::string input_text;
    std::string save;
    void syntax_error(); //syntax error
    void syntax_error(std::string& name); //syntax error during parsing
    void check_namesakes(); //semantics chekcing
    void check_epsilon(); //epsilon checking
  private:
    LexicalAnalyzer lexer;
    Token expect(TokenType expected_type); //expect token
    Token expect(TokenType expected_type, std::string& name); //expect token
    void parse_tokens_section(); //parsing tokens
    void parse_token_list(); //parsing token list
    void parse_token(); //parsing token
    REG* parse_expr(Token& t); //parsing expr
};

#endif
