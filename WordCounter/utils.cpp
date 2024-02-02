#// utils.cpp
#include "utils.hpp"

std::mutex arrayMutex;
HashMap* wordInfo = nullptr;
std::atomic<int> numThreads(1);
size_t MAX_WORDS = 10000;
std::atomic<size_t> currentArraySize(10000);

void initializeGlobals(int threads, size_t arraySize) {
    numThreads.store(threads);
    currentArraySize.store(arraySize);

    // Allocate memory for wordInfo
    wordInfo = new HashMap(arraySize);
}

void processFile(const std::string& filename, int start, int end, size_t& arraySize) {
    std::ifstream file(filename);
    std::string word;

    // Move to the starting position
    file.seekg(start);

    //std::cout << "Thread ID: " << std::this_thread::get_id() << ", Start Pos: " << start << ", End Pos: " << end << std::endl;

    // Process words in the specified range
    while (file >> word) {
        //std::cout << "Read word: " << word << ", File position: " << file.tellg() << std::endl;

        // Check the stream's state
        if (!file) {
            if (file.eof()) {
                // End of file reached
                break;
            }
            else {
                std::cerr << "Error reading from the file." << std::endl;
                break;
            }
        }

        // Break out of the loop if the file position exceeds the end position
        if (file.tellg() >= end) {
            //std::cout << "Reached end position. Breaking out of the loop." << std::endl;
            break;
        }

        // Move to the next word
        file.ignore(std::numeric_limits<std::streamsize>::max(), ' ');

        // Filter out special characters, numbers, and punctuation
        std::string filteredWord;
        for (char c : word) {
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                // Convert to lowercase before hashing
                char lowerC = (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
                filteredWord += lowerC;
            }
        }

        // If the filtered word is not empty, update the count
        if (!filteredWord.empty()) {
            // Use a lock to protect the critical section
            std::lock_guard<std::mutex> lock(arrayMutex);

            // Check if the array size needs to be increased
            if (currentArraySize.load() == MAX_WORDS) {
                // Double the array size (you can adjust this logic as needed)
                currentArraySize.store(2 * MAX_WORDS);
            }

            //std::cout << "Inserting word: " << filteredWord << std::endl;  // Add this logging

            // Insert the word into the HashMap
            wordInfo->insert(filteredWord);
        }
    }

    // Print the file position after the loop
    std::cout << "After loop, File position: " << file.tellg() << std::endl;
}

int getFileSize(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate);
    return static_cast<int>(file.tellg());
}

int stringToInt(const char* str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    // Check for a sign
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }

    // Iterate through the string to convert to integer
    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        }
        else {
            // Handle invalid characters
            std::cerr << "Invalid character in number: " << str << std::endl;
            exit(1);
        }
        ++i;
    }

    return sign * result;
}

char getCharAt(const std::string& filename, int position) {
    std::ifstream file(filename);
    file.seekg(position);
    char c;
    file.get(c);
    return c;
}
void cleanupGlobals() {
    std::lock_guard<std::mutex> lock(arrayMutex);
    delete wordInfo;
    wordInfo = nullptr;
}
