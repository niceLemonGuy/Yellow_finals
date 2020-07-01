#pragma once

#include <sstream>
#include <vector>

enum class TokenType {
  DATE,
  EVENT,
  COLUMN,
  LOGICAL_OP,
  COMPARE_OP,
  PAREN_LEFT,
  PAREN_RIGHT,
};

// Структура токенов для хранения распаршеного потока "строка - тип"
struct Token {
  const std::string value;
  const TokenType type;
};

std::vector<Token> Tokenize(std::istream& cl);
