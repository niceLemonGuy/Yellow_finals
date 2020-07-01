#include "node.h"
using namespace std;

Node::Node() {}
Node::~Node() {}

EmptyNode::EmptyNode() {}
bool EmptyNode::Evaluate(const Date& date, const string& event) const {
  return true;
}

DateComparisonNode::DateComparisonNode(Comparison cmp,
                                       const Date& date)
  : _cmp(cmp)
  , _date(date) {

}
bool DateComparisonNode::Evaluate(const Date& date,
                                  const string& event) const {
  switch(_cmp) {
  case Comparison::Less:
    return date < _date;
  case Comparison::LessOrEqual:
    return (date < _date || date == _date);
  case Comparison::Greater:
    return date > _date;
  case Comparison::GreaterOrEqual:
    return (date > _date || date == _date);
  case Comparison::Equal:
    return date == _date;
  case Comparison::NotEqual:
    return date != _date;
  default:
    break;
  }
  return false;
}

EventComparisonNode::EventComparisonNode(Comparison cmp,
                                         const string& event)
  : _cmp(cmp)
  , _event(event) {

}
bool EventComparisonNode::Evaluate(const Date& date,
                                   const string& event) const {
  switch(_cmp) {
  case Comparison::Less:
    return event < _event;
  case Comparison::LessOrEqual:
    return (event < _event || event == _event);
  case Comparison::Greater:
    return event > _event;
  case Comparison::GreaterOrEqual:
    return (event > _event || event == _event);
  case Comparison::Equal:
    return event == _event;
  case Comparison::NotEqual:
    return event != _event;
  default:
    break;
  }
  return false;
}

LogicalOperationNode::LogicalOperationNode(const LogicalOperation& lo,
                                           shared_ptr<Node> lhs,
                                           shared_ptr<Node> rhs)
  : _lo(lo)
  , _lhs(lhs)
  , _rhs(rhs) {

}
bool LogicalOperationNode::Evaluate(const Date& date,
                                    const string& event) const {
  if(_lo == LogicalOperation::And) {
    return (_lhs->Evaluate(date, event) && _rhs->Evaluate(date, event));;
  } else if(_lo == LogicalOperation::Or) {
    return (_lhs->Evaluate(date, event) || _rhs->Evaluate(date, event));
  }
  return false;
}
