
//NAME: ISKANDER KUSHBAY
//CLASS: CSE340
//PROJECT 1

/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */


#include <iostream>
#include <unordered_set>
#include <map>
#include "parser.h"

using namespace std;

// this syntax error function needs to be 
// modified to produce the appropriate message
//legacy code 
/*std::vector<string> words(string lexeme){
    string word = "";
    vector<string> words;
    for (auto i : lexeme){
        if (i == ' ' || i == '"'){
            if (word != ""){
                words.push_back(word);
            }
            word = "";
        }
        else {
            word += i;
        }
    }
    return words;
}
*/

//syntax error message
void Parser::syntax_error()
{
    cout << "SNYTAX ERORR\n";
    exit(1);
}

//syntax error for expressions
void Parser::syntax_error(string& name){
    cout << "SYNTAX ERROR IN EXPRESSION OF " << name << "\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi

//expect function
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}
Token Parser::expect(TokenType expected_type, std::string& name){
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error(name);
    return t;
} 
//parsing expressions
REG* Parser::parse_expr(Token& id)
{
    Token t = lexer.peek(1);
    REG* t_ptr = nullptr;
    if (t.token_type == CHAR){ //CHAR case
        expect(CHAR, id.lexeme);
        string lex = t.lexeme;
        t_ptr = new REG(lex);
        return t_ptr;
    }
    else if (t.token_type == UNDERSCORE){ //UNDERSCORE case
        expect(UNDERSCORE,id.lexeme);
        string lex = "_";
        t_ptr = new REG(lex);
        return t_ptr;
    }
    else if (t.token_type == LPAREN){ //LPAREN CASE
        expect(LPAREN, id.lexeme);
        REG* t_ptr1 = nullptr;
        t_ptr1 = parse_expr(id);
        expect(RPAREN, id.lexeme); //get RPAREN
        t = lexer.peek(1);
        if (t.token_type == DOT){ //DOT CASE
            //t = lexer.peek(1);
            expect(DOT,id.lexeme);
            //t = lexer.peek(1);
            expect(LPAREN,id.lexeme);
            //REG t_ptr_temp = *t_ptr1;
            REG* t_ptr2;
            t_ptr2 = parse_expr(id);
            expect(RPAREN,id.lexeme);
            t_ptr1->finish->first_neighbour = t_ptr2->start;
            t_ptr1->finish->first_label = '_';
            REG tREG;
            tREG.start = t_ptr1->start;
            tREG.finish = t_ptr2->finish;
            t_ptr = new REG(tREG);
            return t_ptr;
        }
        else if(t.token_type == OR){ //OR CASE
            expect(OR,id.lexeme);
            expect(LPAREN,id.lexeme);
            REG* t_ptr2;
            t_ptr2 = parse_expr(id);
            expect(RPAREN,id.lexeme);
            REG tREG;
            tREG.start = new REG_node();
            tREG.finish = new REG_node();
            tREG.start->first_label = "_";
            tREG.start->second_label = "_";
            tREG.start->first_neighbour = t_ptr1->start;
            tREG.start->second_neighbour = t_ptr2->start;
            t_ptr1->finish->first_label = "_";
            t_ptr2->finish->first_label = "_";
            t_ptr2->finish->first_neighbour = tREG.finish;
            t_ptr1->finish->first_neighbour = tREG.finish;
            t_ptr = new REG(tREG);
            return t_ptr;
        }
        else if(t.token_type == STAR){ //STAR CASE
            expect(STAR,id.lexeme);
            REG tREG;
            tREG.start = new REG_node();
            tREG.finish = new REG_node();
            t_ptr1->finish->first_label = "_";
            t_ptr1->finish->first_neighbour = t_ptr1->start;
            t_ptr1->finish->second_label = "_";
            t_ptr1->finish->second_neighbour = tREG.finish;
            tREG.start->first_label = "_";
            tREG.start->first_neighbour = t_ptr1->start;
            tREG.start->second_label = "_";
            tREG.start->second_neighbour = tREG.finish;
            t_ptr = new REG(tREG);
            return t_ptr;
        }
        else {
            syntax_error(id.lexeme);
        }
    } else {
        syntax_error(id.lexeme);
    }
    return t_ptr; //return
}

//parse token from tokelist
void Parser::parse_token()
{
   Token t = lexer.peek(1);
   if (t.token_type == ID){
    expect(ID, t.lexeme);
    REG* graph = parse_expr(t);
    TokenStruct resultingToken;
    resultingToken.token = t;
    resultingToken.reg = *graph;
    this->parsedTokens.push_back(resultingToken);
   } else {
    syntax_error();
   }
}

//parse tokenlist and check for hash
void Parser::parse_token_list()
{
    parse_token();
    Token t = lexer.peek(1);
    if (t.token_type == COMMA){
        expect(COMMA);
        parse_token_list();
    } else if (t.token_type == HASH){ //check hash
        //return;
    } else {
        syntax_error();
    }
}

//parsing all tokens (line 1)
void Parser::parse_tokens_section()
{
    parse_token_list();
    Token t = lexer.peek(1);
    if (t.token_type == HASH){
        expect(HASH); //checking hash
    } else {
        syntax_error();
    }
}

//parse input
void Parser::parse_input()
{
    //cout << "a";
    parse_tokens_section();
    Token t = lexer.peek(1);
    if (t.token_type == INPUT_TEXT){
        expect(INPUT_TEXT); //checking text
        this->input_text = t.lexeme;
        expect(END_OF_FILE);
    } else {
        syntax_error();
    }
    //check_namesakes(); //check semantics error
    //check_epsilon(); //check epsilon error
}

// This function simply reads and prints all tokens
// I included it as an example. You should compile the provided code
// as it is and then run ./a.out < tests/test0.txt to see what this function does
// This function is not needed for your solution and it is only provided to
// illustrate the basic functionality of getToken() and the Token type.

void Parser::readAndPrintAllInput()
{
    Token t;

    // get a token
    t = lexer.GetToken();

    // while end of input is not reached
    while (t.token_type != END_OF_FILE) 
    {
        t.Print();         	// pringt token
        t = lexer.GetToken();	// and get another one
    }
        
    // note that you should use END_OF_FILE and not EOF
}
 //checking for semantic error
void Parser::check_namesakes(){
    std::map<std::string, int> delpos;
    bool flag = false;
    /*for (int i = 0; i < int(parsedTokens.size()); i++){
        for (int j = i + 1; j < int(parsedTokens.size()); j++){
            if (parsedTokens[j].token.lexeme == parsedTokens[i].token.lexeme && delpos.find(j) == delpos.end()){
                flag = true;
                cout << "Line " << parsedTokens[j].token.line_no << ": " << parsedTokens[j].token.lexeme 
                << " already declared on line " << parsedTokens[i].token.line_no <<"\n";
                delpos.insert(j);
            }
        }
    }*/
    for (int i = 0; i < int(parsedTokens.size()); i++){
        if (delpos.find(parsedTokens[i].token.lexeme)==delpos.end()){
            delpos[parsedTokens[i].token.lexeme] = parsedTokens[i].token.line_no;
        }
        else{
            flag = true;
            cout << "Line " << parsedTokens[i].token.line_no << ": " << parsedTokens[i].token.lexeme 
                << " already declared on line " << delpos[parsedTokens[i].token.lexeme] <<"\n";
        }
    }
    if (flag){
        exit(1);
    }
}
//checking for an epsilon error
void Parser:: check_epsilon(){
    bool flag = false;
    for (int i = 0; i < parsedTokens.size(); i++){
        REG_node* finish = parsedTokens[i].reg.finish;
        REG_node* start = parsedTokens[i].reg.start;
        string epsilon = "_";
        std::unordered_set<REG_node, std::hash<REG_node>, std::equal_to<REG_node>> arg;
        arg.insert(*start);
        std::unordered_set<REG_node, std::hash<REG_node>, std::equal_to<REG_node>> output = Match_One_Char(arg, epsilon);
        if (output.find(*finish) != output.end()){
            if (!flag){
                cout << "EPSILON IS NOOOOOOOT A TOKEN !!!";
            }
            flag = true;
            cout << " " << parsedTokens[i].token.lexeme;
        }
    }
    if (flag){
        std::cout<<"\n";
        exit(1);
    }
}

int main()
{
    // note: the parser class has a lexer object instantiated in it (see file
    // parser.h). You should not be declaring a separate lexer object. 
    // You can access the lexer object in the parser functions as shown in 
    // the example  method Parser::readAndPrintAllInput()
    // If you declare another lexer object, lexical analysis will 
    // not work correctly
    Parser parser; //inst parser
    parser.parse_input();//parsing
    parser.check_namesakes();
    parser.check_epsilon();
    my_LexicalAnalyzer lexique(parser.parsedTokens, parser.input_text); //starting my_LexicalAnalyzer obj
    lexique.print_tokens(); // printing output
    //parser.readAndPrintAllInput();
    return 0;
	
}
