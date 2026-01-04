#ifndef INFO_DEQUE_H
#define INFO_DEQUE_H

#include <algorithm>
#include <deque>
#include <string>
#include <vector>
#include "utils/point.h"

enum class MessageType {
  SYSTEM,
  COMBAT,
  LOOT,
  INFO
};

struct MessageEntry {
  MessageType type;
  Point source;
  bool hasSource;
  std::vector<std::string> lines;
  int repeatCount;
};

class ReverseWrapper {
  std::deque<MessageEntry> &info;
  int startIndex;

public:
  ReverseWrapper(std::deque<MessageEntry> &info, int startIndex)
      : info(info), startIndex(startIndex) {}

  std::deque<MessageEntry>::reverse_iterator begin() {
    auto it = info.rbegin();
    std::advance(it, startIndex);
    return it;
  }

  std::deque<MessageEntry>::reverse_iterator end() {
    return info.rend();
  }
};

class InfoDeque {
private:
  std::deque<MessageEntry> info;
  size_t maxSize;
  int startIndex = 0;

public:
  InfoDeque(size_t maxSize) : maxSize(maxSize) {}

  void addMessage(MessageType type, const Point *source,
                  const std::vector<std::string> &lines) {
    MessageEntry entry{type, Point(), source != nullptr, lines, 1};
    if (source != nullptr) {
      entry.source = *source;
    }

    if (!info.empty()) {
      const auto &last = info.back();
      if (last.type == entry.type && last.hasSource == entry.hasSource &&
          (!entry.hasSource || last.source == entry.source) &&
          last.lines == entry.lines) {
        info.back().repeatCount++;
        return;
      }
    }

    if (info.size() >= maxSize) {
      info.pop_front();
      if (startIndex > 0) {
        startIndex--;
      }
    }
    info.push_back(std::move(entry));
  }

  void addMessage(MessageType type, const Point *source, std::string &&line) {
    addMessage(type, source, std::vector<std::string>{std::move(line)});
  }

  void addMessage(const std::vector<std::string> &message) {
    addMessage(MessageType::INFO, nullptr, message);
  }
  void addMessage(std::string &&message) {
    addMessage(MessageType::INFO, nullptr,
               std::vector<std::string>{std::move(message)});
  }

  MessageEntry front() { return info.front(); }

  MessageEntry back() { return info.back(); }

  size_t size() const { return info.size(); }

  bool empty() const { return info.empty(); }

  std::deque<MessageEntry>::iterator begin() {
    return info.begin();
  }

  std::deque<MessageEntry>::iterator end() { return info.end(); }

  std::deque<MessageEntry>::const_iterator begin() const {
    return info.begin();
  }

  std::deque<MessageEntry>::const_iterator end() const {
    return info.end();
  }

  void increaseStartIndex() {
    if (info.empty()) {
      return;
    }
    if (startIndex < static_cast<int>(info.size()) - 1) {
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

#endif // INFO_DEQUE_H
