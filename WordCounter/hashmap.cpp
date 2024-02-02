#include "hashmap.hpp"
#include <iostream>

std::mutex arrayMutex1;

HashMap::HashMap(size_t size) : table(new WordInfo[size]), tableSize(size), numElements(0) {
    for (size_t i = 0; i < size; ++i) {
        table[i].word = "";
        table[i].count = 0;
    }
}

HashMap::~HashMap() {
    delete[] table;
}

void HashMap::insert(const std::string& key) {
    std::lock_guard<std::mutex> lock(arrayMutex1); // Protect the entire insert operation

    // Check if resizing is needed
    if (getNumElements() >= tableSize * 0.7) {
        resizeTable(tableSize * 2);  // Double the size, adjust as needed
    }

    size_t index = customHash(key);
    size_t originalIndex = index;

    while (!table[index].word.empty() && table[index].word != key) {
        index = (index + 1) % tableSize;

        // Break if we have traversed the entire table without finding an empty slot
        if (index == originalIndex) {
            std::cerr << "Error: Hash table is full even after resizing." << std::endl;
            return;
        }
    }

    if (index < tableSize) {
        if (table[index].word.empty()) {
            table[index].word = key;
            table[index].count = 1;
            numElements++;  // Increment the count of elements
        }
        else {
            table[index].count++;
        }
    }
}

WordInfo* HashMap::find(const std::string& key) {
    std::lock_guard<std::mutex> lock(arrayMutex1); // Protect the entire find operation

    size_t index = customHash(key);
    while (!table[index].word.empty() && table[index].word != key) {
        index = (index + 1) % tableSize;
    }

    if (index < tableSize) {
        return &table[index];
    }

    return nullptr;
}

WordInfo* HashMap::getWordInfoAtIndex(size_t index) {
    std::lock_guard<std::mutex> lock(arrayMutex1);

    if (index < tableSize) {
        return &table[index];
    }
    else {
        return nullptr;  // Out of bounds index, handle accordingly
    }
}

size_t HashMap::customHash(const std::string& str) {
    // std::lock_guard<std::mutex> lock(arrayMutex1); // Uncomment if needed

    size_t hash = 0;
    for (char c : str) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' and c <= 'z')) {  // Use 'and' instead of '&&'
            char lowerC = std::tolower(c);
            hash = (hash * 31 + lowerC) % tableSize;
        }
    }
    return hash;
}

size_t HashMap::getNumElements() const {
    return numElements;
}

void HashMap::resizeTable(size_t newSize) {
    WordInfo* newTable = new WordInfo[newSize];

    for (size_t i = 0; i < newSize; ++i) {
        newTable[i].word = "";
        newTable[i].count = 0;
    }

    // Rehash existing elements into the new table
    for (size_t i = 0; i < tableSize; ++i) {
        if (!table[i].word.empty()) {
            size_t newIndex = customHash(table[i].word);
            while (!newTable[newIndex].word.empty()) {
                newIndex = (newIndex + 1) % newSize;
            }
            newTable[newIndex] = table[i];
        }
    }

    // Swap the new table with the old table
    delete[] table;
    table = newTable;
    tableSize = newSize;
}

void HashMap::quickSort(WordInfo* arr, size_t low, size_t high) {
    if (low < high) {
        size_t pivotIndex = (low + high) / 2;
        WordInfo pivotValue = arr[pivotIndex];

        size_t i = low;
        size_t j = high;

        while (i <= j) {
            while (arr[i].count > pivotValue.count) {
                i++;
            }

            while (arr[j].count < pivotValue.count) {
                j--;
            }

            if (i <= j) {
                // Swap elements at i and j
                std::swap(arr[i], arr[j]);

                i++;
                j--;
            }
        }

        // Recursively sort the two partitions
        if (low < j) {
            quickSort(arr, low, j);
        }

        if (i < high) {
            quickSort(arr, i, high);
        }
    }
}

// Function to sort the WordInfo array based on count
void HashMap::sortWordsByCount() {
    std::lock_guard<std::mutex> lock(arrayMutex1);

    // Sort the table array using quicksort
    quickSort(table, 0, tableSize - 1);
}