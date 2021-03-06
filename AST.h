#pragma once

#include <iostream>
#include <unordered_map>

#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"

namespace llvm_globals
{
  extern llvm::LLVMContext context;
  extern llvm::IRBuilder<> builder;
  extern llvm::Module module;
  extern std::unordered_map<std::string, llvm::Value *> var_map;
}  // namespace llvm_globals

namespace AST
{
  struct CodeLoc
  {
    int line, chr;
  };

  class Base
  {
  public:
    CodeLoc loc;

    Base(CodeLoc loc = {}) : loc(loc) {}

    virtual ~Base() = default;

    virtual llvm::Value *codegen() = 0;

    template<typename... Ts>
    nullptr_t report_error(Ts &&... args)
    {
      ((std::cerr << "Error at " << loc.line << ":" << loc.chr << ": ") << ... << args) << std::endl;
      return nullptr;
    }
  };

  inline std::unique_ptr<Base> move_loc(std::unique_ptr<Base> arg, int line, int chr)
  {
    arg->loc = {line, chr};
    return std::move(arg);
  }

  class Number : public Base
  {
    double val;

  public:
    Number(CodeLoc loc, double val) : Base(loc), val(val) {}

    llvm::Value *codegen() override;
  };

  class VariableAssign : public Base
  {
    std::string name;
    bool is_define;
    std::unique_ptr<Base> val;

  public:
    VariableAssign(CodeLoc loc, std::string name, bool is_define, std::unique_ptr<Base> val = nullptr)
        : Base(loc), name(std::move(name)), is_define(is_define), val(std::move(val))
    {
    }

    llvm::Value *codegen() override;
  };

  class VariableRef : public Base
  {
    std::string name;

  public:
    VariableRef(CodeLoc loc, std::string name)
        : Base(loc), name(std::move(name))
    {
    }

    llvm::Value *codegen() override;
  };

  class Binary : public Base
  {
  public:
    enum OP
    {
      ADD,
      SUB,
      MUL,
      DIV
    };

  private:
    OP op;
    std::unique_ptr<Base> lhs, rhs;

  public:
    Binary(OP op, std::unique_ptr<Base> lhs, std::unique_ptr<Base> rhs)
        : Base(lhs->loc), op(op), lhs(std::move(lhs)), rhs(std::move(rhs))
    {
    }

    llvm::Value *codegen() override;
  };

  class Unary : public Base
  {
  public:
    enum OP
    {
      PLUS,
      MINUS
    };

  private:
    OP op;
    std::unique_ptr<Base> arg;

  public:
    Unary(OP op, std::unique_ptr<Base> arg) : Base(arg->loc), op(op), arg(std::move(arg))
    {
    }

    llvm::Value *codegen() override;
  };

  class Program : public Base
  {
    std::vector<std::unique_ptr<AST::Base>> stts;

  public:
    Program(std::vector<std::unique_ptr<AST::Base>> stts) : Base(stts[0]->loc), stts(std::move(stts)) {}

    llvm::Value *codegen() override;
  };
}  // namespace AST
