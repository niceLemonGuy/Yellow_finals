#pragma once

#include "date.h"
#include <memory>
#include <string>
#include <vector>

enum class Comparison {
  Less,
  LessOrEqual,
  Greater,
  GreaterOrEqual,
  Equal,
  NotEqual
};

enum class LogicalOperation {
  Or,
  And
};

class Node {
public:
  Node();
  virtual ~Node();

  virtual bool Evaluate(const Date& date, const std::string& event) const = 0;
};

class EmptyNode : public Node {
public:
  EmptyNode();

  bool Evaluate(const Date& date, const std::string& event) const override;
};

class DateComparisonNode : public Node {
public:
  DateComparisonNode(Comparison cmp, const Date& date);

  bool Evaluate(const Date& date, const std::string& event) const override;

private:
  const Comparison _cmp;
  const Date _date;
};

class EventComparisonNode : public Node {
public:
  EventComparisonNode(Comparison cmp, const std::string& event);

  bool Evaluate(const Date& date, const std::string& event) const override;

private:
  const Comparison _cmp;
  const std::string _event;
};

class LogicalOperationNode : public Node {
public:
  LogicalOperationNode(const LogicalOperation& lo,
                       std::shared_ptr<Node> lhs,
                       std::shared_ptr<Node> rhs);

  bool Evaluate(const Date& date, const std::string& event) const override;

private:
  const LogicalOperation _lo;
  std::shared_ptr<Node> _lhs, _rhs;
};
