#include "date.h"
#include <stdexcept>
#include <vector>
#include <iomanip>
#include <tuple>
using namespace std;

// конструктор выбрасывает исключение, если его аргументы некорректны
Date::Date(int new_year, int new_month, int new_day) {
  year = new_year;
  if (new_month > 12 || new_month < 1) {
    throw logic_error("Month value is invalid: " + to_string(new_month));
  }
  month = new_month;
  if (new_day > 31 || new_day < 1) {
    throw logic_error("Day value is invalid: " + to_string(new_day));
  }
  day = new_day;
}

int Date::GetYear() const {
  return year;
}
int Date::GetMonth() const {
  return month;
}
int Date::GetDay() const {
  return day;
}

Date ParseDate(istream& is) {
  int year;
  is >> year;
  is.ignore(1);

  int month;
  is >> month;
  is.ignore(1);

  int day;
  is >> day;
  is.eof();

  return Date(year, month, day);
}

// даты будут по умолчанию выводиться в нужном формате
ostream& operator << (ostream& stream, const Date& date) {
  stream << setw(4) << setfill('0') << date.GetYear() <<
      "-" << setw(2) << setfill('0') << date.GetMonth() <<
      "-" << setw(2) << setfill('0') << date.GetDay();
  return stream;
}

bool operator < (const Date& lhs, const Date& rhs) {
  return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay())
       < make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool operator > (const Date& lhs, const Date& rhs) {
  return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay())
       > make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool operator == (const Date& lhs, const Date& rhs) {
  return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay())
      == make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool operator != (const Date& lhs, const Date& rhs) {
  return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay())
      != make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

string MakeStringDateEvent(const Date& date, const string& event) {
  stringstream stream;
  stream << date << " " << event;
  return stream.str();
}
