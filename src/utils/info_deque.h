#ifndef _INFO_DEQUE_H
#define _INFO_DEQUE_H

#include <algorithm>
#include <deque>
#include <string>
#include <vector>

class ReverseWrapper {
  std::deque<std::vector<std::string>> &info;
  int startIndex;

public:
  ReverseWrapper(std::deque<std::vector<std::string>> &info, int startIndex)
      : info(info), startIndex(startIndex) {}

  std::deque<std::vector<std::string>>::reverse_iterator begin() {
    auto it = info.rbegin();
    std::advance(it, startIndex);
    return it;
  }

  std::deque<std::vector<std::string>>::reverse_iterator end() {
    return info.rend();
  }
};

class InfoDeque {
private:
  std::deque<std::vector<std::string>> info;
  size_t maxSize;
  int startIndex = 0;

public:
  InfoDeque(size_t maxSize) : maxSize(maxSize) {}

  void addMessage(const std::vector<std::string> &message) {
    if (info.size() >= maxSize) {
      info.pop_front();
    }
    info.push_back(message);
  }

  std::vector<std::string> front() { return info.front(); }

  std::vector<std::string> back() { return info.back(); }

  size_t size() const { return info.size(); }

  bool empty() const { return info.empty(); }

  std::deque<std::vector<std::string>>::iterator begin() {
    return info.begin();
  }

  std::deque<std::vector<std::string>>::iterator end() { return info.end(); }

  std::deque<std::vector<std::string>>::const_iterator begin() const {
    return info.begin();
  }

  std::deque<std::vector<std::string>>::const_iterator end() const {
    return info.end();
  }

  void increaseStartIndex() {
    if (startIndex < info.size() - 1) {
      startIndex++;
    }
  }

  void decreaseStartIndex() {
    if (startIndex > 0) {
      startIndex--;
    }
  }

  ReverseWrapper reverse() { return ReverseWrapper(info, startIndex); }
};

#endif
