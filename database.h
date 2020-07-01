#pragma once

#include "date.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

using DateEvent = std::pair<Date, std::string>;

class Database {
public:

  Database();

  ~Database();

  void Add(const Date& date, const std::string& event);

  void Print(std::ostream& os) const;

  template <class Predicate>
  int RemoveIf(Predicate predicate) {
    int res = 0;
    std::vector<Date> keys_storage;
    auto it = storage.begin();
    while(it != storage.end()) {
      const auto &date = it->first;
      auto border = stable_partition(it->second.begin(), it->second.end(),
            [date, predicate](const std::string& event) {
              return predicate(date, event);
            });
      for(auto i = it->second.begin(); i != border; ++i) {
        pointer.erase({it->first, *i});
        res++;
      }
      it->second.erase(it->second.begin(), border);

      if(it->second.empty()) {
        keys_storage.push_back(it->first);
      }
      it = next(it);
    }
    for(auto i : keys_storage) {
      storage.erase(i);
    }
    return res;
  }

  template <class Predicate>
  std::vector<std::string> FindIf(Predicate predicate) const {
    std::vector<std::string> result;
    if(storage.empty()) {
      return result;
    }
    auto it_m = storage.begin();
    auto it_v = it_m->second.begin();
    while(it_m != storage.end()) {
      Date date = it_m->first;
      it_v = find_if(it_v, it_m->second.end(),
          [predicate, date](const std::string& event) {
            return predicate(date, event);
      });
      if(it_v != it_m->second.end()) {
        result.push_back(MakeStringDateEvent(it_m->first, *it_v));
        it_v++;
      } else {
        it_m = next(it_m);
        it_v = it_m->second.begin();
      }
    }
    return result;
  }

  std::string Last(const Date& date) const;

  std::map<Date, std::vector<std::string>> GetStorage() const;

private:
  std::map<Date, std::vector<std::string>> storage;
  std::set<std::pair<Date, std::string>> pointer;
};

std::string ParseEvent(std::istream& is);

void TestDatabase();
