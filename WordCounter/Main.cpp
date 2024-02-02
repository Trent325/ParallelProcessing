#include "hashmap.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> [numThreads]" << std::endl;
        return 1;
    }

    // Declare and initialize currentSize
    size_t currentSize = 10000;

    // Parse the command line arguments for the number of threads
    int numThreads = 1; // Default value

    if (argc > 2) {
        numThreads = stringToInt(argv[2]);
        if (numThreads <= 0) {
            std::cerr << "Invalid number of threads specified." << std::endl;
            return 1;
        }
    }
    else if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename> [numThreads]" << std::endl;
        return 1;
    }

    initializeGlobals(numThreads, currentSize);

    // Create an array of thread pointers
    std::thread* threads = new std::thread[numThreads];

    const std::string filename(argv[1]);
    const int fileSize = getFileSize(filename);

    // Calculate the range for each thread
    const int chunkSize = fileSize / numThreads;

    // Launch threads
    int start, end;
    start = 0;
    for (int i = 0; i < numThreads; ++i) {
        
        end = (i == numThreads - 1) ? fileSize : (i + 1) * chunkSize;

        // Adjust the end position to the end of a word
        while (end < fileSize && !isspace(getCharAt(filename, end))) {
            ++end;
        }

        // Create and launch a thread for the current chunk
        threads[i] = std::thread(processFile, filename, start, end, std::ref(currentSize));
        start = end;

    }

    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }



    // Add a delay or sleep to ensure all threads finish before the main thread terminates
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Find the actual count of words (non-zero count)
    size_t actualWordCount = 0;
    for (size_t i = 0; i < currentSize; ++i) {
        WordInfo* info = wordInfo->getWordInfoAtIndex(i);

        if (info != nullptr) {

            if (info->count > 0) {
                actualWordCount++;
            }
        }
    }

    std::cout << "Sorting wordInfo array..." << std::endl;

    // Sort the wordInfo array
   wordInfo->sortWordsByCount();

    std::cout << "Sorting completed." << std::endl;


    // Output the word counts to a file
    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return 1;
    }

    for (size_t i = 0; i < currentSize; ++i) {
        WordInfo* info = wordInfo->getWordInfoAtIndex(i);

        if (info != nullptr && info->count > 0) {
            outputFile << "Word: " << info->word << " Count: " << info->count << std::endl;
        }
    }
    outputFile.close();

    delete[] threads;
    cleanupGlobals();

    return 0;
}
