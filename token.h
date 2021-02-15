#pragma once
#include <string>

enum class TokenKind : unsigned short
{
    //��clangѧϰ�ļ��ɣ�ʹ�ú궨���ó�������޸�
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