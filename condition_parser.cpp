#include "condition_parser.h"
#include "token.h"

#include <map>

using namespace std;

template <class It> shared_ptr<Node> ParseComparison(It& current, It end) {
  if (current == end) { // Начало выражение равно концу
    throw logic_error("Expected column name: date or event"); // Кинуть исключение
  }

  Token& column = *current; // Разыменовать итератор и сохранить ссылку в ожидании, что это будет определитель дальнейшего выражения: date или event
  if (column.type != TokenType::COLUMN) { // Если токен не является определителем дальнейшего выражения: date или event
    throw logic_error("Expected column name: date or event"); // Кинуть исключение
  }
  ++current; // Передвинуть итератор на следующий токен

  if (current == end) { // Если уже дошли до конца выражения
    throw logic_error("Expected comparison operation"); // Кинуть исключение
  }

  Token& op = *current; // Разыменовать итератор и сохранить ссылку в ожидании, что это будет оператор сравнения
  if (op.type != TokenType::COMPARE_OP) { // Если токен не является оператором сравнения
    throw logic_error("Expected comparison operation"); // Кинуть исключение
  }
  ++current; // Передвинуть итератор на следующий токен

  if (current == end) { // Если уже дошли до конца выражения
    throw logic_error("Expected right value of comparison"); // Кинуть исключение
  }

  Comparison cmp; // Создать экземпляр класса Comparison, для хранения оператора сравнения
  if (op.value == "<") { // Дальше определяется оператор сравнения
    cmp = Comparison::Less;
  } else if (op.value == "<=") {
    cmp = Comparison::LessOrEqual;
  } else if (op.value == ">") {
    cmp = Comparison::Greater;
  } else if (op.value == ">=") {
    cmp = Comparison::GreaterOrEqual;
  } else if (op.value == "==") {
    cmp = Comparison::Equal;
  } else if (op.value == "!=") {
    cmp = Comparison::NotEqual;
  } else { // Оператор не был определён
    throw logic_error("Unknown comparison token: " + op.value); // Кинуть исключение
  }

  const string& value = current->value; // Сохранить ссылку на выражение
  ++current; // Передвинуть итератор на следующий токен

  if (column.value == "date") { // Если токен определяется как дата
    istringstream is(value); // Создать поток и инициализировать его строкой с датой
    return make_shared<DateComparisonNode>(cmp, ParseDate(is)); // Вернуть класс по нахождению даты
  } else {
    return make_shared<EventComparisonNode>(cmp, value); // Вернуть класс по нахождению событий
  }
}

template <class It>
shared_ptr<Node> ParseExpression(It& current, It end, unsigned precedence) {
  if (current == end) { // Проверка на пустоту вектора
    return shared_ptr<Node>(); // Вернуть пустой Node
  }

  shared_ptr<Node> left;

  if (current->type == TokenType::PAREN_LEFT) { // Если скобка открывается
    ++current; // consume '(' // Пропустить левую скобку, означающую начало выражения
    left = ParseExpression(current, end, 0u); // Запустить функцию ещё раз минуя скобку (0 - уровень старшинства в иерархии выражений)
    if (current == end || current->type != TokenType::PAREN_RIGHT) { // Если итератор дошёл до конца строки или итератор указывает не на правую скобку
      throw logic_error("Missing right paren"); // Кинуть исключение
    }
    ++current; // consume ')' // Пропустить правую скобку, означающую конец выражения
  } else {
    left = ParseComparison(current, end); // Иначе подать выражение в функцию определитель сравнения
  }

  const map<LogicalOperation, unsigned> precedences = { // Создать словарь старшинства логических операций
      {LogicalOperation::Or, 1}, {LogicalOperation::And, 2}
  };

  while (current != end && current->type != TokenType::PAREN_RIGHT) { // Запустить цикл пока не конец выражения и правая скобка
    if (current->type != TokenType::LOGICAL_OP) { // Если токен не логическая операция
      throw logic_error("Expected logic operation"); // Кинуть исключение
    }

    const auto logical_operation = current->value == "AND" ? LogicalOperation::And // Определение логической операции
                                                           : LogicalOperation::Or;
    const auto current_precedence = precedences.at(logical_operation); // Сохранить в переменную показатель старшинства
    if (current_precedence <= precedence) { // Если это выражение младше или равно тому, которое вызвало данную функцию
      break; // Прервать выполнение цикла
    }

    ++current; // consume op // Передвинуть итератор на следующий токен

    left = make_shared<LogicalOperationNode>( // Создать узел сравнения, добавить в него оператор и две части выражения для сравнения
        logical_operation, left, ParseExpression(current, end, current_precedence)
    );
  }

  return left;
}

shared_ptr<Node> ParseCondition(istream& is) {
  auto tokens = Tokenize(is); // Разделить строки из потока на кусоки "токены", с которыми удобно работать дальше
  auto current = tokens.begin(); // Сохранить итератор указывающий на начало вектора токенов
  auto top_node = ParseExpression(current, tokens.end(), 0u); // Разобрать выражение

  if (!top_node) {
    top_node = make_shared<EmptyNode>();
  }

  if (current != tokens.end()) { // Итератор не указывает на конец вектора токенов. Что-то не так
    throw logic_error("Unexpected tokens after condition"); // Кинуть исключение
  }

  return top_node;
}
