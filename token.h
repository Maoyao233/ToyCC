#pragma once
#include <string>

enum class TokenKind : unsigned short
{
    //从clang学习的技巧，使用宏定义让程序便于修改
#define TOK(X) X,
#include "TokenKinds.def"
    NUM_TOKENS
};

struct Location
{
    int row;
    int col;
};

struct Token
{
    TokenKind kind;
    std::string content;
    Location loc;

    std::string getKindName() const;
};