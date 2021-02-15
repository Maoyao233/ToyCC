#include "CodeGenerator.h"

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
/// 由于目前只有int，因此不用指定类型
AllocaInst* CodeGenerator::CreateEntryBlockAlloca(Function* TheFunction,
    const std::string& VarName) {
    IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
        TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(Type::getInt32Ty(TheContext), 0,
        VarName.c_str());
}

Value* CodeGenerator::gen(const IntegerLiteral& expr)
{
    return ConstantInt::get(TheContext, APInt(32, expr.getValue(), true));
}

Value* CodeGenerator::gen(const DeclRefExpr& expr)
{
    if (Value* V = getVar(expr.getName()))
    {
        return V;
        //return Builder->CreateLoad(V, expr.getName().c_str());
    }
    return logErrorV("Unknown variable " + expr.getName());
}

Value* CodeGenerator::getVar(const std::string& varName)
{
    if(auto global = TheModule->getGlobalVariable(varName))
    {
        return global;
    }
    if(auto local = NamedValues[varName])
    {
        return local;
    }
    return nullptr;
}

Value* CodeGenerator::gen(const AST::BinaryOperator& expr)
{
    if (expr.isAssignment())
    {
        DeclRefExpr* LHSE = dynamic_cast<DeclRefExpr*>(expr.LHS.get());
        if (!LHSE)
        {
            return logErrorV("Required lvalue at left of assignment.");
        }
        
        Value* Val = expr.RHS->genCode(*this);

        Value* Var = getVar(LHSE->getName());
        if (!Var)
        {
            return logErrorV("Undefined references of " + LHSE->getName());
        }
        switch (expr.getOpKind())
        {
            case AST::BinaryOperatorKind::BO_Assign:
            {
                Builder->CreateStore(Val, Var);
                return Val;
            }
            case AST::BinaryOperatorKind::BO_MulAssign:
            {
                Value* VarVal = Builder->CreateLoad(Var);
                Value* tmp = Builder->CreateMul(VarVal, Val, "mulassigntmp");
                Builder->CreateStore(tmp, Var);
                return tmp;
            }
            case AST::BinaryOperatorKind::BO_DivAssign:
            {
                Value* VarVal = Builder->CreateLoad(Var);
                Value* tmp = Builder->CreateSDiv(VarVal, Val, "divassigntmp");
                Builder->CreateStore(tmp, Var);
                return tmp;
            }
            case AST::BinaryOperatorKind::BO_RemAssign:
            {
                Value* VarVal = Builder->CreateLoad(Var);
                Value* tmp = Builder->CreateSRem(VarVal, Val, "remassigntmp");
                Builder->CreateStore(tmp, Var);
                return tmp;
            }
            case AST::BinaryOperatorKind::BO_AddAssign:
            {
                Value* VarVal = Builder->CreateLoad(Var);
                Value* tmp = Builder->CreateAdd(VarVal, Val, "addassigntmp");
                Builder->CreateStore(tmp, Var);
                return tmp;
            }
            case AST::BinaryOperatorKind::BO_SubAssign:
            {
                Value* VarVal = Builder->CreateLoad(Var);
                Value* tmp = Builder->CreateSub(VarVal, Val, "subassigntmp");
                Builder->CreateStore(tmp, Var);
                return tmp;
            }
            case AST::BinaryOperatorKind::BO_ShlAssign:
            {
                Value* VarVal = Builder->CreateLoad(Var);
                Value* tmp = Builder->CreateShl(VarVal, Val, "shlassigntmp");
                Builder->CreateStore(tmp, Var);
                return tmp;
            }
            case AST::BinaryOperatorKind::BO_ShrAssign:
            {
                Value* VarVal = Builder->CreateLoad(Var);
                Value* tmp = Builder->CreateAShr(VarVal, Val, "shrassigntmp");
                Builder->CreateStore(tmp, Var);
                return tmp;
            }
            case AST::BinaryOperatorKind::BO_AndAssign:
            {
                Value* VarVal = Builder->CreateLoad(Var);
                Value* tmp = Builder->CreateAnd(VarVal, Val, "andassigntmp");
                Builder->CreateStore(tmp, Var);
                return tmp;
            }
            case AST::BinaryOperatorKind::BO_XorAssign:
            {
                Value* VarVal = Builder->CreateLoad(Var);
                Value* tmp = Builder->CreateXor(VarVal, Val, "xorassigntmp");
                Builder->CreateStore(tmp, Var);
                return tmp;
            }
            case AST::BinaryOperatorKind::BO_OrAssign:
            {
                Value* VarVal = Builder->CreateLoad(Var);
                Value* tmp = Builder->CreateOr(VarVal, Val, "orassigntmp");
                Builder->CreateStore(tmp, Var);
                return tmp;
            }
            default:
                return nullptr;
        }
    }
    else
    {
        Value* L = expr.LHS->genCode(*this);
        Value* R = expr.RHS->genCode(*this);
        if (!L || !R)
        {
            return nullptr;
        }
        if (L->getType()->getIntegerBitWidth() < 32)
        {
            L = Builder->CreateIntCast(L, Type::getInt32Ty(TheContext), true);
        }
        if (R->getType()->getIntegerBitWidth() < 32)
        {
            R = Builder->CreateIntCast(R, Type::getInt32Ty(TheContext), true);
        }
        switch (expr.getOpKind())
        {
            
            case AST::BinaryOperatorKind::BO_Mul:
                return Builder->CreateMul(L, R, "multmp");
            case AST::BinaryOperatorKind::BO_Div:
                return Builder->CreateSDiv(L, R, "divtmp");
            case AST::BinaryOperatorKind::BO_Rem:
                return Builder->CreateSRem(L, R, "remtmp");
            case AST::BinaryOperatorKind::BO_Add:
                return Builder->CreateAdd(L, R, "addtmp");
            case AST::BinaryOperatorKind::BO_Sub:
                return Builder->CreateSub(L, R, "subtmp");
            case AST::BinaryOperatorKind::BO_Shl:
                return Builder->CreateShl(L, R, "shltmp");
            case AST::BinaryOperatorKind::BO_Shr:
                return Builder->CreateAShr(L, R, "shrtmp");
            case AST::BinaryOperatorKind::BO_LT:
                return Builder->CreateICmpSLT(L, R, "lttmp");
            case AST::BinaryOperatorKind::BO_GT:
                return Builder->CreateICmpSGT(L, R, "gttmp");
            case AST::BinaryOperatorKind::BO_LE:
                return Builder->CreateICmpSLE(L, R, "letmp");
            case AST::BinaryOperatorKind::BO_GE:
                return Builder->CreateICmpSGE(L, R, "getmp");
            case AST::BinaryOperatorKind::BO_EQ:
                return Builder->CreateICmpEQ(L, R, "eqtmp");
            case AST::BinaryOperatorKind::BO_NE:
                return Builder->CreateICmpNE(L, R, "netmp");
            case AST::BinaryOperatorKind::BO_And:
                return Builder->CreateAnd(L, R, "andtmp");
            case AST::BinaryOperatorKind::BO_Xor:
                return Builder->CreateXor(L, R, "xortmp");
            case AST::BinaryOperatorKind::BO_Or:
                return Builder->CreateOr(L, R, "ortmp");
            case AST::BinaryOperatorKind::BO_LAnd:
            {
                // todo: 实现“短路”特性
                Value* op1 = Builder->CreateICmpNE(L, ConstantInt::get(TheContext, APInt(32, 0, true)));
                Value* op2 = Builder->CreateICmpNE(R, ConstantInt::get(TheContext, APInt(32, 0, true)));
                return Builder->CreateAnd(op1, op2, "landtmp");
            }
            case AST::BinaryOperatorKind::BO_LOr:
            {
                // todo: 实现“短路”特性
                Value* op1 = Builder->CreateICmpNE(L, ConstantInt::get(TheContext, APInt(32, 0, true)));
                Value* op2 = Builder->CreateICmpNE(R, ConstantInt::get(TheContext, APInt(32, 0, true)));
                return Builder->CreateOr(op1, op2, "lortmp");
            }
            default:
                break;
        }
    }
    return nullptr;
}

Value* CodeGenerator::gen(const CallExpr& expr)
{
    Function* fun = TheModule->getFunction(dynamic_cast<DeclRefExpr*>(expr.function.get())->getName());
    if (!fun)
    {
        return logErrorV("Unknown function referenced");
    }
    if (expr.paras.size() != fun->arg_size())
    {
        return logErrorV("The function " + fun->getName().str() + " requried for "
            + std::to_string(fun->arg_size()) +
            " arguments, but " + std::to_string(expr.paras.size()) + " were given.");
    }

    std::vector<Value*> ArgsV;
    for (const auto& arg : expr.paras)
    {
        ArgsV.emplace_back(arg->genCode(*this));
        if (!ArgsV.back())
            return nullptr;
    }

    return Builder->CreateCall(fun, ArgsV, "calltmp");
}

static void removeDeadCode(BasicBlock* BB)
{
    //assert(BB->getTerminator() && "BasicBlock must have terminator!");
    bool isDead = false;
    for (auto it = BB->begin(); it != BB->end(); ++it)
    {
        if (isDead)
        {
            it->removeFromParent();
            break;
        }
        if (it->isTerminator())
        {
            isDead = true;
        }
    }
}

Value* CodeGenerator::gen(const AST::IfStmt& node)
{
    Value* CondV = node.cond->genCode(*this);
    if (!CondV)
        return nullptr;

    // Convert condition to a bool by comparing non-equal to 0.
    if (CondV->getType() != Type::getInt1Ty(TheContext))
    {
        CondV = Builder->CreateICmpNE(CondV,
            ConstantInt::get(TheContext, APInt(32, 0)), "ifcond");
    }

    Function* TheFunction = Builder->GetInsertBlock()->getParent();

    BasicBlock* ThenBB = BasicBlock::Create(TheContext, "if.then");
    BasicBlock* EndBB = BasicBlock::Create(TheContext, "if.end");

    Function::iterator insertPos = ----TheFunction->end();
    insertPos = TheFunction->getBasicBlockList().insertAfter(insertPos, ThenBB);

    if (node.elseBody)
    {
        BasicBlock* ElseBB = BasicBlock::Create(TheContext, "if.else");

        Builder->CreateCondBr(CondV, ThenBB, ElseBB);
        insertPos = TheFunction->getBasicBlockList().insertAfter(insertPos, ElseBB);
        Builder->SetInsertPoint(ElseBB);
        node.elseBody->genCode(*this);
        Builder->CreateBr(EndBB);
    }
    else
    {
        Builder->CreateCondBr(CondV, ThenBB, EndBB);
    }

    // Emit then value.
    Builder->SetInsertPoint(ThenBB);
    node.body->genCode(*this);
    Builder->CreateBr(EndBB);

    TheFunction->getBasicBlockList().insertAfter(insertPos, EndBB);
    Builder->SetInsertPoint(EndBB);
    return CondV;
}

Value* CodeGenerator::gen(const AST::WhileStmt& node)
{
    Function* TheFunction = Builder->GetInsertBlock()->getParent();
    BasicBlock* condBB = BasicBlock::Create(TheContext, "while.cond");
    BasicBlock* LoopBB = BasicBlock::Create(TheContext, "while.body");
    BasicBlock* AfterBB =
        BasicBlock::Create(TheContext, "while.end");
    Builder->CreateBr(condBB);
    auto insertPos = ----TheFunction->getBasicBlockList().end();
    insertPos = TheFunction->getBasicBlockList().insertAfter(insertPos, condBB);
    Builder->SetInsertPoint(condBB);
    Value* CondV = node.cond->genCode(*this);
    Value* loopCond = CondV;
    if (CondV->getType() != Type::getInt1Ty(TheContext))
    {
        loopCond = Builder->CreateICmpNE(CondV,
            ConstantInt::get(TheContext, APInt(32, 0)));
    }

    Builder->CreateCondBr(loopCond, LoopBB, AfterBB);
    insertPos = TheFunction->getBasicBlockList().insertAfter(insertPos, LoopBB);
    //TheFunction->getBasicBlockList().push_back(LoopBB);
    Builder->SetInsertPoint(LoopBB);
    node.body->genCode(*this);
    Builder->CreateBr(condBB);
    insertPos = TheFunction->getBasicBlockList().insertAfter(insertPos, AfterBB);
    //TheFunction->getBasicBlockList().push_back(AfterBB);
    Builder->SetInsertPoint(AfterBB);
    return CondV;
}

Function* CodeGenerator::gen(const AST::FunctionDecl& decl)
{
    std::vector<Type*> Args(decl.paras.size(),
        Type::getInt32Ty(TheContext));//目前参数类型只有int

    FunctionType* FT = FunctionType::get(decl.returnType == "void" ? Type::getVoidTy(TheContext) : Type::getInt32Ty(TheContext),
        Args, false);

    auto F = TheModule->getFunction(decl.name);
    if (F)
    {
        if (!F->empty() || !decl.body)
        {
            logErrorV("Redefined funciton " + decl.name);
            return nullptr;
        }
        if (F->arg_size() != decl.paras.size())
        {
            logErrorV("Wrong arguments number " + std::to_string(decl.paras.size()));
            return nullptr;
        }
    }
    else {
        F = Function::Create(FT, Function::ExternalLinkage, decl.name, TheModule.get());
    }

    unsigned Idx = 0;
    for (auto& Arg : F->args())
        Arg.setName(decl.paras[Idx++]->getName());

    if (!decl.body)
    {
        return F;
    }

    BasicBlock* BB = BasicBlock::Create(TheContext, "entry", F);
    BasicBlock* returnBB = BasicBlock::Create(TheContext, "return", F);
    Builder->SetInsertPoint(returnBB);
    retVal = !F->getReturnType()->isVoidTy() ? CreateEntryBlockAlloca(F, "retval") : nullptr;
    if (retVal)
    {
        auto ret = Builder->CreateLoad(retVal);
        Builder->CreateRet(ret);
    }
    else
    {
        Builder->CreateRetVoid();
    }
    Builder->SetInsertPoint(BB);

    auto NamedValuesBackup = std::move(NamedValues);

    for (auto& Arg : F->args()) {
        AllocaInst* Alloca = CreateEntryBlockAlloca(F, Arg.getName().str());

        Builder->CreateStore(&Arg, Alloca);

        NamedValues[Arg.getName().str()] = Alloca;
    }

    decl.body->genCode(*this);
    Builder->CreateBr(returnBB);
    for (auto& it : F->getBasicBlockList())
    {
        removeDeadCode(&it);
    }

    std::string error_output;
    raw_ostream* out = new raw_string_ostream(error_output);
    if (verifyFunction(*F, out))
        std::cerr << error_output << std::endl;
    NamedValues = std::move(NamedValuesBackup);
    return F;
}

Value* CodeGenerator::gen(const AST::ReturnStmt& stmt)
{
    Function* TheFunction = Builder->GetInsertBlock()->getParent();
    auto& returnBB = TheFunction->getBasicBlockList().back();
    if (!stmt.returnValue)
    {
        return Builder->CreateBr(&returnBB);
    }
    else
    {
        Builder->CreateStore(stmt.returnValue->genCode(*this), retVal);
        return Builder->CreateBr(&returnBB);
    }
}

Value* CodeGenerator::gen(const AST::CompoundStmt& stmt)
{
    auto NamedValueBackup = NamedValues;

    Value* res = nullptr;

    for (const auto& bodyStmt : stmt.body)
    {
        auto tmp = bodyStmt->genCode(*this);
        if (!res)
        {
            res = tmp;
        }
    }
    NamedValues = NamedValueBackup;
    return res;
}

Value* CodeGenerator::gen(const AST::VarDecl& decl)
{
    if (NamedValues[decl.getName()])
    {
        return logErrorV("Redefined " + decl.getName());
    }

    Value* InitVal = decl.hasInit ? decl.initValue->genCode(*this) : nullptr;

    if (Builder->GetInsertBlock())
    {
        Function* TheFunction = Builder->GetInsertBlock()->getParent();

        AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, decl.getName());
        NamedValues[decl.getName()] = Alloca;

        if (InitVal)
        {
            return Builder->CreateStore(InitVal, Alloca);
        }
        return nullptr;
    }
    else
    {
        if (TheModule->getGlobalVariable(decl.getName()))
        {
            return logErrorV("Redefined variable " + decl.getName());
        }
        GlobalVariable* var = new GlobalVariable(*TheModule, Type::getInt32Ty(TheContext), false,
            GlobalValue::ExternalLinkage,
            Constant::getIntegerValue(Type::getInt32Ty(TheContext),
                APInt(32, 0, true)), decl.getName());
        return var;
    }
}

Value* CodeGenerator::gen(const AST::UnaryOperator& expr)
{
    Value* val = expr.body->genCode(*this);
    switch (expr.op)
    {
        case UnaryOperatorKind::UO_PreInc:
        {
            Value* add_val = Builder->CreateNSWAdd(val, Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 1, true)),"preInc");
            return Builder->CreateStore(add_val, val);
        }
        case UnaryOperatorKind::UO_PreDec:
        {
            Value* sub_val = Builder->CreateNSWSub(val, Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 1, true)),"preDec");
            return Builder->CreateStore(sub_val, val);
        }
        case UnaryOperatorKind::UO_PostInc:
        {
            Value* add_val = Builder->CreateNSWAdd(val, Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 1, true)),"postInc");
            Builder->CreateStore(add_val, val);
            return val;
        }
        case UnaryOperatorKind::UO_PostDec:
        {
            Value* sub_val = Builder->CreateNSWSub(val, Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 1, true)),"postDec");
            Builder->CreateStore(sub_val, val);
            return val;
        }
        case UnaryOperatorKind::UO_Not:
        {
            return Builder->CreateNot(val, "not");
        }
        case UnaryOperatorKind::UO_Minus:
        {
            return Builder->CreateNeg(val, "neg");
        }
        case UnaryOperatorKind::UO_Plus:
        {
            return val;
        }
        case UnaryOperatorKind::UO_LNot:
        {
            return Builder->CreateICmpEQ(val, ConstantInt::get(TheContext, APInt(32, 0, true)));
        }
        
        default:
            break;
    }
    return val;
}

Value* CodeGenerator::gen(const AST::ImplicitCastExpr& expr)
{
    Value* val = expr.subExpr->genCode(*this);
    if (expr.castKind == "LValueToRValue")
    {
        return Builder->CreateLoad(val);
    }
    return val;
}
