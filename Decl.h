/** @file Decl.h
* @brief Decl��������
**/

#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Expr.h"
#include "ASTNode.h"

namespace AST
{
    class FunctionDecl;

    class Decl : public ASTNode
    {
    };

    /**
 * @brief �����﷨���ĸ��ڵ㣬��ʵ�ʺ���
*/
    class TranslationUnitDecl : public Decl
    {
        std::vector<std::unique_ptr<Decl>> decls;

    public:
        TranslationUnitDecl() = default;
        TranslationUnitDecl(std::vector<std::unique_ptr<Decl>> &rhsDecls) : decls(std::move(rhsDecls)) {}

        virtual json toJson() const override;

        // ͨ�� Decl �̳�
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    /**
 * @brief ��������
*/
    class VarDecl : public Decl
    {
        friend CodeGenerator;

    protected:
        friend class FunctionDecl;
        std::string name;
        std::string type;
        bool hasInit;
        std::unique_ptr<Expr> initValue;
        
    public:
        VarDecl() = default;
        VarDecl(const std::string &type, const std::string &name, bool hasInit = false,
                std::unique_ptr<Expr> initValue = nullptr) : type(type), name(name), hasInit(hasInit), initValue(std::move(initValue)){};
        const std::string getName() const 
        {
            return name;
        }
        virtual json toJson() const override;

        // ͨ�� Decl �̳�
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    class ParmVarDecl : public VarDecl
    {
    public:
        ParmVarDecl(const std::string &type, const std::string &name) : VarDecl(type, name) {}
        virtual json toJson() const override;
    };
}; // namespace AST
