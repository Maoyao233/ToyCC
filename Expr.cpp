/**
* @file Expr.cpp
* 描述Stmt及其子类转换为Json格式的方式
**/

#include "Expr.h"
#include <string>

using namespace AST;
/**
 * @brief 取得双目运算符对应的字符串
 * @param op 该运算符的枚举类型
 * @return std::string
*/
static std::string getBinOpcode(BinaryOperatorKind op)
{
    switch (op)
    {
        case BinaryOperatorKind::BO_Mul:
            return "*";
        case BinaryOperatorKind::BO_Div:
            return "/";
        case BinaryOperatorKind::BO_Rem:
            return "%";
        case BinaryOperatorKind::BO_Add:
            return "+";
        case BinaryOperatorKind::BO_Sub:
            return "-";
        case BinaryOperatorKind::BO_Shl:
            return "<<";
        case BinaryOperatorKind::BO_Shr:
            return ">>";
        case BinaryOperatorKind::BO_LT:
            return "<";
        case BinaryOperatorKind::BO_GT:
            return ">";
        case BinaryOperatorKind::BO_LE:
            return "<=";
        case BinaryOperatorKind::BO_GE:
            return ">=";
        case BinaryOperatorKind::BO_EQ:
            return "==";
        case BinaryOperatorKind::BO_NE:
            return "!=";
        case BinaryOperatorKind::BO_And:
            return "&";
        case BinaryOperatorKind::BO_Xor:
            return "^";
        case BinaryOperatorKind::BO_Or:
            return "|";
        case BinaryOperatorKind::BO_LAnd:
            return "&&";
        case BinaryOperatorKind::BO_LOr:
            return "||";
        case BinaryOperatorKind::BO_Assign:
            return "=";
        case BinaryOperatorKind::BO_MulAssign:
            return "*=";
        case BinaryOperatorKind::BO_DivAssign:
            return "/=";
        case BinaryOperatorKind::BO_RemAssign:
            return "%=";
        case BinaryOperatorKind::BO_AddAssign:
            return "+=";
        case BinaryOperatorKind::BO_SubAssign:
            return "-=";
        case BinaryOperatorKind::BO_ShlAssign:
            return "<<=";
        case BinaryOperatorKind::BO_ShrAssign:
            return ">>=";
        case BinaryOperatorKind::BO_AndAssign:
            return "&=";
        case BinaryOperatorKind::BO_XorAssign:
            return "^=";
        case BinaryOperatorKind::BO_OrAssign:
            return "|=";
        default:
            return "";
    }
}

static std::string getUnaryOpcode(UnaryOperatorKind op)
{
    switch (op)
    {
        case AST::UnaryOperatorKind::UO_PostInc:
        case AST::UnaryOperatorKind::UO_PreInc:
            return "++";
        case AST::UnaryOperatorKind::UO_PostDec:
        case AST::UnaryOperatorKind::UO_PreDec:
            return "--";
        case AST::UnaryOperatorKind::UO_Plus:
            return "+";
        case AST::UnaryOperatorKind::UO_Minus:
            return "-";
        case AST::UnaryOperatorKind::UO_Not:
            return "~";
        case AST::UnaryOperatorKind::UO_LNot:
            return "!";
        default:
            return "";
    }
}

AST::BinaryOperator::BinaryOperator(BinaryOperatorKind op, unique_ptr<Expr> LHS, unique_ptr<Expr> RHS) : op(op)
{
    if (!isAssignment() && LHS->getIsLvalue())
    {
        LHS = std::make_unique<ImplicitCastExpr>(std::move(LHS), "LValueToRValue");
    }
    if (RHS->getIsLvalue())
    {
        RHS = std::make_unique<ImplicitCastExpr>(std::move(RHS), "LValueToRValue");
    }
    this->LHS = std::move(LHS);
    this->RHS = std::move(RHS);
}

bool AST::BinaryOperator::isAssignment() const
{
    switch (op)
    {
        case AST::BinaryOperatorKind::BO_Assign:
        case AST::BinaryOperatorKind::BO_MulAssign:
        case AST::BinaryOperatorKind::BO_DivAssign:
        case AST::BinaryOperatorKind::BO_RemAssign:
        case AST::BinaryOperatorKind::BO_AddAssign:
        case AST::BinaryOperatorKind::BO_SubAssign:
        case AST::BinaryOperatorKind::BO_ShlAssign:
        case AST::BinaryOperatorKind::BO_ShrAssign:
        case AST::BinaryOperatorKind::BO_AndAssign:
        case AST::BinaryOperatorKind::BO_XorAssign:
        case AST::BinaryOperatorKind::BO_OrAssign:
            return true;
        default:
            return false;
    }  
}

json AST::BinaryOperator::toJson() const
{
    json res = {
        {"kind", "BinaryOperator"},
        {"opcode", getBinOpcode(op)}
    };
    res["inner"] = json::array({ LHS->toJson(), RHS->toJson() });
    return res;
}

Value* AST::BinaryOperator::genCode(CodeGeneratorBase& generator) const
{
    return generator.gen(*this);
}

json IntegerLiteral::toJson() const
{
    return {
        {"kind", "IntegerLiteral"},
        {"type", "int"},
        {"value", std::to_string(value)}
    };
}

Value* AST::IntegerLiteral::genCode(CodeGeneratorBase& generator) const
{
    return generator.gen(*this);
}

json DeclRefExpr::toJson() const
{
    return {
        {"kind", "DeclRefExpr"},
        {"name", name}
    };
}

Value* AST::DeclRefExpr::genCode(CodeGeneratorBase& generator) const
{
    return generator.gen(*this);
}

Function* AST::DeclRefExpr::genFunc(CodeGeneratorBase& generator) const
{
    return nullptr;
}

json ParenExpr::toJson() const
{
    return {
        {"kind", "ParenExpr"},
        {"inner", json::array({subExpr->toJson()})}
    };
}

Value* AST::ParenExpr::genCode(CodeGeneratorBase& generator) const
{
    return subExpr->genCode(generator);
}

json CallExpr::toJson() const
{
    json res = {
        {"kind", "CallExpr"},
        {"inner", json::array({function->toJson()})}
    };
    for (const auto& u : paras)
    {
        res["inner"].emplace_back(u->toJson());
    }
    return res;
}

Value* AST::CallExpr::genCode(CodeGeneratorBase& generator) const
{
    return generator.gen(*this);
}

json AST::UnaryOperator::toJson() const
{
    json res = {
       {"kind", "UnaryOperator"},
       {"opcode", getUnaryOpcode(op)}
    };
    res["inner"] = json::array({ body->toJson()});
    return res;
}

Value* AST::UnaryOperator::genCode(CodeGeneratorBase& generator) const
{
    return generator.gen(*this);
}

json AST::ImplicitCastExpr::toJson() const
{
    json res = {
       {"kind", "ImplicitCastExpr"},
       {"valueCategory", "rvalue"},
       {"castKind", castKind}
    };
    res["inner"] = json::array({ subExpr->toJson() });
    return res;
}

Value* AST::ImplicitCastExpr::genCode(CodeGeneratorBase& generator) const
{
    return generator.gen(*this);
}
