/**
* @file Parser.cpp
* 语法分析器主体部分实现
**/

#include <cassert>
#include "Parser.h"

precLevel Parser::getBinOpPrecedence(TokenKind opcode)
{
    switch (opcode)
    {
        default:
            return precLevel::Unknown;

        case TokenKind::equal:
        case TokenKind::plusequal:
        case TokenKind::minusequal:
        case TokenKind::starequal:
        case TokenKind::slashequal:
        case TokenKind::percentequal:
        case TokenKind::lesslessequal:
        case TokenKind::greatergreaterequal:
        case TokenKind::ampequal:
        case TokenKind::pipeequal:
        case TokenKind::caretequal:
            return precLevel::Assignment;

        case TokenKind::pipepipe:
            return precLevel::LogicalOr;

        case TokenKind::ampamp:
            return precLevel::LogicalAnd;

        case TokenKind::pipe:
            return precLevel::InclusiveOr;

        case TokenKind::caret:
            return precLevel::ExclusiveOr;

        case TokenKind::amp:
            return precLevel::And;

        case TokenKind::exclaimequal:
        case TokenKind::equalequal:
            return precLevel::Equality;

        case TokenKind::less:
        case TokenKind::lessequal:
        case TokenKind::greater:
        case TokenKind::greaterequal:
            return precLevel::Relational;

        case TokenKind::lessless:
        case TokenKind::greatergreater:
            return precLevel::Shift;

        case TokenKind::plus:
        case TokenKind::minus:
            return precLevel::Additive;

        case TokenKind::slash:
        case TokenKind::star:
        case TokenKind::percent:
            return precLevel::Multiplicative;
    }
}

BinaryOperatorKind Parser::getBinOpKind(TokenKind opcode)
{
    switch (opcode)
    {
        case TokenKind::plus:
            return BinaryOperatorKind::BO_Add;
        case TokenKind::plusequal:
            return BinaryOperatorKind::BO_AddAssign;
        case TokenKind::minus:
            return BinaryOperatorKind::BO_Sub;
        case TokenKind::minusequal:
            return BinaryOperatorKind::BO_SubAssign;
        case TokenKind::exclaimequal:
            return BinaryOperatorKind::BO_NE;
        case TokenKind::equalequal:
            return BinaryOperatorKind::BO_EQ;
        case TokenKind::slash:
            return BinaryOperatorKind::BO_Div;
        case TokenKind::slashequal:
            return BinaryOperatorKind::BO_DivAssign;
        case TokenKind::percent:
            return BinaryOperatorKind::BO_Rem;
        case TokenKind::percentequal:
            return BinaryOperatorKind::BO_RemAssign;
        case TokenKind::star:
            return BinaryOperatorKind::BO_Mul;
        case TokenKind::starequal:
            return BinaryOperatorKind::BO_MulAssign;
        case TokenKind::less:
            return BinaryOperatorKind::BO_LT;
        case TokenKind::lessequal:
            return BinaryOperatorKind::BO_LE;
        case TokenKind::lessless:
            return BinaryOperatorKind::BO_Shl;
        case TokenKind::lesslessequal:
            return BinaryOperatorKind::BO_ShlAssign;
        case TokenKind::greater:
            return BinaryOperatorKind::BO_GT;
        case TokenKind::greaterequal:
            return BinaryOperatorKind::BO_GE;
        case TokenKind::greatergreater:
            return BinaryOperatorKind::BO_Shr;
        case TokenKind::greatergreaterequal:
            return BinaryOperatorKind::BO_ShrAssign;
        case TokenKind::equal:
            return BinaryOperatorKind::BO_Assign;
        case TokenKind::amp:
            return BinaryOperatorKind::BO_And;
        case TokenKind::ampamp:
            return BinaryOperatorKind::BO_LAnd;
        case TokenKind::ampequal:
            return BinaryOperatorKind::BO_AndAssign;
        case TokenKind::pipe:
            return BinaryOperatorKind::BO_Or;
        case TokenKind::pipepipe:
            return BinaryOperatorKind::BO_LOr;
        case TokenKind::pipeequal:
            return BinaryOperatorKind::BO_OrAssign;
        case TokenKind::caret:
            return BinaryOperatorKind::BO_Xor;
        case TokenKind::caretequal:
            return BinaryOperatorKind::BO_XorAssign;
        default:
            assert(0 && "Not a binary operator");
            return BinaryOperatorKind();
    }
}

UnaryOperatorKind Parser::getUnaryOpKind(TokenKind opcode)
{
    switch (opcode)
    {
        case TokenKind::plus:
            return UnaryOperatorKind::UO_Plus;
        case TokenKind::minus:
            return UnaryOperatorKind::UO_Minus;
        case TokenKind::tilde:
            return UnaryOperatorKind::UO_Not;
        case TokenKind::exclaim:
            return UnaryOperatorKind::UO_LNot;
        default:
            assert(0 && "Not a binary operator");
            return UnaryOperatorKind();
            break;
    }
}



unique_ptr<Expr> Parser::parseIntegerLiteral()
{
    auto res = make_unique<IntegerLiteral>(std::stoi(curTok->content));
    getNextTok();
    return std::move(res);
}


/// ParenExpr ::= '(' Expr ')'
unique_ptr<Expr> Parser::parseParenExpr()
{
    auto lparen = curTok;
    getNextTok(); //eat (
    auto res = parseExpression(); 
    if (!res)
    {
        return nullptr;
    }

    if (curTok->kind != TokenKind::r_paren)
    {
        logError("expected ')'");
        addNote("to math this '(", lparen->loc);
        return nullptr;
    }
    getNextTok(); //eat )
    return make_unique<ParenExpr>(std::move(res));
}

/// RefOrCall 
///     ::= DeclRefExpr
///     ::= DeclRefExpr '(' Paras ')'
/// 
/// Paras
///     ::= Expr
///     ::= Expr ',' Paras
unique_ptr<Expr> Parser::parseRefOrCall()
{
    const std::string& name=curTok->content;
    getNextTok();
    if (curTok->kind == TokenKind::l_paren)
    {
        auto lparen = curTok;
        getNextTok();
        std::vector<unique_ptr<Expr>> paras;
        while (true)
        {
            paras.emplace_back(parseRValue());
            if (curTok->kind == TokenKind::r_paren)
            {
                getNextTok();
                break;
            }
            else if (curTok->kind == TokenKind::comma)
            {
                getNextTok();
            }
            else
            {
                logError("expected ')'");
                addNote("to math this '(", lparen->loc);
                return nullptr;
            }
        }
        return make_unique<CallExpr>(make_unique<DeclRefExpr>(name), std::move(paras));
    }
    else
    {
        return make_unique<DeclRefExpr>(name);
    }
}

/// Primary
///     ::= RefOrCall
///     ::= IntegerLiteral
///     ::= ParenExpr
unique_ptr<Expr> Parser::parsePrimary()
{
    switch (curTok->kind)
    {
        case TokenKind::identifier:
            return parseRefOrCall();
        case TokenKind::numeric_constant:
            return parseIntegerLiteral(); //目前只支持整型
        case TokenKind::l_paren:
            return parseParenExpr();
        default:
            logError("expected expression");
            return nullptr;
    }
}

unique_ptr<Expr> Parser::parseRHSOfBinaryExpression(unique_ptr<Expr> LHS, precLevel minPrec)
{
    while (1)
    {
        precLevel nextTokPrec = getBinOpPrecedence(curTok->kind);
        if (nextTokPrec < minPrec)
        {
            return LHS;
        }
        auto binOP = getBinOpKind(curTok->kind);
        getNextTok();
        auto RHS = parsePrimary();
        if (!RHS)
        {
            return nullptr;
        }
        precLevel thisPrec = nextTokPrec;
        nextTokPrec = getBinOpPrecedence(curTok->kind);
        bool isRightAssoc = thisPrec == precLevel::Assignment; 

        if (thisPrec < nextTokPrec ||
            (thisPrec == nextTokPrec && isRightAssoc))
        {
            RHS = parseRHSOfBinaryExpression(std::move(RHS),
                static_cast<precLevel>(static_cast<int>(thisPrec) + !isRightAssoc));
        }
        LHS = make_unique<AST::BinaryOperator>(binOP, std::move(LHS), std::move(RHS));
    }
    return nullptr;
}

unique_ptr<Expr> Parser::parseExpression()
{
    return parseBinaryOperator();
}

unique_ptr<Expr> Parser::parseRValue()
{
    auto expr = parseExpression();
    if (expr->getIsLvalue())
    {
        expr = make_unique<ImplicitCastExpr>(std::move(expr), "LValueToRValue");
    }
    return expr;
}

unique_ptr<Expr> Parser::parseUnaryOperator()
{
    switch (curTok->kind)
    {
        case TokenKind::exclaim:
        case TokenKind::tilde:
        case TokenKind::plus:
        case TokenKind::minus:
        case TokenKind::plusplus:
        case TokenKind::minusminus:
        {
            auto op = curTok->kind == TokenKind::plusplus ? UnaryOperatorKind::UO_PreInc :
                      curTok->kind == TokenKind::minusminus ? UnaryOperatorKind::UO_PreDec :
                            getUnaryOpKind(curTok->kind);
            getNextTok();
            unique_ptr<Expr> body = parseUnaryOperator();
            return make_unique<AST::UnaryOperator>(op, std::move(body));
        }
        default:
            auto body=parsePrimary();
            if (curTok->kind == TokenKind::plusplus)
            {
                getNextTok();
                return make_unique<AST::UnaryOperator>(UnaryOperatorKind::UO_PostInc, std::move(body));
            }
            else if (curTok->kind == TokenKind::minusminus)
            {
                getNextTok();
                return make_unique<AST::UnaryOperator>(UnaryOperatorKind::UO_PostDec, std::move(body));
            }
            else
            {
                return std::move(body);
            }
    }
}

unique_ptr<Expr> Parser::parseBinaryOperator()
{
    auto LHS = parseUnaryOperator();
    if (!LHS)
    {
        return nullptr;
    }
    return parseRHSOfBinaryExpression(std::move(LHS), precLevel::Comma);
}

unique_ptr<Stmt> Parser::parseStmt()
{
    switch (curTok->kind)
    {
        case TokenKind::semi:
            return parseNullStmt();
        case TokenKind::l_brace:
            return parseCompoundStmt();
        case TokenKind::numeric_constant:
        case TokenKind::l_paren:
        case TokenKind::identifier:
            return parseValueStmt();
        case TokenKind::kw_if:
            return parseIfStmt();
        case TokenKind::kw_while:
            return parseWhileStmt();
        case TokenKind::kw_return:
            return parseReturnStmt();
        case TokenKind::kw_void:
        case TokenKind::kw_int:
            return parseDeclStmt();
        default:
            logError("excepted statement");
            return nullptr;
    }
}

// NullStmt ::= ';'
unique_ptr<Stmt> Parser::parseNullStmt()
{
    getNextTok(); // eat ';'
    return make_unique<NullStmt>();
}


// ValueStmt ::= Expr ';'
unique_ptr<Stmt> Parser::parseValueStmt()
{
    auto expr = parseExpression();
    if (curTok->kind != TokenKind::semi)
    {
        logError("except ; after expression");
        return nullptr;
    }
    getNextTok(); // eat ';'
    return make_unique<ValueStmt>(std::move(expr));
}

/// IfStmt ::= 'if' '(' Expr ')' Stmt
///        ::= 'if' '(' Expr ')' Stmt 'else' Stmt
unique_ptr<Stmt> Parser::parseIfStmt()
{
    getNextTok();//eat if
    if (curTok->kind != TokenKind::l_paren)
    {
        logError("except ; after if");
        return nullptr;
    }
    auto lparen = curTok;
    getNextTok(); // eat '('
    auto cond = parseRValue();
    if (!cond)
    {
        return nullptr;
    }
    if (curTok->kind != TokenKind::r_paren)
    {
        logError("except ')'");
        addNote("to match this '('", lparen->loc);
        return nullptr;
    }
    getNextTok(); // eat ')'
    auto body = parseStmt();
    if (!body)
    {
        return nullptr;
    }

    if (curTok->kind == TokenKind::kw_else)
    {
        getNextTok();
        auto elseBody = parseStmt();
        return make_unique<IfStmt>(std::move(cond), std::move(body), std::move(elseBody));
    }
    return make_unique<IfStmt>(std::move(cond), std::move(body));
}

/// WhileStmt ::= 'while' '(' Expr ')' Stmt
unique_ptr<Stmt> Parser::parseWhileStmt()
{
    getNextTok();//eat while
    if (curTok->kind != TokenKind::l_paren)
    {
        logError("except ; after while");
        return nullptr;
    }
    auto lparen = curTok;
    getNextTok();//eat (
    auto cond = parseRValue();
    if (!cond)
    {
        return nullptr;
    }
    if (curTok->kind != TokenKind::r_paren)
    {
        logError("except ')'");
        addNote("to match this '('", lparen->loc);
        return nullptr;
    }
    getNextTok();//eat )
    auto body = parseStmt();
    if (!body)
    {
        return nullptr;
    }
    return make_unique<WhileStmt>(std::move(cond), std::move(body));
}

unique_ptr<Stmt> Parser::parseCompoundStmt()
{
    assert(curTok->kind == TokenKind::l_brace && "Compound Statement should start with '{'");
    auto lbrace = curTok;
    getNextTok();
    std::vector<unique_ptr<Stmt>> body;
    while (curTok->kind != TokenKind::r_brace)
    {
        auto res = parseStmt();
        if (!res)
        {
            break;
        }
        body.push_back(std::move(res));
    }
    if (curTok->kind != TokenKind::r_brace)
    {
        logError("except '}'");
        addNote("to match this '{'", lbrace->loc);
        return nullptr;
    }
    getNextTok();
    return make_unique<CompoundStmt>(body);
}

unique_ptr<Stmt> Parser::parseDeclStmt()
{
    std::string type = curTok->content;
    getNextTok();

    std::vector<unique_ptr<Decl>> decls;

    do {
        std::string name = curTok->content;
        getNextTok();
        if (curTok->kind == TokenKind::l_paren)
        {
            auto lparen = curTok;
            //function Decl;
            getNextTok();
            std::vector<unique_ptr<ParmVarDecl>> paras;
            while (curTok->kind != TokenKind::r_paren)
            {
                //目前参数类型只能是int
                if (curTok->kind != TokenKind::kw_int && curTok->kind != TokenKind::kw_void)
                {
                    logError(("unknown type name '" + curTok->content + "'").c_str());
                    return nullptr;
                }
                std::string paraType = curTok->content, paraName;
                getNextTok();
                if (curTok->kind == TokenKind::identifier)
                {
                    paraName = curTok->content;
                    getNextTok();
                }
                if (curTok->kind != TokenKind::comma && curTok->kind != TokenKind::r_paren)
                {
                    logError("except ')'");
                    addNote("to match this '('", lparen->loc);
                    return nullptr;
                }
                if (curTok->kind == TokenKind::comma)
                {
                    getNextTok();
                }
                paras.push_back(make_unique<ParmVarDecl>(paraType, paraName));
            }
            getNextTok();
            unique_ptr<Stmt> body = nullptr;
            if (curTok->kind == TokenKind::l_brace)
            {
                body = parseCompoundStmt();
            }
            else if (curTok->kind == TokenKind::comma)
            {
                getNextTok();
            }
            else if (curTok->kind != TokenKind::semi)
            {
                logError("expected ';' at end of declaration");
                return nullptr;
            }
            decls.push_back(make_unique<FunctionDecl>(type, name, paras, std::move(body)));
        }
        else if (curTok->kind == TokenKind::equal)
        {
            getNextTok();
            decls.push_back(make_unique<VarDecl>(type, name, true, parseRValue()));
            if (curTok->kind == TokenKind::comma)
            {
                getNextTok();
            }
            else if (curTok->kind != TokenKind::semi)
            {
                logError("expected ';' at end of declaration");
                return nullptr;
            }
        }
        else if (curTok->kind == TokenKind::comma)
        {
            decls.push_back(make_unique<VarDecl>(type, name, false));
            getNextTok();
        }
        else if (curTok->kind == TokenKind::semi)
        {
            decls.push_back(make_unique<VarDecl>(type, name, false));
            break;
        }
        else
        {
            logError("expected ';' at end of declaration");
            return nullptr;
        }
    } while (curTok->kind != TokenKind::semi);
    getNextTok();
    return make_unique<DeclStmt>(decls);
}

unique_ptr<Stmt> Parser::parseReturnStmt()
{
    assert(curTok->kind == TokenKind::kw_return && "Return Statement should be start with 'return'");
    getNextTok(); //eat return
    if (curTok->kind != TokenKind::semi)
    {
        auto expr = parseRValue();
        if (!expr)
        {
            return nullptr;
        }
        if (curTok->kind != TokenKind::semi)
        {
            logError("expected ';' after return statement");
            return nullptr;
        }
        getNextTok();
        return make_unique<ReturnStmt>(std::move(expr));
    }
    getNextTok(); //eat semi
    return make_unique<ReturnStmt>();
}

/// @brief 解析全局变量或函数的定义
/// @return 
/// 暂时不支持一个语句声明多个全局变量
unique_ptr<Decl> Parser::parseTopLevelDecl()
{
    //curTok is kw_int or kw_void
    assert(curTok->kind == TokenKind::kw_int || curTok->kind == TokenKind::kw_void || "Expected type name");
    std::string type = curTok->content;
    getNextTok();
    if (curTok->kind != TokenKind::identifier)
    {
        logError("expected identifier");
        return nullptr;
    }
    std::string name = curTok->content;
    getNextTok();

    if (curTok->kind == TokenKind::l_paren)
    {
        auto lparen = curTok;
        //function Decl;
        getNextTok();
        std::vector<unique_ptr<ParmVarDecl>> paras;
        while (curTok->kind != TokenKind::r_paren)
        {
            //目前参数类型只能是int或者void
            if (curTok->kind != TokenKind::kw_int && curTok->kind != TokenKind::kw_void)
            {
                logError(("unknown type name '" + curTok->content + "'").c_str());
                return nullptr;
            }
            std::string paraType = curTok->content, paraName;
            getNextTok();
            if (curTok->kind == TokenKind::identifier)
            {
                paraName = curTok->content;
                getNextTok();
            }
            if (curTok->kind != TokenKind::comma && curTok->kind != TokenKind::r_paren)
            {
                logError("except ')'");
                addNote("to match this '('", lparen->loc);
                return nullptr;
            }
            if (curTok->kind == TokenKind::comma)
            {
                getNextTok();
            }
            paras.push_back(make_unique<ParmVarDecl>(paraType, paraName));
        }
        getNextTok();
        unique_ptr<Stmt> body = nullptr;
        if (curTok->kind == TokenKind::l_brace)
        {
            body = parseCompoundStmt();
        }
        else if (curTok->kind == TokenKind::semi)
        {
            getNextTok();
        }
        else
        {
            logError("expected ';' after top level declarator");
            return nullptr;
        }
        return make_unique<FunctionDecl>(type, name, paras, std::move(body));
    }
    else if (curTok->kind == TokenKind::equal)
    {
        getNextTok();
        auto value = parseBinaryOperator();
        if (!value || !value->getIsConst())
        {
            return nullptr;
        }
        if (curTok->kind != TokenKind::semi)
        {
            return nullptr;
        }
        getNextTok();
        if (value->getIsLvalue())
        {
            value = make_unique<ImplicitCastExpr>(std::move(value), "LValueToRValue");
        }
        return make_unique<VarDecl>(type, name, true, std::move(value));
    }
    else if (curTok->kind == TokenKind::semi)
    {
        getNextTok();
        return make_unique<VarDecl>(type, name);
    }
    else
    {
        logError("expected ';' after top level declarator");
        return nullptr;
    }
}

unique_ptr<Decl> Parser::parse()
{
    curTok = tokens.begin();
    std::vector<unique_ptr<Decl>> decls;
    while (curTok!=tokens.end())
    {
        switch (curTok->kind)
        {
            case TokenKind::eof:
                return make_unique<TranslationUnitDecl>(decls);
            case TokenKind::kw_int:
            case TokenKind::kw_void:
                decls.push_back(parseTopLevelDecl());
                break;
            default:
                logError("Excepted declaration");
                return nullptr;
        }
    }
    return make_unique<TranslationUnitDecl>(decls);
}


