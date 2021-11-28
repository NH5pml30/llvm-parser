#include "AST.h"
#include <unordered_map>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"

namespace llvm_globals
{
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder(context);
  llvm::Module module("cgsgforever", context);
  std::unordered_map<std::string, llvm::Value *> var_map;
  auto eval_type = llvm::Type::getDoubleTy(llvm_globals::context);
} // namespace llvm_global

llvm::Value *AST::Number::codegen()
{
  return llvm::ConstantFP::get(llvm_globals::context, llvm::APFloat(val));
}

llvm::Value *AST::VariableAssign::codegen()
{
  llvm::Value *v = llvm_globals::var_map[name];
  if (v == nullptr)
  {
    if (!is_define)
      return report_error("undeclared variable assignment");
    else
      llvm_globals::var_map[name] = v =
          llvm_globals::builder.CreateAlloca(llvm_globals::eval_type, nullptr, name);
  }
  if (val != nullptr)
  {
    auto valcode = val->codegen();
    if (valcode == nullptr)
      return nullptr;
    llvm_globals::builder.CreateStore(valcode, v);
    return valcode;
  }
  return v;
}

llvm::Value *AST::VariableRef::codegen()
{
  llvm::Value *v = llvm_globals::var_map[name];
  if (v == nullptr)
    return report_error("unknown variable name '", name, "'");
  return llvm_globals::builder.CreateLoad(llvm_globals::eval_type, v);
}

llvm::Value *AST::Binary::codegen()
{
  llvm::Value *l = lhs->codegen();
  if (l == nullptr)
    return nullptr;
  llvm::Value *r = rhs->codegen();
  if (r == nullptr)
    return nullptr;

  switch (op)
  {
  case OP::ADD:
    return llvm_globals::builder.CreateFAdd(l, r, "addtmp");
  case OP::SUB:
    return llvm_globals::builder.CreateFSub(l, r, "subtmp");
  case OP::MUL:
    return llvm_globals::builder.CreateFMul(l, r, "multmp");
  case OP::DIV:
    return llvm_globals::builder.CreateFDiv(l, r, "divtmp");
  }
}

llvm::Value *AST::Unary::codegen()
{
  llvm::Value *a = arg->codegen();
  if (a == nullptr)
    return nullptr;

  switch (op)
  {
  case OP::PLUS:
    return a;
  case OP::MINUS:
    return llvm_globals::builder.CreateFNeg(a, "negtmp");
  }
}

llvm::Value *AST::Program::codegen()
{
  auto int_type = llvm::Type::getInt32Ty(llvm_globals::context);
  llvm::FunctionType *entry_type = llvm::FunctionType::get(int_type, {}, false);
  llvm::Function *entry = llvm::Function::Create(entry_type, llvm::Function::ExternalLinkage,
                                                 "main", llvm_globals::module);
  llvm::BasicBlock *bb = llvm::BasicBlock::Create(llvm_globals::context, "entry", entry);
  llvm_globals::builder.SetInsertPoint(bb);

  llvm::Value *ret{};
  for (auto &&stt : stts)
    ret = stt->codegen();

  if (ret != nullptr)
  {
    llvm_globals::builder.CreateRet(llvm_globals::builder.CreateFPToSI(ret, int_type));
    llvm::verifyFunction(*entry);
    llvm::verifyModule(llvm_globals::module);
    return entry;
  }

  entry->eraseFromParent();
  return nullptr;
}
