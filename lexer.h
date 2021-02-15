#pragma once
#include "token.h"
#include <vector>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;
const int N = 255;

class Lexer
{
    std::vector<Token> tokens;
    int tr[N][131];
    int cnt, tot;
    TokenKind f[N];
    ifstream in;
    int row, col;
    int root_alpha, root_number, root_sign;
    bool stop, readin;
    void add(int rt, string s, TokenKind kind);
    void spsolve_root(int rt, TokenKind flag);
    int build_alpha();
    int build_number();
    int build_sign();
public:
    Lexer(std::ifstream& in):in(std::move(in)) 
    {
        cnt = 0;
        col = 0;
        tot = 0;
        row = 0;
        stop = 0;
        readin = 1;
        memset(tr, 0, sizeof(tr));
        fill(std::begin(f), std::end(f), TokenKind::unknown);
        root_alpha = build_alpha();
        root_number = build_number();
        root_sign = build_sign();
    }
    std::vector<Token> Lex();
    int check_classify(char ch);
    void solve_alpha(int rt, char* s, int m, int l, int& len, TokenKind& flag);
    void solve_number(int rt, char* s, int m, int l, int& len, TokenKind& flag);
    void solve_sign(int rt, char* s, int m, int l, int& len, TokenKind& flag);
};
