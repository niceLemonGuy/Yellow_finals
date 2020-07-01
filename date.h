#pragma once

#include <iostream>

class Date {
public:
  // ����������� ����������� ����������, ���� ��� ��������� �����������
  Date(int new_year, int new_month, int new_day);

  int GetYear() const;
  int GetMonth() const;
  int GetDay() const;

private:
  int year;
  int month;
  int day;
};

Date ParseDate(std::istream& date);

// ���� ����� �� ��������� ���������� � ������ �������
std::ostream& operator<<(std::ostream& stream, const Date& date);

std::string MakeStringDateEvent(const Date& date, const std::string& event);

bool operator < (const Date& lhs, const Date& rhs);

bool operator > (const Date& lhs, const Date& rhs);

bool operator == (const Date& lhs, const Date& rhs);

bool operator != (const Date& lhs, const Date& rhs);
