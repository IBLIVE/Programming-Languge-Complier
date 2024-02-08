//NAME: ISKANDER KUSHBAY
//CLASS: CSE340
//PROJECT 2
/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <unordered_map>
#include <map>
#include <string>
#include "lexer.h"
#include<unordered_set>
using namespace std;

struct rule{
    string lhs;
    vector<string> rhs;
};

struct prefRules{
    int index; 
    rule pravilo;
    int len = 0;
    vector<int> ruleList = {};
};

unordered_map<string, set<string>> FIRST;
unordered_map<string, set<string>> FOLLOW;
set<string> NT;
set<string> TN;
unordered_map<string, vector<rule>> NTrules;
vector<string> alphabet = {"#", "$"};
int iter = 2;
vector<rule> rules;
map<string, int> nt_counter;
vector<string> t; 
void setUnion(set<string>&A, set<string>& B);
vector<string> extractPrefix(vector<string>& rightside, int len);
vector<string> extractAllButPrefix(vector<string>& rightside, int len);
bool isEpsilon(set<string>& A);
bool checkEpsilon(set<string>& A);
void printContent(set<string> A);
void ReadGrammar(LexicalAnalyzer& lexer);
void parse_grammar(LexicalAnalyzer& lexer);
void parse_ruleList(LexicalAnalyzer& lexer);
void parse_rule(LexicalAnalyzer& lexer);
void parse_rhs(LexicalAnalyzer& lexer, vector<string>& rside);
void parse_idList(LexicalAnalyzer& lexer, vector<string>& rside);
int common_prefix(rule a, rule b);
void syntax_error();
void sortlexique(vector<prefRules>& prefList);
void swaprules(prefRules& a, prefRules&b);
vector<rule> determineRules(string& A);
void addRules(rule r);
pair<vector<rule>, vector<rule>> sepRules(string& A);
pair<vector<rule>, vector<rule>> delImmedRecur(vector<rule>& A, vector<rule>& A1);
void Task1();
void Task2();
void Task3();
void Task4();
void Task5();

void setUnion(set<string>& A, set<string>& B){
    for (auto i: B){
        if (i != "#"){
            A.emplace(i);
        }
    }
}
void printrule(rule r){
    cout << r.lhs << " -> ";
    for (auto i: r.rhs){
        cout << i << " ";
    }
}
bool isEpsilon(set<string>& A){
    set<string> B = A;
    A.emplace("#");
    if (B != A){
        return true;
    }
    return false;
}

bool checkEpsilon(set<string>& A){
    bool check = A.find("#") != A.end();
    return check;
}

void printContent(set<string> A){
    for (auto i: A){
        std::cout << i << " ";
    }
}

int common_prefix(rule a, rule b){
    int count = 0;
    int i = 0;
    while (i < a.rhs.size() && i < b.rhs.size()){
        if (a.rhs[i] == b.rhs[i]){
            count++;
        } else {
            i = a.rhs.size();
            break;
        }
        i++;
    }
    return count;
}
// read grammar
void ReadGrammar(LexicalAnalyzer& lexer)
{
    //cout << "1";
    parse_grammar(lexer);
    //fflush(stdin);
    //fflush(stdout);
    //fflush(stdin);
    //fflush(stdout);
    //fflush(stdin);
    //Task1();
}

void parse_grammar(LexicalAnalyzer& lexer){
    //cout << "2";
    parse_ruleList(lexer);
    Token t = lexer.peek(1);
    if (t.token_type == HASH){
        lexer.GetToken();
        t = lexer.peek(1);
        if (t.token_type != END_OF_FILE){
            syntax_error();
        }
    } else {
        syntax_error();
    }
}

void parse_ruleList(LexicalAnalyzer& lexer){
    //cout << "3";
    parse_rule(lexer);
    Token t = lexer.peek(1);
    if (t.token_type == HASH){
        int i = 0;
    } else if (t.token_type == ID){
        parse_ruleList(lexer);
    }
    else {
        syntax_error();
    }
}

void parse_rule(LexicalAnalyzer& lexer){
    //cout << "4 ";
    rule Rule;
    Token t = lexer.peek(1);
    if (t.token_type != ID){
        syntax_error();
    }
    if (TN.find(t.lexeme) == TN.end() && NT.find(t.lexeme) == NT.end()){
        alphabet.push_back(t.lexeme);
        iter++;
    }
    Rule.lhs = t.lexeme;
    lexer.GetToken();
    NT.emplace(t.lexeme);

    t = lexer.peek(1);
    if (t.token_type != ARROW){
        syntax_error();
    }
    lexer.GetToken();

    parse_rhs(lexer, Rule.rhs);
    rules.push_back(Rule);
    t = lexer.peek(1);
    if (t.token_type == STAR){
        lexer.GetToken();
    } else {
        syntax_error();
    }
}

void parse_rhs(LexicalAnalyzer& lexer, vector<string>& rside){
    Token t = lexer.peek(1);
    if(t.token_type == ID){
        parse_idList(lexer, rside);
    } else if (t.token_type == STAR){
        rside.push_back("#");
    } else {
        syntax_error();
    }
}

void parse_idList(LexicalAnalyzer& lexer, vector<string>& rside){
    Token t = lexer.peek(1);
    if(t.token_type == ID){
        if (TN.find(t.lexeme) == TN.end() && NT.find(t.lexeme) == NT.end()){
            alphabet.push_back(t.lexeme);
            iter++;
        }
        TN.emplace(t.lexeme);
        rside.push_back(t.lexeme);
        lexer.GetToken();
        t = lexer.peek(1);
        if(t.token_type == ID){
            parse_idList(lexer, rside);
        } else if(t.token_type == STAR){
            ;
        } else {
            syntax_error();
        }
    } else {
        syntax_error();
    }
}

void separate(){
    set <string> terminals;

    for (auto i: TN){
        if (NT.find(i) == NT.end()){
            terminals.emplace(i);
        }
    }
    fflush(stdin);
    fflush(stdout);
    TN.clear();
    for (auto i: terminals){
        TN.emplace(i);
    }
}

void addRules(rule r){
    string B = r.rhs[0];
    vector<rule>subRules = determineRules(B);
    for(auto i: subRules){
        rule newRule;
        newRule.lhs = r.lhs;
        for (auto j: i.rhs){
            newRule.rhs.push_back(j);
        }
        for (int k = 1; k < r.rhs.size(); k++){
            newRule.rhs.push_back(r.rhs[k]);
        }
        rules.push_back(newRule);
    }
}

pair<vector<rule>, vector<rule>> delImmedRecur(vector<rule>& AA, vector<rule>& A){
    string newS = AA[0].lhs + to_string(nt_counter[AA[0].lhs]);
    nt_counter[AA[0].lhs]++;
    vector<rule> newA;
    if (A.size() == 0){
        newA = {};
    } else {
        newA = A;
    }
    for (int i = 0; i < newA.size(); i++){
        newA[i].rhs.push_back(newS);
    }
    /*
    for (auto i: newA.size()){
        i.rhs.push_back(newS);
    }*/
    vector<rule> A1 = {};
    for(auto i: AA){
        rule newRule;
        newRule.lhs = newS;
        for(int j = 1; j < i.rhs.size(); j++){
            newRule.rhs.push_back(i.rhs[j]);
        }
        newRule.rhs.push_back(newS);
        A1.push_back(newRule);
    }
    pair<vector<rule>, vector<rule>> ans = make_pair(newA, A1);
    int y = 0;
    return ans;
}

vector<rule> determineRules(string& A){
    vector<rule> ans;
    for (auto i: NTrules[A]){
        if (i.lhs == A){
            ans.push_back(i);
        }
    }
    return ans;
}

vector<string> extractPrefix(vector<string>& rightside, int len){
    vector<string> pref;
    for (int i = 0; i < len; i++){
        pref.push_back(rightside[i]);
    }
    return pref;
};

vector<string> extractAllButPrefix(vector<string>& rightside, int len){
    vector<string> nonpref;
    for (int i = len; i < rightside.size(); i++){
        nonpref.push_back(rightside[i]);
    }
    return nonpref;
}

pair<vector<rule>, vector<rule>> sepRules(string& A){
    vector<rule> subRules = determineRules(A);
    vector<rule> hasA = {};
    vector<rule> hasntA = {};
    for(auto i: subRules){
        if(i.rhs[0] == A){
            hasA.push_back(i);
        } else {
            hasntA.push_back(i);
        }
    }
    pair<vector<rule>, vector<rule>> ans(hasA, hasntA);
    return ans;
}

void syntax_error(){
    std::cout << "SYNTAX ERROR !!!";
    exit(1);
}
/*
void sortlexique(vector<prefRules>& prefList){
    for(int i = 0; i < prefList.size() - 1; i++){
        for(int j = 0; j < prefList.size() - 1 - i; j++){
            if (prefList[j].len < prefList[j+1].len){
                prefRules t = prefList[j];
                prefList[j] = prefList[j+1];
                prefList[j+1] = t;
            }
            else if (prefList[j].len == prefList[j+1].len){
                if (prefList[j].pravilo.lhs > prefList[j+1].pravilo.lhs){
                    prefRules t = prefList[j];
                    prefList[j] = prefList[j+1];
                    prefList[j+1] = t;
                } else if (prefList[j].pravilo.lhs == prefList[j+1].pravilo.lhs){
                    int k = 0;
                    bool flag = 1;
                    while (flag && k < prefList[j].pravilo.rhs.size() && k < prefList[j+1].pravilo.rhs.size()){
                        if (prefList[j].pravilo.rhs[k] > prefList[j+1].pravilo.rhs[k]){
                            prefRules t = prefList[j];
                            prefList[j] = prefList[j+1];
                            prefList[j+1] = t;
                            flag = false;
                        } else if (prefList[j].pravilo.rhs[k] < prefList[j+1].pravilo.rhs[k]) {
                            flag = false;
                        }
                        k++;
                    }
                    if (flag && prefList[j].pravilo.rhs.size() > prefList[j+1].pravilo.rhs.size()){
                        prefRules t = prefList[j];
                        prefList[j] = prefList[j+1];
                        prefList[j+1] = t;
                    }
                }

            }
        }
    }

    for (int r = 0; r < prefList.size(); r++){
        for (int j = 0; j < prefList.size(); j++){
            if (r != j && (prefList[r].pravilo.lhs == prefList[j].pravilo.lhs)){
                int preflen = common_prefix(prefList[r].pravilo, prefList[j].pravilo);
                if (preflen == prefList[r].len){
                    prefList[r].ruleList.push_back(j);
                }
            }
        }
    }
}*/

void sortlexique(vector<prefRules>& prefList){
    for(int i = 0; i < prefList.size(); i++){
        int maxlen = 0;
        for(int j = 0; j < prefList.size(); j++){
            if(i != j && prefList[i].pravilo.lhs == prefList[j].pravilo.lhs && common_prefix(prefList[i].pravilo, prefList[j].pravilo) > maxlen){
                maxlen = common_prefix(prefList[i].pravilo, prefList[j].pravilo);
            }
        }
        prefList[i].len = maxlen;
    }
    int n = prefList.size();
    for(int i = 0; i < prefList.size() - 1; i++){
        for(int j = 0; j < prefList.size() - i - 1; j++){
            if(prefList[j].len < prefList[j+1].len){
                prefRules t = prefList[j];
                prefList[j] = prefList[j+1];
                prefList[j+1] = t;
            } else if (prefList[j].len == prefList[j+1].len){
                if(prefList[j].pravilo.lhs > prefList[j+1].pravilo.lhs){
                    prefRules t = prefList[j];
                    prefList[j] = prefList[j+1];
                    prefList[j+1] = t;
                } else if(prefList[j].pravilo.lhs == prefList[j+1].pravilo.lhs){
                    int k = 0;
                    bool flag = true;
                    while(flag && k < prefList[j].pravilo.rhs.size() && k < prefList[j+1].pravilo.rhs.size()){
                        if(prefList[j].pravilo.rhs[k] > prefList[j+1].pravilo.rhs[k]){
                            prefRules t = prefList[j];
                            prefList[j] = prefList[j+1];
                            prefList[j+1] = t;
                            flag = false;
                        } else if (prefList[j].pravilo.rhs[k] == prefList[j+1].pravilo.rhs[k]){
                            k++;
                        } else {
                            flag = false;
                        }
                    } 
                    if(flag){
                        if (prefList[j].pravilo.rhs.size() > prefList[j+1].pravilo.rhs.size()){
                            prefRules t = prefList[j];
                            prefList[j] = prefList[j+1];
                            prefList[j+1] = t;
                        }
                    }
                }
            }
        }
    }
}

void sortlexiqueFinal(vector<rule>& prefList){
    for(int i = 0; i < prefList.size() - 1; i++){
        for(int j = 0; j < prefList.size() - 1 - i; j++){
            if (prefList[j].lhs > prefList[j+1].lhs){
                    rule t = prefList[j];
                    prefList[j] = prefList[j+1];
                    prefList[j+1] = t;
            } else if (prefList[j].lhs == prefList[j+1].lhs){
                int k = 0;
                bool flag = 1;
                    while (flag && k < prefList[j].rhs.size() && k < prefList[j+1].rhs.size()){
                        if (prefList[j].rhs[k] > prefList[j+1].rhs[k]){
                            rule t = prefList[j];
                            prefList[j] = prefList[j+1];
                            prefList[j+1] = t;
                            flag = false;
                        } else if (prefList[j].rhs[k] < prefList[j+1].rhs[k]) {
                            flag = false;
                        }
                        k++;
                    }
                    if (flag && prefList[j].rhs.size() > prefList[j+1].rhs.size()){
                        rule t = prefList[j];
                        prefList[j] = prefList[j+1];
                        prefList[j+1] = t;
                    }
                }

            }
        }
}

void swaprules(prefRules& a, prefRules&b){
    rule ind;
    int leng;
    string st;
    vector<int> asocrules;
    ind = a.pravilo;
    leng = a.len;
    asocrules = a.ruleList;

    a.pravilo = b.pravilo;
    a.len = b.len;
    a.ruleList = b.ruleList;

    b.pravilo = ind;
    b.len = leng;
    b.ruleList = asocrules;
}

void printFIRST(){
    for(auto i: alphabet){
        if (NT.find(i) != NT.end()){
            std::cout << "FIRST(" << i << ") = { ";
            string out = ""; 
            bool flag = false;
            for (auto j: alphabet){
                if (FIRST[i].find(j) != FIRST[i].end()){
                    flag = true;
                    out += j;
                    out += ", ";
                }
            }
            if (flag){
                out.pop_back();
                out.pop_back();
            }
            std::cout << out << " }" << endl;
        }
    }
}

void printFOLLOW(){
    for(auto i: alphabet){
        if (NT.find(i) != NT.end()){
            std::cout << "FOLLOW(" << i << ") = { ";
            string out = ""; 
            bool flag = false;
            for (auto j: alphabet){
                if (FOLLOW[i].find(j) != FOLLOW[i].end()){
                    flag = true;
                    out += j;
                    out += ", ";
                }
            }
            if (flag){
                out.pop_back();
                out.pop_back();
            }
            std::cout << out << " }" << endl;
        }
    }
}
// Task 1
void Task1()
{
    separate();
    set<string> TNt = TN;
    for (auto i: alphabet){
        if (TN.find(i) != TN.end()){
            std::cout << i << " ";
            std::cout.flush();
            TN.erase(i);
        }
    }
    TN = TNt;
    set<string> NTt = NT;
    for (auto i: alphabet){
        if (NT.find(i) != NT.end()){
            std::cout << i << " ";
            std::cout.flush();
            NT.erase(i);
        }
    }
    NT = NTt;
    cout << endl;
}

// Task 2
void Task2()
{
    set <string> terminals;
    for (auto i: TN){
        if (NT.find(i) == NT.end()){
            terminals.emplace(i);
        }
    }
    fflush(stdin);
    fflush(stdout);
    TN.clear();
    for (auto i: terminals){
        TN.emplace(i);
    }

    FIRST["#"] = {"#"};
    for (auto i: TN){
        FIRST[i] = {i};
    }
    for (auto i: NT){
        FIRST[i] = {};
    }
    bool changed = true;
    while (changed){
        changed = false;
        for (auto i: rules){
            set<string> tempSet = FIRST[i.lhs];
            setUnion(FIRST[i.lhs], FIRST[i.rhs[0]]);

            for (int j = 1; j < i.rhs.size(); j++){
                if (!checkEpsilon(FIRST[i.rhs[j-1]])){
                    break;
                }
                if (checkEpsilon(FIRST[i.rhs[j-1]])){        
                    setUnion(FIRST[i.lhs], FIRST[i.rhs[j]]);
                }
            }

            int flag = true;
            for (int j = 0; j < i.rhs.size(); j++){
                if (!checkEpsilon(FIRST[i.rhs[j]])){
                    flag = false;
                    break;
                }
            }
            if (flag){
                FIRST[i.lhs].emplace("#");
            }
            if (tempSet != FIRST[i.lhs]){
                changed = true;
            }
        }
    }
}

// Task 3
void Task3()
{
    Task2();
    //cout << "F\n";

    for (auto i: NT){
        FOLLOW[i] = {};
    }
    FOLLOW[alphabet[2]] = {"$"};
    for (auto i: rules){
        for(int j = 0; j < i.rhs.size()-1; j++){
            if (NT.find(i.rhs[j]) != NT.end()){
                setUnion(FOLLOW[i.rhs[j]], FIRST[i.rhs[j+1]]);
                for(int k = j+2; k < i.rhs.size(); k++){
                    if (!checkEpsilon(FIRST[i.rhs[k-1]])){
                        break;
                    }
                    setUnion(FOLLOW[i.rhs[j]], FIRST[i.rhs[k]]);
                }
            }
        }
    }

    bool changed = true;
    while (changed){
        changed = false;
        for (auto i: rules){
            set<string> tempSet;
            if (NT.find(i.rhs[i.rhs.size()-1]) != NT.end()){
                tempSet = FOLLOW[i.rhs[i.rhs.size()-1]];
                setUnion(FOLLOW[i.rhs[i.rhs.size()-1]], FOLLOW[i.lhs]);
                if (FOLLOW[i.rhs[i.rhs.size()-1]] != tempSet){
                    changed = true;
                }
            }

            for (int j = 0; j < i.rhs.size(); j++){
                if(NT.find(i.rhs[j]) != NT.end()){
                    bool flag = true;
                    tempSet = FOLLOW[i.rhs[j]];
                    for(int k = j+1; k < i.rhs.size(); k++){
                        if(!checkEpsilon(FIRST[i.rhs[k]])){
                            flag = false;
                            break;
                        }
                    }
                    if(flag){
                        setUnion(FOLLOW[i.rhs[j]], FOLLOW[i.lhs]);
                    }
                    if(tempSet != FOLLOW[i.rhs[j]]){
                        changed = true;
                    }
                }
            }
        }
    }

}

// Task 4

void Task4()
{
    separate();
    for (auto r: NT){
        nt_counter[r] = 1;
    }
    
    vector<prefRules> commonRules = {};
    vector<rule> nrules = {};
    set<string> nTN = TN;
    set<string> nNT = {};
    vector<rule> rulest = rules;
    set<string> iterNT = NT;
    for(auto r: rules){
        prefRules newRule;
        newRule.pravilo = r;
        commonRules.push_back(newRule);
    }
    
    for(int i = 0; i < commonRules.size(); i++){
        int maxlen = 0;
        for(int j = 0; j < commonRules.size(); j++){
            if(i != j && commonRules[i].pravilo.lhs == commonRules[j].pravilo.lhs && common_prefix(commonRules[i].pravilo, commonRules[j].pravilo) > maxlen){
                maxlen = common_prefix(commonRules[i].pravilo, commonRules[j].pravilo);
            }
        }
        commonRules[i].len = maxlen;
    }

    sortlexique(commonRules);

    /*for (int r = 0; r < rules.size(); r++){
        int maxlen = 0;
        for (int j = 0; j < rules.size(); j++){
            if (r != j && (rules[r].lhs == rules[j].lhs)){
                int preflen = common_prefix(rules[r], rules[j]);
                if (preflen > maxlen){
                    maxlen = preflen;
                }
            }
        }
        prefRules ruleInfo;
        ruleInfo.index = r;
        ruleInfo.pravilo = rules[r];
        ruleInfo.len = maxlen;
        for (int j = 0; j < rules.size(); j++){
            if (r != j && (rules[r].lhs == rules[j].lhs)){
                int preflen = common_prefix(rules[r], rules[j]);
                if (preflen == maxlen){
                    ruleInfo.ruleList.push_back(j);
                }
            }
        }
        commonRules.push_back(ruleInfo);
    }*/
    
    //sortlexique(commonRules);
    rules.clear();
    for (auto i: commonRules){
        rules.push_back(i.pravilo);
    }
    string dumpNT = "";
    
    do{
        for (auto i: iterNT){
            bool isRules = false;
            int length = 0;
            vector<string> prefix = {};
            vector<vector<string>> suffices = {};
            for(auto r: commonRules){
                if(r.len > 0 && r.pravilo.lhs == i){
                    isRules = true;
                    length = r.len;
                    prefix = extractPrefix(r.pravilo.rhs, r.len);
                    
                    cout << endl;
                    break;
                }
            }
            if(isRules){
                int j = 0;
                string newS = i + to_string(nt_counter[i]);
                nt_counter[i]++;
                prefRules addedRule;
                rule pravilo;
                pravilo.lhs = i;
                pravilo.rhs = prefix;
                pravilo.rhs.push_back(newS);
                addedRule.pravilo = pravilo;
                commonRules.push_back(addedRule);
                while(j < commonRules.size()){
                    vector<string> prefix2 = extractPrefix(commonRules[j].pravilo.rhs, commonRules[j].len);
                    vector<string> suffix = {};
                    if (prefix == prefix2){
                        suffix = extractAllButPrefix(commonRules[j].pravilo.rhs, commonRules[j].len);
                        commonRules.erase(commonRules.begin() + j);
                        rule newRule;
                        newRule.lhs = newS;
                        newRule.rhs = suffix;
                        nrules.push_back(newRule);
                    } else{
                        j++;
                    }
                }
                nNT.emplace(newS);
            }else{
                nNT.emplace(i);
                int k = 0;
                while(k < commonRules.size()){
                    if (commonRules[k].pravilo.lhs == i){
                        nrules.push_back(commonRules[k].pravilo);
                        commonRules.erase(commonRules.begin() + k);
                    } else {
                        k++;
                    }
                }
                NT.erase(i);
            }
            if (commonRules.size() > 0){
                sortlexique(commonRules);
            };
        }
        iterNT = NT;
    } while(NT.size() != 0);
    /*do{
        dumpNT.clear();
        for(auto r: NT){
            int ind = 0;
            bool flag = false;
            for (int i = 0; i < commonRules.size(); i++){
                if (commonRules[i].pravilo.lhs == r && commonRules[i].len > 0){
                    ind = i;
                    flag = true;
                    break;
                }
            }

            if(flag){
                vector<string> prefix = {};
                vector<vector<string>> postprefices = {};
                vector<int> usedRules = {};
                commonRules[ind].ruleList = {};
                for(int i = 0; i< commonRules.size(); i++){
                    if (ind != i && (commonRules[ind].pravilo.lhs == commonRules[i].pravilo.lhs)){
                        int preflen = common_prefix(commonRules[ind].pravilo, commonRules[i].pravilo);
                        if (preflen == commonRules[ind].len){
                            commonRules[ind].ruleList.push_back(i);
                        }
                    }
                }

                usedRules.push_back(ind);
                for(auto i: commonRules[ind].ruleList){
                    usedRules.push_back(i);
                }

                prefix = extractPrefix(commonRules[ind].pravilo.rhs, commonRules[ind].len);
                for(auto i: usedRules){
                    vector<string> postprefix = extractAllButPrefix(commonRules[i].pravilo.rhs, commonRules[i].len);
                    postprefices.push_back(postprefix);
                }

                string sNew = r + to_string(nt_counter[r]); //adding new rule A-> a Anew
                nt_counter[r]++;
                rule newRule;
                newRule.lhs = r;
                for (int k = 0; k < prefix.size(); k++){
                    newRule.rhs.push_back(prefix[k]);
                }
                newRule.rhs.push_back(sNew);
                prefRules newPrefRule;
                newPrefRule.pravilo = newRule;
                commonRules.push_back(newPrefRule);
                nNT.emplace(sNew);

                for (int k = 0; k < postprefices.size(); k++){
                    rule newRule2;
                    newRule2.lhs = sNew;
                    newRule2.rhs = postprefices[k];
                    nrules.push_back(newRule2);
                }

                int i = 0;
                while(i < usedRules.size()){
                    commonRules.erase(commonRules.begin()+usedRules[i]);
                    for(int j = 0; j < usedRules.size(); j++){
                        usedRules[j]--;
                    }
                    i++;
                }

            } else {
                int i = 0;
                while (i < commonRules.size()){
                    if(commonRules[i].pravilo.lhs == r){
                        nrules.push_back(commonRules[i].pravilo);
                        commonRules.erase(commonRules.begin() + i);
                    } else {
                        i++;
                    }
                }
                dumpNT.emplace(r);
                nNT.emplace(r);
            }
        }
        for(auto i: dumpNT){
            NT.erase(i);
        }
        for (int r = 0; r < commonRules.size(); r++){
            int maxlen = 0;
            for (int j = 0; j < commonRules.size(); j++){
                if (r != j && (rules[r].lhs == rules[j].lhs)){
                    int preflen = common_prefix(commonRules[r].pravilo, commonRules[j].pravilo);
                    if (preflen > maxlen){
                        maxlen = preflen;
                    }
                }
            }
            commonRules[r].len = maxlen;
            sortlexique(commonRules);
        }
    } while (NT.size() != 0);*/

    sortlexiqueFinal(nrules);
    for(auto i: nrules){
        cout << i.lhs << " -> ";
        for(auto j: i.rhs){
            cout << j << " ";
        }
        if (i.rhs.size() == 0 || i.rhs[i.rhs.size()-1] != "#"){
            cout << "#";
        }
        cout << endl;
    }
    cout << endl;
}
/*
void Task4()
{
    separate();
    for (auto r: NT){
        nt_counter[r] = 1;
    }

    vector<prefRules> commonRules = {};
    vector<rule> nrules = {};
    set<string> nTN = TN;
    set<string> nNT = {};
    vector<rule> rulest = rules;

    for (int r = 0; r < rules.size(); r++){
        int maxlen = 0;
        for (int j = 0; j < rules.size(); j++){
            if (r != j && (rules[r].lhs == rules[j].lhs)){
                int preflen = common_prefix(rules[r], rules[j]);
                if (preflen > maxlen){
                    maxlen = preflen;
                }
            }
        }
        prefRules ruleInfo;
        ruleInfo.index = r;
        ruleInfo.pravilo = rules[r];
        ruleInfo.len = maxlen;
        for (int j = 0; j < rules.size(); j++){
            if (r != j && (rules[r].lhs == rules[j].lhs)){
                int preflen = common_prefix(rules[r], rules[j]);
                if (preflen == maxlen){
                    ruleInfo.ruleList.push_back(j);
                }
            }
        }
        commonRules.push_back(ruleInfo);
    }
    
    sortlexique(commonRules);
    rules.clear();
    for (auto i: commonRules){
        rules.push_back(i.pravilo);
    }
    set<string> dumpNT = {};
    do{
        dumpNT.clear();
        for(auto r: NT){
            int ind = 0;
            bool flag = false;
            for (int i = 0; i < commonRules.size(); i++){
                if (commonRules[i].pravilo.lhs == r && commonRules[i].len > 0){
                    ind = i;
                    flag = true;
                    break;
                }
            }

            if(flag){
                vector<string> prefix = {};
                vector<vector<string>> postprefices = {};
                vector<int> usedRules = {};
                commonRules[ind].ruleList = {};
                for(int i = 0; i< commonRules.size(); i++){
                    if (ind != i && (commonRules[ind].pravilo.lhs == commonRules[i].pravilo.lhs)){
                        int preflen = common_prefix(commonRules[ind].pravilo, commonRules[i].pravilo);
                        if (preflen == commonRules[ind].len){
                            commonRules[ind].ruleList.push_back(i);
                        }
                    }
                }

                usedRules.push_back(ind);
                for(auto i: commonRules[ind].ruleList){
                    usedRules.push_back(i);
                }

                prefix = extractPrefix(commonRules[ind].pravilo.rhs, commonRules[ind].len);
                for(auto i: usedRules){
                    vector<string> postprefix = extractAllButPrefix(commonRules[i].pravilo.rhs, commonRules[i].len);
                    postprefices.push_back(postprefix);
                }

                string sNew = r + to_string(nt_counter[r]); //adding new rule A-> a Anew
                nt_counter[r]++;
                rule newRule;
                newRule.lhs = r;
                for (int k = 0; k < prefix.size(); k++){
                    newRule.rhs.push_back(prefix[k]);
                }
                newRule.rhs.push_back(sNew);
                prefRules newPrefRule;
                newPrefRule.pravilo = newRule;
                commonRules.push_back(newPrefRule);
                nNT.emplace(sNew);

                for (int k = 0; k < postprefices.size(); k++){
                    rule newRule2;
                    newRule2.lhs = sNew;
                    newRule2.rhs = postprefices[k];
                    nrules.push_back(newRule2);
                }

                int i = 0;
                while(i < usedRules.size()){
                    commonRules.erase(commonRules.begin()+usedRules[i]);
                    for(int j = 0; j < usedRules.size(); j++){
                        usedRules[j]--;
                    }
                    i++;
                }

            } else {
                int i = 0;
                while (i < commonRules.size()){
                    if(commonRules[i].pravilo.lhs == r){
                        nrules.push_back(commonRules[i].pravilo);
                        commonRules.erase(commonRules.begin() + i);
                    } else {
                        i++;
                    }
                }
                dumpNT.emplace(r);
                nNT.emplace(r);
            }
        }
        for(auto i: dumpNT){
            NT.erase(i);
        }
        for (int r = 0; r < commonRules.size(); r++){
            int maxlen = 0;
            for (int j = 0; j < commonRules.size(); j++){
                if (r != j && (rules[r].lhs == rules[j].lhs)){
                    int preflen = common_prefix(commonRules[r].pravilo, commonRules[j].pravilo);
                    if (preflen > maxlen){
                        maxlen = preflen;
                    }
                }
            }
            commonRules[r].len = maxlen;
            sortlexique(commonRules);
        }
    } while (NT.size() != 0);

    sortlexiqueFinal(nrules);
    for(auto i: nrules){
        cout << i.lhs << " -> ";
        for(auto j: i.rhs){
            cout << j << " ";
        }
        if (i.rhs.size() == 0 || i.rhs[i.rhs.size()-1] != "#"){
            cout << "#";
        }
        cout << endl;
    }
    cout << endl;
}
*/
// Task 5
void Task5()
{   
    separate();
    vector<rule> newrules = {};
    for (auto r: NT){
        nt_counter[r] = 1;
    }
    for(auto i: NT){
        NTrules[i] = {};
    }
    for(auto i: rules){
        NTrules[i.lhs].push_back(i);
        //NTrules[i.lhs].emplace(i);
    }
    vector<string>newNT;
    for(auto i: NT){
        newNT.push_back(i);
    }
     //sotring algo
     
    for(int i = 0; i < newNT.size() - 1; i++){
        for(int j = 0; j < newNT.size() - 1 - i; j++){
            if(newNT[j] > newNT[j+1]){
                string t = newNT[j];
                newNT[j] = newNT[j+1];
                newNT[j+1] = t;
            }
        }
    }
    
    //main algo
    for (int i = 0; i < newNT.size(); i++){
        string A = newNT[i];
        for (int j = 0; j < i ; j++){
            /*for (auto it = NTrules[A].begin(); it != NTrules[A].end(); ){
                if ((*it).rhs[0] == newNT[j]){
                    vector<string> temp = (*it).rhs;
                    NTrules[A].erase(it);
                        for (auto iter = NTrules[newNT[j]].begin(); iter != NTrules[newNT[j]].end(); ){
                            rule newRule;
                            newRule.lhs = A;
                            newRule.rhs = (*iter).rhs;
                            for (auto k: temp){
                                newRule.rhs.push_back(k);
                            }
                            NTrules[A].push_back(newRule);
                            //Trules[A].emplace(newRule);
                        }
                } else {
                    ++it;
                }
            }*/
            string B = newNT[j];
            int r = 0;
            while(r < NTrules[A].size()){
                rule checkrule = NTrules[A][r];
                if (checkrule.rhs[0] == newNT[j]){ //prohozhus po pravilam
                    vector<string> ruleRHS = NTrules[A][r].rhs;
                    ruleRHS.erase(ruleRHS.begin() + 0);
                    NTrules[newNT[i]].erase(NTrules[newNT[i]].begin() + r);
                    for(int k = 0; k < NTrules[newNT[j]].size(); k++){
                        rule newRule;
                        newRule.lhs = newNT[i];
                        newRule.rhs = NTrules[newNT[j]][k].rhs;
                        for (auto l: ruleRHS){
                            newRule.rhs.push_back(l);
                        }
                        NTrules[newNT[i]].push_back(newRule);
                    }
                } else {
                    r++;
                }
            }
        }
        int y;
        
        pair<vector<rule>, vector<rule>>g = sepRules(A);
        vector<rule> hasA = g.first;
        vector<rule> hasntA = g.second;
        for (auto temp: hasA){
            int j = 0;
            while (j < NTrules[A].size()){
                if(NTrules[A][j].rhs == temp.rhs){
                    NTrules[A].erase(NTrules[A].begin() + j);
                    j = NTrules[A].size()+1;
                    break;
                }
                j++;
            }
        }
        if(hasA.size() != 0){
            pair<vector<rule>, vector<rule>>t = delImmedRecur(hasA, hasntA);
            
            if (t.first.size() > 0){
            for (auto d: t.first){
                newrules.push_back(d);
            }}
            if(t.second.size() > 0){
            for (auto d: t.second){
                newrules.push_back(d);
            }}
        } else {
            if (hasntA.size() > 0){
            for (auto d: hasntA){
                newrules.push_back(d);
            }}
        }
        
    }

    sortlexiqueFinal(newrules);
    for(auto i: newrules){
        cout << i.lhs << " -> ";
        for(auto j: i.rhs){
            cout << j << " ";
        }
        if (i.rhs.size() == 0 || i.rhs[i.rhs.size()-1] != "#"){
            cout << "#";
        }
        cout << endl;
    }

    //std::cout << "5\n";
}

int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        std::cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);
    LexicalAnalyzer lexer;
    ReadGrammar(lexer);  // Reads the input grammar from standard input
                    // and represent it internally in data structures
                    // ad described in project 2 presentation file

    switch (task) {
        case 1: 
            Task1();
            break;

        case 2: 
            Task2();
            printFIRST();
            break;

        case 3: 
            Task3();
            printFOLLOW();
            break;

        case 4: Task4();
            //cout << " OK ";
            break;

        case 5: Task5();
            //cout << " OK " ;
            cout << endl;
            break;

        default:
            std::cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

