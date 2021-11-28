#pragma once

#include "../AST.h"
namespace
{
  template<typename T>
  std::vector<T> append(std::vector<T> &v, T &&t)
  {
    v.push_back(std::move(t));
    return std::move(v);
  }
  template<typename T>
  std::vector<T> singleton(T &&t)
  {
    std::vector<T> v;
    v.push_back(std::move(t));
    return v;
  }
}

#include <fstream>
#include <variant>
#include <vector>

#include "lexer.h"

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

class LALR_parser
{
private:
  struct _shift_action
  {
    unsigned next_state;
  };

  struct _reduce_action
  {
    unsigned rule_id;
  };

  using _action = std::variant<std::monostate, _shift_action, _reduce_action>;
  std::vector<_action> _trans_table;

  struct _written_data
  {
    uint32_t type : 2;
    uint32_t data : 30;
  };

  void _read_table() {
    std::ifstream in("generated/parser_table.dat", std::ios::binary);

    if (!in.is_open())
      throw parser_exception("Cannot open the parser table (expecting a valid file 'generated/parser_table.dat')", 0, 0);

    auto convert_data_to_variant = [](_written_data data) -> _action {
      _action res{};
      if (data.type == 1)
        res = _shift_action{data.data};
      else if (data.type == 2)
        res = _reduce_action{data.data};
      return res;
    };

    _trans_table.resize(770);
    for (auto &el : _trans_table)
    {
      _written_data data {};
      in.read((char *)&data, sizeof(data));
      el = convert_data_to_variant(data);
    }
  }

  _lexer _the_lexer;
  std::vector<std::string> _symbols = {
    "ws",
    "var",
    "id",
    "d",
    "n",
    "semicolon",
    "assign",
    "plus",
    "minus",
    "asterisk",
    "slash",
    "lp",
    "rp",
  };

  std::string _get_symbol(int c) const
  {
    return c == 0 ? "<EOF>" : _symbols[c - 9];
  }

  std::string _get_error_msg(int c, unsigned state) const
  {
    std::stringstream str;
    str << "Parse error: unexpected token: " << _get_symbol(c) << ", expected one of the following: ";
    bool is_first = true;
    for (int el = 9; el < 22; el++)
      if (el != c && _trans_table[state * 22 + el].index() > 0)
        str << (is_first ? (is_first = false, "") : ", ") << _get_symbol(el);
    if (0 != c && _trans_table[state * 22].index() > 0)
      str << (is_first ? (is_first = false, "") : ", ") << _get_symbol(0);
    return str.str();
  }

  struct _pos_token
  {
    std::string as_string;
    int line, chr;

    operator std::string() &&
    {
      return std::move(as_string);
    }

    const char *c_str() const
    {
      return as_string.c_str();
    }
  };

public:
  LALR_parser()
  {
    _read_table();
  }

  AST::Program parse(std::istream &i)
  {
    using _attr_type0 = AST::Program;
    using _attr_type1 = AST::Program;
    using _attr_type2 = std::vector<std::unique_ptr<AST::Base>>;
    using _attr_type3 = std::unique_ptr<AST::Base>;
    using _attr_type4 = std::unique_ptr<AST::Base>;
    using _attr_type5 = std::unique_ptr<AST::Base>;
    using _attr_type6 = std::unique_ptr<AST::Base>;
    using _attr_type7 = std::unique_ptr<AST::Base>;

    _the_lexer.set_input(i);

    using _work_data_type = std::variant<unsigned, _pos_token, AST::Program, AST::Program, std::vector<std::unique_ptr<AST::Base>>, std::unique_ptr<AST::Base>, std::unique_ptr<AST::Base>, std::unique_ptr<AST::Base>, std::unique_ptr<AST::Base>, std::unique_ptr<AST::Base>>;
    std::vector<_work_data_type> _work;
    _work.emplace_back(_work_data_type{std::in_place_index<0>, 0u});

    bool _to_continue = true;
    std::string $0;
    size_t _last_token_len {};

    while (_to_continue)
    {
      unsigned _state = std::get<0>(_work.back());

      std::visit(overloaded{[&](std::monostate) { _the_lexer.fail(_get_error_msg(_the_lexer.cur_token().token_id, _state)); },
                            [&](_shift_action _act) {
                              auto tok = _the_lexer.cur_token();
                              _work.push_back(_work_data_type{std::in_place_index<1>, _pos_token{tok.str, tok.line, tok.chr}});
                              _work.push_back(_work_data_type{std::in_place_index<0>, _act.next_state});
                              _last_token_len = _the_lexer.cur_token().str.size();
                              _the_lexer.next_token();
                            },
                            [&](_reduce_action _act) {
                              _work_data_type $n;
                              unsigned _lhs {};
                              switch (_act.rule_id)
                              {
                                case 0:
                                {
                                  _work.pop_back();
                                  auto $1 = std::get<3>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<2>, _attr_type0{std::move($1)}};
                                  _to_continue = false;
                                  break;
                                }
                                case 1:
                                {
                                  _work.pop_back();
                                  auto $1 = std::get<4>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<3>, _attr_type1{ std::move($1) }};
                                  _lhs = 2;
                                  break;
                                }
                                case 2:
                                {
                                  _work.pop_back();
                                  auto $2 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $1 = std::get<4>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<3>, _attr_type1{ std::move($1) }};
                                  _lhs = 2;
                                  break;
                                }
                                case 3:
                                {
                                  _work.pop_back();
                                  auto $1 = std::get<5>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<4>, _attr_type2{ singleton(std::move($1)) }};
                                  _lhs = 3;
                                  break;
                                }
                                case 4:
                                {
                                  _work.pop_back();
                                  auto $3 = std::get<5>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $2 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $1 = std::get<4>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<4>, _attr_type2{ append($1, std::move($3)) }};
                                  _lhs = 3;
                                  break;
                                }
                                case 5:
                                {
                                  _work.pop_back();
                                  auto $4 = std::get<6>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $3 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $2 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $1 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<5>, _attr_type3{ std::make_unique<AST::VariableAssign>(AST::CodeLoc{$2.line, $2.chr}, std::move($2.as_string), true, std::move($4)) }};
                                  _lhs = 4;
                                  break;
                                }
                                case 6:
                                {
                                  _work.pop_back();
                                  auto $2 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $1 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<5>, _attr_type3{ std::make_unique<AST::VariableAssign>(AST::CodeLoc{$2.line, $2.chr}, std::move($2.as_string), true) }};
                                  _lhs = 4;
                                  break;
                                }
                                case 7:
                                {
                                  _work.pop_back();
                                  auto $1 = std::get<6>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<5>, _attr_type3{ std::move($1) }};
                                  _lhs = 4;
                                  break;
                                }
                                case 8:
                                {
                                  _work.pop_back();
                                  auto $3 = std::get<6>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $2 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $1 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<6>, _attr_type4{ std::make_unique<AST::VariableAssign>(AST::CodeLoc{$1.line, $1.chr}, std::move($1.as_string), false, std::move($3)) }};
                                  _lhs = 5;
                                  break;
                                }
                                case 9:
                                {
                                  _work.pop_back();
                                  auto $1 = std::get<7>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<6>, _attr_type4{ std::move($1) }};
                                  _lhs = 5;
                                  break;
                                }
                                case 10:
                                {
                                  _work.pop_back();
                                  auto $3 = std::get<8>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $2 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $1 = std::get<7>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<7>, _attr_type5{ std::make_unique<AST::Binary>(AST::Binary::ADD, std::move($1), std::move($3)) }};
                                  _lhs = 6;
                                  break;
                                }
                                case 11:
                                {
                                  _work.pop_back();
                                  auto $3 = std::get<8>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $2 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $1 = std::get<7>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<7>, _attr_type5{ std::make_unique<AST::Binary>(AST::Binary::SUB, std::move($1), std::move($3)) }};
                                  _lhs = 6;
                                  break;
                                }
                                case 12:
                                {
                                  _work.pop_back();
                                  auto $1 = std::get<8>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<7>, _attr_type5{ std::move($1) }};
                                  _lhs = 6;
                                  break;
                                }
                                case 13:
                                {
                                  _work.pop_back();
                                  auto $3 = std::get<9>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $2 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $1 = std::get<8>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<8>, _attr_type6{ std::make_unique<AST::Binary>(AST::Binary::MUL, std::move($1), std::move($3)) }};
                                  _lhs = 7;
                                  break;
                                }
                                case 14:
                                {
                                  _work.pop_back();
                                  auto $3 = std::get<9>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $2 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $1 = std::get<8>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<8>, _attr_type6{ std::make_unique<AST::Binary>(AST::Binary::DIV, std::move($1), std::move($3)) }};
                                  _lhs = 7;
                                  break;
                                }
                                case 15:
                                {
                                  _work.pop_back();
                                  auto $1 = std::get<9>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<8>, _attr_type6{ std::move($1) }};
                                  _lhs = 7;
                                  break;
                                }
                                case 16:
                                {
                                  _work.pop_back();
                                  auto $2 = std::get<9>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $1 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<9>, _attr_type7{ std::make_unique<AST::Unary>(AST::Unary::MINUS, std::move($2)) }};
                                  _lhs = 8;
                                  break;
                                }
                                case 17:
                                {
                                  _work.pop_back();
                                  auto $2 = std::get<9>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $1 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<9>, _attr_type7{ std::make_unique<AST::Unary>(AST::Unary::PLUS, std::move($2)) }};
                                  _lhs = 8;
                                  break;
                                }
                                case 18:
                                {
                                  _work.pop_back();
                                  auto $3 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $2 = std::get<6>(std::move(_work.back()));
                                  _work.pop_back();
                                  _work.pop_back();
                                  auto $1 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<9>, _attr_type7{ AST::move_loc(std::move($2), $1.line, $1.chr) }};
                                  _lhs = 8;
                                  break;
                                }
                                case 19:
                                {
                                  _work.pop_back();
                                  auto $1 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<9>, _attr_type7{ std::make_unique<AST::VariableRef>(AST::CodeLoc{$1.line, $1.chr}, std::move($1.as_string)) }};
                                  _lhs = 8;
                                  break;
                                }
                                case 20:
                                {
                                  _work.pop_back();
                                  auto $1 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<9>, _attr_type7{ std::make_unique<AST::Number>(AST::CodeLoc{$1.line, $1.chr}, std::atof($1.c_str())) }};
                                  _lhs = 8;
                                  break;
                                }
                                case 21:
                                {
                                  _work.pop_back();
                                  auto $1 = std::get<1>(std::move(_work.back()));
                                  _work.pop_back();
                                  $n = _work_data_type{std::in_place_index<9>, _attr_type7{ std::make_unique<AST::Number>(AST::CodeLoc{$1.line, $1.chr}, std::atof($1.c_str())) }};
                                  _lhs = 8;
                                  break;
                                }
                              }

                              unsigned _rollback_state = std::get<0>(_work.back());
                              _work.push_back(std::move($n));

                              if (_to_continue)
                                _work.push_back(std::get<_shift_action>(
                                                   _trans_table[_rollback_state * 22 + _lhs])
                                                   .next_state);
                            }
        }, _trans_table[_state * 22 + _the_lexer.cur_token().token_id]);
    }
    return std::get<2>(std::move(_work.back()));
  }
};
