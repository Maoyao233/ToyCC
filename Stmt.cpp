/**
* @file Stmt.cpp
* 描述Stmt及其子类转换为Json格式的方式
**/
#include "Stmt.h"

using namespace AST;

json IfStmt::toJson() const
{
    json res = {
        {"kind", "IfStmt"},
        {"hasElse", !elseBody}
    };
    auto inner = json::array({ cond->toJson() , body->toJson() });
    if (elseBody)
    {
        inner.emplace_back(elseBody->toJson());
    }
    res["inner"] = std::move(inner);;
    return res;
}

Value* AST::IfStmt::genCode(CodeGeneratorBase& generator) const
{
    return generator.gen(*this);
}

json DeclStmt::toJson() const
{
    json res = {
        {"kind", "DeclStmt"}
    };
    if (!decls.empty())
    {
        auto inner = json::array();
        for (const auto& u : decls)
        {
            inner.emplace_back(u->toJson());
        }
        res["inner"] = std::move(inner);
    }
    return res;
}

Value* AST::DeclStmt::genCode(CodeGeneratorBase& generator) const
{
    for (const auto& decl : decls)
    {
        decl->genCode(generator);
    }
    return nullptr;
}

json CompoundStmt::toJson() const
{
    json res = {
        {"kind", "CompoundStmt"},
    };
    if (!body.empty())
    {
        auto inner = json::array();
        for (const auto& u : body)
        {
            inner.emplace_back(u->toJson());
        }
        res["inner"] = std::move(inner);
    }
    return res;
}

Value* AST::CompoundStmt::genCode(CodeGeneratorBase& generator) const
{
    return generator.gen(*this);
}


json FunctionDecl::toJson() const
{
    std::string type = returnType + "(";
    for (auto it = paras.begin(); it != paras.end(); ++it)
    {
        if (it != paras.begin())
        {
            type += ", ";
        }
        type += (*it)->type;
    }
    type += ")";
    json res = {
        {"kind", "FunctionDecl"},
        {"name", name},
        {"type", type}
    };
    if (body || !paras.empty())
    {
        auto inner = json::array();
        if (!paras.empty())
        {
            for (const auto& u : paras)
            {
                inner.emplace_back(u->toJson());
            }
        }
        if (body)
        {
            inner.emplace_back(body->toJson());
        }
        res["inner"] = std::move(inner);
    }
    return res;
}

Function* AST::FunctionDecl::genFunc(CodeGeneratorBase& generator) const
{
    return generator.gen(*this);
}

Value* AST::FunctionDecl::genCode(CodeGeneratorBase& generator) const
{
    generator.gen(*this);
    return nullptr;
}


json ReturnStmt::toJson() const
{
    json res = {
        {"kind", "ReturnStmt"}
    };
    if (returnValue)
    {
        res["inner"] = json::array({ returnValue->toJson() });
    }
    return res;
}

Value* AST::ReturnStmt::genCode(CodeGeneratorBase& generator) const
{
    return generator.gen(*this);
}


json ValueStmt::toJson() const
{
    return {
        {"kind", "ValueStmt"},
        {"inner", json::array({expr->toJson()})}
    };
}

Value* AST::ValueStmt::genCode(CodeGeneratorBase& generator) const
{
    return expr->genCode(generator);
}


json WhileStmt::toJson() const
{
    return {
        {"kind", "WhileStmt"},
        {"inner", json::array({cond->toJson(), body->toJson()})}
    };
}

Value* AST::WhileStmt::genCode(CodeGeneratorBase& generator) const
{
    return generator.gen(*this);
}

json NullStmt::toJson() const
{
    return { 
        { "kind", "NullStmt" }
    };
}

Value* AST::NullStmt::genCode(CodeGeneratorBase& generator) const
{
    return nullptr;
}
