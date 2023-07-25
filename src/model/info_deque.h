#ifndef _INFO_DEQUE_H
#define _INFO_DEQUE_H

#include <deque>
#include <string>

class InfoDeque {
private:
  std::deque<std::string> info;
  size_t maxSize;

public:
  InfoDeque(size_t maxSize) : maxSize(maxSize) {}

  void addMessage(const std::string &message) {
    if (info.size() >= maxSize) {
      info.pop_front();
    }
    info.push_back(message);
  }

  std::string front() { return info.front(); }

  std::string back() { return info.back(); }

  size_t size() const { return info.size(); }

  bool empty() const { return info.empty(); }

  std::deque<std::string>::iterator begin() { return info.begin(); }

  std::deque<std::string>::iterator end() { return info.end(); }

  std::deque<std::string>::const_iterator begin() const { return info.begin(); }

  std::deque<std::string>::const_iterator end() const { return info.end(); }
};

#endif
