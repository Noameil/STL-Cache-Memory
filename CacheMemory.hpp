#include <exception>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#pragma once

class DuplicateKeyException : public std::exception {
public:
  const char *what() const noexcept override {
    return "DuplicateKeyException: Key already exists in memory.";
  }
};

class ObjectNotExistException : public std::exception {
public:
  const char *what() const noexcept override {
    return "ObjectNotExistException: Key does not exist in memory.";
  }
};

template <typename K, typename V> class CacheMemory {
private:
  std::map<K, std::shared_ptr<V>> mainMemory;
  std::map<K, std::weak_ptr<V>> cacheMemory;

public:
  void add(const K &key, const V &value) {
    if (mainMemory.find(key) != mainMemory.end()) {
      throw DuplicateKeyException();
    }
    std::shared_ptr<V> valueSPtr = std::make_shared<V>(value);
    mainMemory.insert({key, valueSPtr});
  }

  void erase(const K &key) {
    if (mainMemory.find(key) == mainMemory.end()) {
      throw ObjectNotExistException();
    }
    mainMemory.erase(key);
  }

  std::shared_ptr<V> get(const K &key) {
    if (cacheMemory.find(key) != cacheMemory.end()) {
      std::shared_ptr<V> foundPointerShared = cacheMemory[key].lock();

      if (foundPointerShared)
        return foundPointerShared;
      cacheMemory.erase(key);
      return nullptr;
    }

    if (mainMemory.find(key) != mainMemory.end()) {
      cacheMemory.insert({key, mainMemory[key]});
      return mainMemory[key];
    }
    return nullptr;
  }

  std::vector<V> getCacheValues() {
    std::vector<V> cacheVector;
    for (const std::pair<K, std::weak_ptr<V>> pair : cacheMemory) {
      std::shared_ptr<V> tempShared = pair.second.lock();
      if (tempShared) {
        cacheVector.push_back(*tempShared);
      }
    }
    return cacheVector;
  };
};