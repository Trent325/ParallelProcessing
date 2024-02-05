void HashMap::quickSort(WordInfo* arr, size_t low, size_t high) {
    // Check for null pointer
    if (arr == nullptr) {
        std::cerr << "Error: Null pointer in quickSort." << std::endl;
        return;
    }

    if (low < high) {
        // Verify indices
        if (low >= tableSize || high >= tableSize) {
            std::cerr << "Error: Indices out of bounds in quickSort. low=" << low << ", high=" << high << ", tableSize=" << tableSize << std::endl;
            return;
        }

        size_t pivotIndex = (low + high) / 2;

        // Verify pivotIndex
        if (pivotIndex >= tableSize) {
            std::cerr << "Error: Pivot index out of bounds in quickSort. pivotIndex=" << pivotIndex << ", tableSize=" << tableSize << std::endl;
            return;
        }

        WordInfo pivotValue = arr[pivotIndex];

        size_t i = low;
        size_t j = high;

        while (i <= j) {
            // Find the first element on the left side that is greater than the pivot
            while (arr[i].count < pivotValue.count) {
                // Verify index
                if (i >= tableSize) {
                    std::cerr << "Error: Index out of bounds in quickSort. i=" << i << ", tableSize=" << tableSize << std::endl;
                    return;
                }

                i++;
            }

            // Find the first element on the right side that is smaller than the pivot
            while (arr[j].count > pivotValue.count) {
                // Verify index
                if (j >= tableSize) {
                    std::cerr << "Error: Index out of bounds in quickSort. j=" << j << ", tableSize=" << tableSize << std::endl;
                    return;
                }

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
