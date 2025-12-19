#ifndef COCKTAILSORT_H
#define COCKTAILSORT_H

#include <vector>
#include <functional>

class CocktailSort {
public:
    CocktailSort() = default;

    template<typename T>
    void sort(std::vector<T>& arr) {
        if (arr.empty()) return;

        bool swapped = true;
        int start = 0;
        int end = static_cast<int>(arr.size()) - 1;

        while (swapped) {
            swapped = false;

            for (int i = start; i < end; ++i) {
                if (arr[i] > arr[i + 1]) {
                    std::swap(arr[i], arr[i + 1]);
                    swapped = true;
                }
            }

            if (!swapped) break;

            swapped = false;
            --end;

            for (int i = end - 1; i >= start; --i) {
                if (arr[i] > arr[i + 1]) {
                    std::swap(arr[i], arr[i + 1]);
                    swapped = true;
                }
            }
            ++start;
        }
    }

    template<typename T, typename Compare>
    void sort(std::vector<T>& arr, Compare comp) {
        bool swapped = true;
        int start = 0;
        int end = arr.size() - 1;

        while (swapped) {
            swapped = false;

            for (int i = start; i < end; ++i) {
                if (comp(arr[i + 1], arr[i])) {
                    std::swap(arr[i], arr[i + 1]);
                    swapped = true;
                }
            }

            if (!swapped) break;

            swapped = false;
            --end;

            for (int i = end - 1; i >= start; --i) {
                if (comp(arr[i + 1], arr[i])) {
                    std::swap(arr[i], arr[i + 1]);
                    swapped = true;
                }
            }
            ++start;
        }
    }

    template<typename T>
    void sortArray(T* arr, int size) {
        bool swapped = true;
        int start = 0;
        int end = size - 1;

        while (swapped) {
            swapped = false;

            for (int i = start; i < end; ++i) {
                if (arr[i] > arr[i + 1]) {
                    std::swap(arr[i], arr[i + 1]);
                    swapped = true;
                }
            }

            if (!swapped) break;

            swapped = false;
            --end;

            for (int i = end - 1; i >= start; --i) {
                if (arr[i] > arr[i + 1]) {
                    std::swap(arr[i], arr[i + 1]);
                    swapped = true;
                }
            }
            ++start;
        }
    }

    template<typename T>
    bool isSorted(const std::vector<T>& arr) const {
        if (arr.empty()) return true;  // Важная проверка!
        for (size_t i = 0; i < arr.size() - 1; ++i) {
            if (arr[i] > arr[i + 1]) {
                return false;
            }
        }
        return true;
    }

    template<typename T, typename Compare>
    bool isSorted(const std::vector<T>& arr, Compare comp) const {
        if (arr.empty()) return true;
        for (size_t i = 0; i < arr.size() - 1; ++i) {
            if (comp(arr[i + 1], arr[i])) {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    static void staticSort(std::vector<T>& arr) {
        CocktailSort sorter;
        sorter.sort(arr);
    }
};

#endif // COCKTAILSORT_H