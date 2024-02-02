#pragma once

#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <mutex>
#include "hashmap.hpp"

extern std::mutex arrayMutex;
extern HashMap* wordInfo; // Update to use the new HashMap
extern std::atomic<int> numThreads;
extern size_t MAX_WORDS;
extern std::atomic<size_t> currentArraySize;

void initializeGlobals(int threads, size_t arraySize);

void processFile(const std::string& filename, int start, int end, size_t& arraySize);

int getFileSize(const std::string& filename);

int stringToInt(const char* str);

char getCharAt(const std::string& filename, int position);
void cleanupGlobals();

