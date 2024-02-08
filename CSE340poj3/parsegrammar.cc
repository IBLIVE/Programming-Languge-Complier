//NAME: ISKANDER KUSHBAY
//CLASS: CSE340
//PROJECT 3
#include <string>
#include <vector>
#include <map>
#include "execute.h"
#include "lexer.h"
#include <cstdlib>
LexicalAnalyzer lexer;
int count = 0; 
std::map<std::string, int> location_table;

struct InstructionNode* parse_program();
void parse_var_section();
void parse_id_list();
InstructionNode* parse_body();
InstructionNode* parse_stmt_list();
InstructionNode* parse_stmt();
InstructionNode* parse_case_list(std::string& var, InstructionNode* label);
void parse_inputs();

void error(){
    //std::cout << "Error!";
    exit(1);
}

struct InstructionNode* parse_Generate_Intermediate_Representation(){
    //std::cout << " JELLO ";
    InstructionNode* prog = parse_program();
    return prog;
}

struct InstructionNode* parse_program(){
    parse_var_section();
    InstructionNode* prog = parse_body();
    parse_inputs();
    return prog;
}

void parse_var_section(){
    parse_id_list();
    if (lexer.peek(1).token_type == SEMICOLON){
        Token t = lexer.GetToken();
    } else {
        //std::cout << " HELLO ";
        error();
    }
}

void parse_id_list(){
    Token t = lexer.peek(1);
    if(t.token_type == ID){
        t = lexer.GetToken();
        if (location_table.find(t.lexeme) == location_table.end()){
            //std::cout << t.lexeme << std::endl;
            location_table[t.lexeme] = count;
            mem[count] = 0;
            count++;
        }
        t = lexer.peek(1);
        if(t.token_type == COMMA){
            t = lexer.GetToken();
            parse_id_list();
        } else{}
    } else {
        error();
    }
}

struct InstructionNode* parse_body(){
    Token t = lexer.peek(1);
    InstructionNode* prog = nullptr;
    if (t.token_type == LBRACE){
        t = lexer.GetToken();//LBRACE
        //std::cout << "{\n";
        prog = parse_stmt_list();
        if (lexer.peek(1).token_type == RBRACE){
            //std::cout << "}\n";
            t = lexer.GetToken();//RBRACE
        } else {
            error();
        }
    } else {
        error();
    }
    return prog;
}

struct InstructionNode* parse_stmt_list(){
    InstructionNode* prog = nullptr;
    InstructionNode* prog2 = nullptr;
    prog = parse_stmt();
    Token t = lexer.peek(1); // statement
    if (t.lexeme == "output"){
        prog2 = parse_stmt_list();
        InstructionNode* temp = prog;
        while (temp->next != nullptr){
            temp = temp->next;
        }
        temp->next = prog2;
    } else if (t.lexeme == "input") {
        prog2 = parse_stmt_list();
        InstructionNode* temp = prog;
        while (temp->next != nullptr){
            temp = temp->next;
        }
        temp->next = prog2;
    } else if (t.token_type == ID || t.token_type == SWITCH || 
        t.token_type == FOR || t.token_type == WHILE || t.token_type == IF){
        prog2 = parse_stmt_list();
        InstructionNode* temp = prog;
        while (temp->next != nullptr){
            temp = temp->next;
        }
        temp->next = prog2;
    } else if (t.token_type == RBRACE){
    } else {
        error();
    }
    return prog;
}

struct InstructionNode* parse_stmt(){
    Token t = lexer.peek(1);
    InstructionNode* prog = new InstructionNode;
    prog->next = nullptr;
    if (t.lexeme == "output"){
        prog->type = OUT;
        lexer.GetToken(); //output
        t = lexer.GetToken(); //ID
        prog->output_inst.var_index = location_table[t.lexeme];
        prog->next = nullptr;
        lexer.GetToken(); //SEMICOLON
    } else if (t.lexeme == "input"){
        prog->type = IN;
        prog->next = nullptr;
        lexer.GetToken(); //input
        t = lexer.GetToken(); //ID
        prog->input_inst.var_index = location_table[t.lexeme];
        lexer.GetToken(); //SEMICOLON
    } else if (t.token_type == ID){
        prog->type = ASSIGN;
        prog->next = nullptr;
        t = lexer.GetToken(); //ID
        std::string var1 = t.lexeme;
        t = lexer.GetToken(); // EQUAL
        t = lexer.GetToken(); // NUM or ID
        if (location_table.find(t.lexeme) == location_table.end() && t.token_type == NUM){
            location_table[t.lexeme] = count;
            count++;
            mem[location_table[t.lexeme]] = stoi(t.lexeme);
        }
        std::string var2 = t.lexeme;
        //prog->assign_inst.left_hand_side_index = location_table[var];
        t = lexer.GetToken(); // OP or SEMICOLON
        if (t.token_type == SEMICOLON){
            prog->assign_inst.opernd1_index = location_table[var2];
            prog->assign_inst.left_hand_side_index = location_table[var1];
            prog->assign_inst.op = OPERATOR_NONE;
            //std::cout << var1 << " = " << var2 << "\n";
        } else {
            std::string var3;
            if (t.token_type==MULT){
                prog->assign_inst.op = OPERATOR_MULT;
                var3 = "*";
            } else if (t.token_type==PLUS){
                prog->assign_inst.op = OPERATOR_PLUS;
                var3 = "+";
            } else if (t.token_type == DIV){
                prog->assign_inst.op = OPERATOR_DIV; 
                var3 = "\\";
            } else if (t.token_type == MINUS){
                prog->assign_inst.op = OPERATOR_MINUS; 
                var3 = "-";
            }
            t = lexer.GetToken(); // NUM or ID
            if (location_table.find(t.lexeme) == location_table.end() && t.token_type == NUM){
                location_table[t.lexeme] = count;
                count++;
                mem[location_table[t.lexeme]] = stoi(t.lexeme);
            }
            prog->assign_inst.opernd1_index = location_table[var2];
            prog->assign_inst.opernd2_index = location_table[t.lexeme];
            prog->assign_inst.left_hand_side_index = location_table[var1];
            lexer.GetToken();//SEMICOLON
            //std::cout << var1 << " = " << var2 << var3 << t.lexeme<<"\n";
            prog->next = nullptr;
        }
    } else if (t.token_type == SWITCH){
        lexer.GetToken(); //SWITCH
        t = lexer.GetToken(); //ID
        std::string var = t.lexeme;
        lexer.GetToken(); //LBRACE

        InstructionNode* switchNode;
        InstructionNode* label = new InstructionNode;
        label->type = NOOP;
        label->next = nullptr;

        switchNode = parse_case_list(var, label);
        prog = switchNode;
        lexer.GetToken(); //RBRACE
        //prog->type = CJMP;
        //prog->cjmp_inst.condition_op;
        //prog = parse_switch_stmt();
    } else if (t.token_type == FOR){
        lexer.GetToken(); //FOR
        lexer.GetToken(); //LPAREN
        InstructionNode* statement1 = parse_stmt();//stmt1

        InstructionNode* condition = new InstructionNode;//condition
        condition->type = CJMP;
        t = lexer.GetToken(); // ID or NUM
        if (t.token_type == ID || t.token_type == NUM){
            if (location_table.find(t.lexeme) == location_table.end() && t.token_type == NUM){
                location_table[t.lexeme] = count;
                count++;
                mem[location_table[t.lexeme]] = stoi(t.lexeme);
            }
            condition->cjmp_inst.opernd1_index = location_table[t.lexeme];
            t = lexer.GetToken(); // OP
            if (t.token_type == GREATER){
                condition->cjmp_inst.condition_op = CONDITION_GREATER;
            } else if (t.token_type == LESS){
                condition->cjmp_inst.condition_op = CONDITION_LESS;
            } else if (t.token_type == NOTEQUAL){
                condition->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
            }
            t = lexer.GetToken(); // NUM or ID
            if (location_table.find(t.lexeme) == location_table.end() && t.token_type == NUM){
                location_table[t.lexeme] = count;
                count++;
                mem[location_table[t.lexeme]] = stoi(t.lexeme);
            }
            condition->cjmp_inst.opernd2_index = location_table[t.lexeme];
        } else {
            error();
        }

        lexer.GetToken(); //SEMICOLON
        InstructionNode* statement2 = parse_stmt();//stmt2
        lexer.GetToken(); //RPAREN
        InstructionNode* body = parse_body();//body

        prog = statement1;
        prog->next = condition;
        condition->next = body;
        InstructionNode* target = new InstructionNode;
        InstructionNode* jump = new InstructionNode;
        jump->type = JMP;
        jump->next = nullptr;
        jump->jmp_inst.target = condition;
        target->type = NOOP;
        target->next = nullptr;
        InstructionNode* temp;
        temp = condition;
        while(temp->next != nullptr){
            temp = temp->next;
        }
        temp->next = statement2;
        statement2->next = jump;
        condition->cjmp_inst.target = target;
        jump->next = target;
    } else if (t.token_type == IF){
        t = lexer.GetToken(); // IF
        prog->type = CJMP;
        t = lexer.GetToken(); // ID or NUM
        if (t.token_type == ID || t.token_type == NUM){
            std::string var = t.lexeme;
            if (location_table.find(t.lexeme) == location_table.end() && t.token_type == NUM){
                location_table[t.lexeme] = count;
                count++;
                mem[location_table[t.lexeme]] = stoi(t.lexeme);
            }
            prog->cjmp_inst.opernd1_index = location_table[t.lexeme];
            t = lexer.GetToken(); // OP
            if (t.token_type == GREATER){
                prog->cjmp_inst.condition_op = CONDITION_GREATER;
                //std::cout << var << " > ";
            } else if (t.token_type == LESS){
                prog->cjmp_inst.condition_op = CONDITION_LESS;
            } else if (t.token_type == NOTEQUAL){
                prog->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
            }
            t = lexer.GetToken(); // NUM or ID
            if (location_table.find(t.lexeme) == location_table.end() && t.token_type == NUM){
                location_table[t.lexeme] = count;
                count++;
                mem[location_table[t.lexeme]] = stoi(t.lexeme);
            }
            prog->cjmp_inst.opernd2_index = location_table[t.lexeme];
            //std::cout << location_table[var] << " > " << location_table[t.lexeme] <<"\n";
            InstructionNode* target = new InstructionNode;
            target->type = NOOP;
            target->next = nullptr;
            prog->next = parse_body();
            InstructionNode* temp;
            temp = prog;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            temp->next = target;
            prog->cjmp_inst.target = target;
            //prog->next = target;
            //lexer.GetToken(); // SEMICOLON
        } else {
            error();
        }
    } else if (t.token_type == WHILE){
        prog->type = CJMP;
        lexer.GetToken(); //WHILE
        t = lexer.GetToken(); // ID or NUM
        if (t.token_type == ID || t.token_type == NUM){
            if (location_table.find(t.lexeme) == location_table.end() && t.token_type == NUM){
                location_table[t.lexeme] = count;
                count++;
                mem[location_table[t.lexeme]] = stoi(t.lexeme);
            }
            prog->cjmp_inst.opernd1_index = location_table[t.lexeme];
            t = lexer.GetToken(); // OP
            if (t.token_type == GREATER){
                prog->cjmp_inst.condition_op = CONDITION_GREATER;
            } else if (t.token_type == LESS){
                prog->cjmp_inst.condition_op = CONDITION_LESS;
            } else if (t.token_type == NOTEQUAL){
                prog->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
            }
            t = lexer.GetToken(); // NUM or ID
            if (location_table.find(t.lexeme) == location_table.end() && t.token_type == NUM){
                location_table[t.lexeme] = count;
                count++;
                mem[location_table[t.lexeme]] = stoi(t.lexeme);
            }
            prog->cjmp_inst.opernd2_index = location_table[t.lexeme];
            InstructionNode* target = new InstructionNode;
            InstructionNode* jump = new InstructionNode;
            jump->type = JMP;
            jump->next = nullptr;
            jump->jmp_inst.target = prog;
            target->type = NOOP;
            target->next = nullptr;
            prog->next = parse_body();
            InstructionNode* temp;
            temp = prog;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            temp->next = jump;
            prog->cjmp_inst.target = target;
            jump->next = target;
            //lexer.GetToken(); // SEMICOLON
        } else {
            error();
        }
    }
    return prog;
}

/*
void parse_expr();
void parse_primary();
void parse_op();
InstructionNode* parse_output_stmt();
InstructionNode* parse_input_stmt();
InstructionNode* parse_if_stmt();
InstructionNode* parse_while_stmt();
void parse_condition();
void parse_relop();
InstructionNode* parse_switch_stmt();
InstructionNode* parse_for_stmt();*/

struct InstructionNode* parse_case_list(std::string& var, InstructionNode* label){
    lexer.GetToken(); //CASE
    Token t = lexer.GetToken(); //NUM
    std::string num = t.lexeme;
    if (location_table.find(num) == location_table.end() && t.token_type == NUM){
        location_table[t.lexeme] = count;
        count++;
        mem[location_table[t.lexeme]] = stoi(t.lexeme);
    }
    lexer.GetToken(); //COLON

    InstructionNode* prog = new InstructionNode;
    prog->next = nullptr;
    prog->type = CJMP;
    prog->cjmp_inst.opernd1_index = location_table[var];
    prog->cjmp_inst.opernd2_index = location_table[num];
    prog->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    InstructionNode* body = parse_body();
    InstructionNode* temp = body;
    while(temp->next != nullptr){
        temp = temp->next;
    }
    temp->next = label;
    prog->cjmp_inst.target = body;
    t = lexer.peek(1);
    if (t.token_type == CASE){
        InstructionNode* nextcase = parse_case_list(var, label);
        prog->next = nextcase;
    } else if (t.token_type == DEFAULT){
        lexer.GetToken(); //DEFAULT
        lexer.GetToken(); //COLON
        InstructionNode* defaultCase = parse_body();
        prog->next = defaultCase;
        temp = defaultCase;
        while(temp->next != nullptr){
            temp = temp->next;
        }
        temp->next = label;
    } else {
        prog->next = label;
    }
    return prog;
}

/*
void parse_case();
void parse_default_case();
*/

void parse_inputs(){
    Token t = lexer.peek(1);//NUM
    if (t.token_type == NUM){
        lexer.GetToken();
        inputs.push_back(stoi(t.lexeme));
        parse_inputs();
    }
}

