#pragma once

#include <string>
#include <mutex>
#include "Stack.hpp"


struct WordInfo {
    std::string word;
    int count;
    WordInfo() : count(0) {}
    WordInfo(const WordInfo& other) : word(other.word), count(other.count) {}

};

class HashMap {
public:
    explicit HashMap(size_t size);
    ~HashMap();

    void insert(const std::string& key);
    WordInfo* find(const std::string& key);
    WordInfo* getWordInfoAtIndex(size_t index);

    // New function declaration
    void sortWordsByCount();
    size_t numElements;
    size_t getNumElements() const;
    void resizeTable(size_t newSize);
    size_t tableSize;
    void quickSort(WordInfo* arr, size_t low, size_t high);

    

private:
    size_t customHash(const std::string& str);

    WordInfo* table;

};
