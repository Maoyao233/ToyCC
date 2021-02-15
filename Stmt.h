/** @file Stmt.h
* @brief Stmt及其子类
**/

#pragma once
#include <vector>
#include <memory>
#include "Expr.h"
#include "Decl.h"

namespace AST
{

    /**
    * @brief 所有Stmt类型结点的基类，表示语句
    */
    class Stmt : public ASTNode
    {
        
    };

    class NullStmt : public Stmt
    {
    public:
        // 通过 Stmt 继承
        virtual json toJson() const override;        
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    class ValueStmt : public Stmt
    {
        std::unique_ptr<Expr> expr;

    public:
        ValueStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

        // 通过 Stmt 继承
        virtual json toJson() const override;

        // 通过 Stmt 继承
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    class IfStmt : public Stmt
    {
        std::unique_ptr<Expr> cond;
        std::unique_ptr<Stmt> body;
        std::unique_ptr<Stmt> elseBody;
        friend CodeGenerator;

    public:
        IfStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> body, std::unique_ptr<Stmt> elseBody = nullptr) : cond(std::move(cond)), body(std::move(body)), elseBody(std::move(elseBody)) {}

        // 通过 Stmt 继承
        // virtual void printToJson(int depth) const override;

        // 通过 Stmt 继承
        virtual json toJson() const override;

        // 通过 Stmt 继承
        virtual Value* genCode(CodeGeneratorBase& generator) const override;

        virtual Value* genFunc(CodeGenerator& generator) const = delete;
    };

    class WhileStmt : public Stmt
    {
        std::unique_ptr<Expr> cond;
        std::unique_ptr<Stmt> body;

        friend CodeGenerator;
    public:
        WhileStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> body) : cond(std::move(cond)), body(std::move(body)) {}

        // 通过 Stmt 继承
        virtual json toJson() const override;


        // 通过 Stmt 继承
        virtual Value* genCode(CodeGeneratorBase& generator) const override;

        // 通过 Stmt 继承
        //virtual void printToJson(int depth) const override;
    };

    class DeclStmt : public Stmt
    {
        std::vector<std::unique_ptr<Decl>> decls;

    public:
        DeclStmt() = default;
        DeclStmt(std::vector<std::unique_ptr<Decl>> &decls) : decls(std::move(decls)) {}

        // 通过 Stmt 继承
        //virtual void printToJson(int depth) const override;

        // 通过 Stmt 继承
        virtual json toJson() const override;

        // 通过 Stmt 继承
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    class CompoundStmt : public Stmt
    {
    public:
        std::vector<std::unique_ptr<Stmt>> body;
        CompoundStmt(std::vector<std::unique_ptr<Stmt>> &body) : body(std::move(body)) {}
        // 通过 Stmt 继承
        // virtual void printToJson(int depth) const override;

        // 通过 Stmt 继承
        virtual json toJson() const override;

        // 通过 Stmt 继承
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    class ReturnStmt : public Stmt
    {
        std::unique_ptr<Expr> returnValue;

        friend CodeGenerator;

    public:
        ReturnStmt(std::unique_ptr<Expr> value = nullptr) : returnValue(std::move(value)) {}

        // 通过 Stmt 继承
        // virtual void printToJson(int depth) const override;

        // 通过 Stmt 继承
        virtual json toJson() const override;

        // 通过 Stmt 继承
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    class FunctionDecl : public Decl
    {
        std::string returnType;
        std::string name;
        std::vector<std::unique_ptr<ParmVarDecl>> paras;
        std::unique_ptr<Stmt> body;

        friend CodeGenerator;

    public:
        FunctionDecl(const std::string &returnType, const std::string &name,
                     std::vector<std::unique_ptr<ParmVarDecl>> &paras, std::unique_ptr<Stmt> body) : returnType(returnType), name(name), paras(std::move(paras)), body(std::move(body)) {}

        // 通过 Decl 继承
        virtual json toJson() const override;

        // 通过 Decl 继承
        virtual Function* genFunc(CodeGeneratorBase& generator) const override;

        // 通过 Decl 继承
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };
}; // namespace AST