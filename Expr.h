/** @file Expr.h
* @brief Expr��������
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
    * @brief ö�����ͣ�˫Ŀ�����������
    */
    enum class BinaryOperatorKind
    {
#define BINARY_OPERATION(Name, Spelling) BO_##Name,
#include "OperationKinds.def"
    };

    /**
    *
    * @brief ö�����ͣ���Ŀ�����������
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
     * @brief ����������
     * ��򵥵ı��ʽ������"123"��Ŀǰ��֧��int����������
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

        // ͨ�� Expr �̳�
        virtual json toJson() const override;


        // ͨ�� Expr �̳�
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
        virtual Function* genFunc(CodeGeneratorBase& generator) const override 
        {
            return nullptr;
        };
    };

    // todo:FloatLiteral

    /**
 * @brief �Ա���������ʹ��
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
        // ͨ�� Expr �̳�
        virtual json toJson() const override;

        // ͨ�� Expr �̳�
        virtual Value* genCode(CodeGeneratorBase& generator) const override;

        // ͨ�� Expr �̳�
        virtual Function* genFunc(CodeGeneratorBase& generator) const override;
    };

    /**
 * @brief ʹ��˫Ŀ������ı��ʽ
 * �������ȼ�����ԣ����һ��ִ�е�����Ϊ˫Ŀ���㼴Ϊ����
 * ����1+2����f(x)+(x*y)-7
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


        // ͨ�� Expr �̳�
        virtual json toJson() const override;
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    /**
     * @brief ʹ�õ�Ŀ������ı��ʽ
     * �������ȼ�����ԣ����һ��ִ�е�����Ϊ��Ŀ���㼴Ϊ����
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

        // ͨ�� Expr �̳�
        virtual json toJson() const override;
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

 /**
 * @brief �����Ű����ı��ʽ
 * ����(1+2)��(x)
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

        // ͨ�� Expr �̳�
        virtual json toJson() const override;
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    /**
     * @brief ��������
    */
    class CallExpr : public Expr
    {
        ///�����õĺ���������ΪDeclRefExpr
        unique_ptr<Expr> function;
        ///�����б�
        std::vector<unique_ptr<Expr>> paras;
        friend CodeGenerator;

    public:
        
        CallExpr(unique_ptr<Expr> func, std::vector<unique_ptr<Expr>> &&paras) : function(std::move(func)), paras(std::move(paras)) {}

        // ͨ�� Expr �̳�
        virtual json toJson() const override;
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

    /**
     * @brief ����ת��
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

        // ͨ�� Expr �̳�
        virtual json toJson() const override;
        virtual Value* genCode(CodeGeneratorBase& generator) const override;
    };

}; // namespace AST
