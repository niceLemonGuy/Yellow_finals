#include "database.h"
#include "test_runner.h"
#include "condition_parser.h"

#include <iostream>
#include <tuple>

using namespace std;
using DateEvent = pair<Date, string>;

Database::Database() {}
Database::~Database() {}

void Database::Add(const Date& date, const string& event) {
  pair<Date, string> de = {date, event};
  if(!pointer.count(de)) {
    storage[date].push_back(event);
    pointer.insert(de);
  }
}

void Database::Print(ostream& os) const {
  for(auto i : storage) {
    for(auto j : i.second) {
      os << i.first << " ";
      os << j << endl;
    }
  }
}

string Database::Last(const Date& date) const {
  if(date < storage.begin()->first || storage.empty()) {
    throw invalid_argument("");
  }
  auto it = storage.upper_bound(date);
  return MakeStringDateEvent(prev(it)->first, *prev(it)->second.rbegin());
}

map<Date, vector<string>> Database::GetStorage() const {
  return storage;
}

string ParseEvent(istream& is) {
  string str1, str2;
  if(is >> str1) {
    getline(is, str2);
  }
  return str1 + str2;
}

void TestDatabase() {
  {
    Database db;
    stringstream ss("0001-01-01");
    Date date = ParseDate(ss);
    db.Add(date, "event_1 event_2 event_3");
    AssertEqual(db.GetStorage().at({1, 1, 1}), vector<string>{"event_1 event_2 event_3"}, "TestDatabase test 1");
  }
  {
    Database db;
    stringstream ss("9999-12-31");
    Date date = ParseDate(ss);
    db.Add(date, "event_1");
    AssertEqual(db.GetStorage().at({9999, 12, 31}), vector<string>{"event_1"}, "TestDatabase test 1");
  }
  {
    Database db;
    stringstream ss(R"(1-01-01 2-02-02 3-03-03 event == "event_1" OR event == "event_3")");
    Date date1 = ParseDate(ss);
    Date date2 = ParseDate(ss);
    Date date3 = ParseDate(ss);
    db.Add(date1, "event_1");
    db.Add(date2, "event_1");
    db.Add(date2, "event_2");
    db.Add(date2, "event_3");
    db.Add(date3, "event_3");
    auto condition = ParseCondition(ss);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.RemoveIf(predicate), 4, "TestDatabase test 5");
    AssertEqual(db.GetStorage().at(date2), vector<string>{"event_2"}, "TestDatabase test 6");
    AssertEqual(db.GetStorage().count(date1), 0, "TestDatabase test 8");
    AssertEqual(db.GetStorage().count(date2), 1, "TestDatabase test 9");
    AssertEqual(db.GetStorage().count(date3), 0, "TestDatabase test 10");
  }
  {
    Database db;
    stringstream ss(R"(1-01-01 2-02-02 3-03-03")");
    stringstream ss_empty;
    Date date1 = ParseDate(ss);
    Date date2 = ParseDate(ss);
    Date date3 = ParseDate(ss);
    db.Add(date1, "event_1");
    db.Add(date2, "event_1");
    db.Add(date2, "event_2");
    db.Add(date2, "event_3");
    db.Add(date3, "event_3");
    auto condition = ParseCondition(ss_empty);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.RemoveIf(predicate), 5, "TestDatabase test 11");
    AssertEqual(db.GetStorage().size(), 0, "TestDatabase test 12");
  }
  {
    Database db;
    stringstream ss(R"(2020-01-01 date == 2020-01-01 AND event == "event_1")");
    Date date = ParseDate(ss);
    db.Add(date, "event_1");
    db.Add(date, "event_2");
    db.Add(date, "event_3");
    auto condition = ParseCondition(ss);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.FindIf(predicate), vector<string>{"2020-01-01 event_1"}, "TestDatabase test 12");
  }
  {
    Database db;
    db.Add({2020, 1, 1}, "event_1");
    db.Add({2020, 1, 5}, "event_2");
    db.Add({2019, 12, 31}, "event_3");
    AssertEqual(db.Last({2020, 1, 1}), "2020-01-01 event_1", "TestDatabase test 13");
    AssertEqual(db.Last({2020, 1, 6}), "2020-01-05 event_2", "TestDatabase test 14");
  }
}
