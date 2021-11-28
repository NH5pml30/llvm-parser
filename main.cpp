#include <iostream>
#include "AST.h"
#include "generated/parser.h"

int main()
{
  try
  {
    AST::Program parsed = LALR_parser().parse(std::cin);
    parsed.codegen();
  }
  catch (std::exception &e)
  {
    print_exception(e);
    return 1;
  }

  {
    std::error_code ec;
    auto out = llvm::raw_fd_ostream("out.ll", ec);
    if (ec)
    {
      std::cerr << ec.message() << std::endl;
      return 1;
    }
    llvm_globals::module.print(out, nullptr);
  }
  system("llc -o out.asm out.ll");
  system("clang out.ll -o out.exe");
  return 0;
}
