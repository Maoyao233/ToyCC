/** 
* @file Decl.cpp
* ����Decl��������ת��ΪJson��ʽ�ķ�ʽ
**/

#include "Decl.h"
#include <iostream>

using namespace AST;

json TranslationUnitDecl::toJson() const
{
    json res = {
        {"kind","TranslationUnitDecl"}
    };
    if (!decls.empty())
    {
        res["inner"] = json::array();
        for (const auto& u : decls)
        {
            res["inner"].emplace_back(u->toJson());
        }
    }
    return res;
}

Value* AST::TranslationUnitDecl::genCode(CodeGeneratorBase& generator) const
{
    for (const auto& decl : decls)
    {
        decl->genCode(generator);
    }
    return nullptr;
}

json VarDecl::toJson() const
{
    json res={
        {"kind", "VarDecl"},
        {"name", name},
        {"type", type},
        {"hasInit", hasInit}
    };
    if (hasInit)
    {
        res["inner"] = json::array({ initValue->toJson() });
    }
    return res;
}

Value* AST::VarDecl::genCode(CodeGeneratorBase& generator) const
{
    return generator.gen(*this);
}

json ParmVarDecl::toJson() const
{
    json res = {
        {"kind", "ParmVarDecl"},
        {"name", name},
        {"type", type},
        {"hasDefault", hasInit}
    };
    if (hasInit)
    {
        res["inner"] = json::array({ initValue->toJson() });
    }
    return res;
}
