#pragma once
#pragma warning(push, 0)
#include "nlohmann/json.hpp"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#pragma warning(pop)

using namespace llvm;

using json = nlohmann::ordered_json;

class CodeGeneratorBase;

/**
 * @class ASTNode
 * @brief 语法分析树结点的基类。
 *
 * 所有结点从共同的基类继承主要为了方便使用虚函数toJson与genCode实现多态性;
 * genCode使用访问者模式。
*/
namespace AST
{
    class ASTNode
    {
    public:
        virtual json toJson() const = 0;
        virtual Value* genCode(CodeGeneratorBase& generator) const = 0;
        virtual Function* genFunc(CodeGeneratorBase& generator) const
        {
            return nullptr;
        }
    };
    class IntegerLiteral;
    class DeclRefExpr;
    class BinaryOperator;
    class UnaryOperator;
    class CallExpr;
    class IfStmt;
    class WhileStmt;
    class ReturnStmt;
    class CompoundStmt;
    class FunctionDecl;
    class VarDecl;
    class ImplicitCastExpr;

}; // namespace AST

class CodeGeneratorBase
{
public:
    virtual Value* gen(const AST::IntegerLiteral& expr) = 0;
    virtual Value* gen(const AST::DeclRefExpr& expr) = 0;
    virtual Value* gen(const AST::BinaryOperator& expr) = 0;
    virtual Value* gen(const AST::CallExpr& expr) = 0;
    virtual Value* gen(const AST::IfStmt& stmt) = 0;
    virtual Value* gen(const AST::WhileStmt& stmt) = 0;
    virtual Value* gen(const AST::ReturnStmt& stmt) = 0;
    virtual Value* gen(const AST::VarDecl& decl) = 0;
    virtual Value* gen(const AST::CompoundStmt& decl) = 0;
    virtual Value* gen(const AST::UnaryOperator& expr) = 0;
    virtual Function* gen(const AST::FunctionDecl& decl) = 0;
    virtual Value* gen(const AST::ImplicitCastExpr& expr) = 0;
};
