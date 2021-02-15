#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "Expr.h"
#include "token.h"
#include "Decl.h"
#include "Stmt.h"

using namespace AST;

using std::make_unique;
using std::unique_ptr;


/**
 * @brief 运算符优先级，遵循C/C++规则
*/
enum class precLevel {
    Unknown = 0,    // Not binary operator.
    Comma = 1,    // ,
    Assignment = 2,    // =, *=, /=, %=, +=, -=, <<=, >>=, &=, ^=, |=
    //Conditional     = 3,    // ?
    LogicalOr = 4,    // ||
    LogicalAnd = 5,    // &&
    InclusiveOr = 6,    // |
    ExclusiveOr = 7,    // ^
    And = 8,    // &
    Equality = 9,    // ==, !=
    Relational = 10,   //  >=, <=, >, <
    //Spaceship       = 11,   // <=>
    Shift = 12,   // <<, >>
    Additive = 13,   // -, +
    Multiplicative = 14,   // *, /, %
    //PointerToMember = 15    // .*, ->*
};

class Parser
{
    std::string sourceFileName;
    std::map<BinaryOperatorKind, int> binopPrecedence;
    std::vector<Token> tokens;
    std::vector<Token>::iterator curTok;
    std::vector<Token>::iterator getNextTok()
    {
        //std::clog << "curToken:" << curTok->getKindName() << " " << curTok->content << std::endl;
        return ++curTok;
    }

    void logError(const char* prompt)
    {
        std::cerr << sourceFileName << ":" << curTok->loc.row << ":" << curTok->loc.col << ": error: " << prompt << std::endl;
    }

    void addNote(const char* prompt, Location loc)
    {
        std::cerr << sourceFileName << ":" << loc.row << ":" << loc.col << ": error: " << prompt << std::endl;
    }

    precLevel getBinOpPrecedence(TokenKind opcode);

    BinaryOperatorKind getBinOpKind(TokenKind opcode);

    UnaryOperatorKind getUnaryOpKind(TokenKind opcode);

    unique_ptr<Expr> parseIntegerLiteral();

    unique_ptr<Expr> parseParenExpr();

    unique_ptr<Expr> parseRefOrCall();

    unique_ptr<Expr> parsePrimary();

    unique_ptr<Expr> parseRHSOfBinaryExpression(unique_ptr<Expr> LHS, precLevel minPrec);

    unique_ptr<Expr> parseExpression();

    unique_ptr<Expr> parseRValue();

    unique_ptr<Expr> parseUnaryOperator();

    unique_ptr<Expr> parseBinaryOperator();

    unique_ptr<Stmt> parseStmt();

    unique_ptr<Stmt> parseNullStmt();

    unique_ptr<Stmt> parseValueStmt();

    unique_ptr<Stmt> parseIfStmt();

    unique_ptr<Stmt> parseWhileStmt();

    unique_ptr<Stmt> parseCompoundStmt();

    unique_ptr<Stmt> parseDeclStmt();

    unique_ptr<Stmt> parseReturnStmt();

    unique_ptr<Decl> parseTopLevelDecl();

public:
    Parser(const std::vector<Token>& tokens, const std::string& sourceFile) :tokens(tokens), sourceFileName(sourceFile) {}
    unique_ptr<Decl> parse();
};