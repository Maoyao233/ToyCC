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
#include "Expr.h"
#include "Stmt.h"
#include "Decl.h"
#include <iostream>
#include <fstream>

using namespace llvm;
using std::shared_ptr;
using namespace AST;

class CodeGenerator : public CodeGeneratorBase
{
    LLVMContext TheContext;
    IRBuilder<>* Builder;
    unique_ptr<Module> TheModule;
    std::map<std::string, AllocaInst*> NamedValues;
    AllocaInst* CreateEntryBlockAlloca(Function* TheFunction, const std::string& VarName);
    std::vector<std::string> errors;
    AllocaInst* retVal;
    Value *getVar(const std::string& varName);

public:
    CodeGenerator()
    {
        Builder = new IRBuilder<>(TheContext);
        TheModule = std::make_unique<Module>("", TheContext);
    }
    Value* logErrorV(const std::string& prompt)
    {
        errors.push_back(prompt);
        return nullptr;
    }
    void print(const char* path = "output.ll")
    {
        if (errors.empty())
        {
            std::error_code EC;
            raw_fd_ostream* out = new raw_fd_ostream(path, EC);
            TheModule->print(*out, nullptr, false, true);
        }
        else
        {
            std::ofstream fout(path);
            if (!fout.is_open())
            {
                std::cerr << "Failed open file " << path << std::endl;
                return;
            }
            fout << "#ERR" << std::endl;
            for (const auto& err : errors)
            {
                fout << err << std::endl;
            }
            fout.close();
        }
    }


    // Í¨¹ý CodeGeneratorBase ¼Ì³Ð
    virtual Value* gen(const IntegerLiteral& expr) override;
    virtual Value* gen(const DeclRefExpr& expr) override;
    virtual Value* gen(const AST::BinaryOperator& expr) override;
    virtual Value* gen(const CallExpr& expr) override;
    virtual Value* gen(const AST::IfStmt& node) override;
    virtual Value* gen(const AST::WhileStmt& node) override;
    virtual Function* gen(const AST::FunctionDecl& decl) override;
    virtual Value* gen(const AST::ReturnStmt& stmt) override;
    virtual Value* gen(const AST::CompoundStmt& stmt) override;
    virtual Value* gen(const AST::VarDecl& decl) override;
    virtual Value* gen(const AST::UnaryOperator& expr) override;
    virtual Value* gen(const AST::ImplicitCastExpr& expr) override;
};