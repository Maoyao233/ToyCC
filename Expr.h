/** @file Expr.h
* @brief Expr及其子类
**/

#pragma once
#include <string>
#include <memory>
#include <vector>
#include "ASTNode.h"

class CodeGenerator;

using std::unique_ptr;

namespace AST
{
    /**
    * 
    * @brief 枚举类型，双目运算符的种类
    */
    enum class BinaryOperatorKind
    {
#define BINARY_OPERATION(Name, Spelling) BO_##Name,
#include "OperationKinds.def"
    };

    /**
    *
    * @brief 枚举类型，单目运算符的种类
    */
    enum class UnaryOperatorKind
    {
#define UNARY_OPERATION(Name, Spelling) UO_##Name,
#include "OperationKinds.def"
    };

    class Expr : public ASTNode
    {
    protected:
        bool isConst = false;
        bool isLvalue = false;
    public:
        bool getIsConst() const
        {
            return isConst;
        }
        bool getIsLvalue() const
        {
            return isLvalue;
        }
    };

    /**
     * @class IntegerLiteral
     * @brief 整型字面量
     * 最简单的表达式，形如"123"；目前仅支持int类型字面量
    */
    class IntegerLiteral : public Expr
    {
        int value;

    public:
        IntegerLiteral(int val) : value(val) { isConst = true; }
        int getValue() const
        {
            return value;
        }

        // 通过 Expr 继承
        virtual json toJson() const override;


        // 通过 Expr 继承
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
        virtual Function* genFunc(CodeGeneratorBase& generator) const override 
        {
            return nullptr;
        };
    };

    // todo:FloatLiteral

    /**
 * @brief 对变量或函数的使用
 * 
*/
    class DeclRefExpr : public Expr
    {
        std::string name;
        bool isCall;
        // todo: type
    public:
        DeclRefExpr(const std::string& name, bool isCall = false) : name(name), isCall(isCall) { isLvalue = !isCall; }
        const std::string& getName() const {
            return name;
        }
        // 通过 Expr 继承
        virtual json toJson() const override;

        // 通过 Expr 继承
        virtual Value* genCode(CodeGeneratorBase& generator) const override;

        // 通过 Expr 继承
        virtual Function* genFunc(CodeGeneratorBase& generator) const override;
    };

    /**
 * @brief 使用双目运算符的表达式
 * 按照优先级结合性，最后一个执行的运算为双目运算即为此类
 * 形如1+2或者f(x)+(x*y)-7
*/
    class BinaryOperator : public Expr
    {
        BinaryOperatorKind op;
        unique_ptr<Expr> LHS, RHS;
        friend CodeGenerator;
    public:
        BinaryOperator(BinaryOperatorKind op, unique_ptr<Expr> LHS,
            unique_ptr<Expr> RHS);

        BinaryOperatorKind getOpKind() const {
            return op;
        }
        bool isAssignment() const;


        // 通过 Expr 继承
        virtual json toJson() const override;
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    /**
     * @brief 使用单目运算符的表达式
     * 按照优先级结合性，最后一个执行的运算为单目运算即为此类
     */
    class UnaryOperator : public Expr
    {
        UnaryOperatorKind op;
        unique_ptr<Expr> body;
        friend CodeGenerator;
    public:
        UnaryOperator(UnaryOperatorKind op, unique_ptr<Expr> body) : op(op), body(std::move(body)) {}

        UnaryOperatorKind getOpKind() const {
            return op;
        }

        // 通过 Expr 继承
        virtual json toJson() const override;
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

 /**
 * @brief 被括号包括的表达式
 * 形如(1+2)或(x)
*/
    class ParenExpr : public Expr
    {

        unique_ptr<Expr> subExpr;

    public:
        ParenExpr(unique_ptr<Expr> expr) : subExpr(std::move(expr)) 
        {
            isConst = subExpr->getIsConst();
            isLvalue = subExpr->getIsLvalue();
        }

        // 通过 Expr 继承
        virtual json toJson() const override;
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    /**
     * @brief 函数调用
    */
    class CallExpr : public Expr
    {
        ///所调用的函数，类型为DeclRefExpr
        unique_ptr<Expr> function;
        ///参数列表
        std::vector<unique_ptr<Expr>> paras;
        friend CodeGenerator;

    public:
        
        CallExpr(unique_ptr<Expr> func, std::vector<unique_ptr<Expr>> &&paras) : function(std::move(func)), paras(std::move(paras)) {}

        // 通过 Expr 继承
        virtual json toJson() const override;
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    /**
     * @brief 类型转换
    */
    class CastExpr : public Expr
    {
    protected:
        StringRef castKind;
        unique_ptr<Expr> subExpr;

    public:
        CastExpr(unique_ptr<Expr> expr, const char* type):subExpr(std::move(expr)), castKind(type) {}
    };

    class ImplicitCastExpr : public CastExpr
    {
        friend CodeGenerator;
    public:
        ImplicitCastExpr(unique_ptr<Expr> expr, const char* type):CastExpr(std::move(expr), type) {}

        // 通过 Expr 继承
        virtual json toJson() const override;
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

}; // namespace AST
