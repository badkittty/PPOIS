#ifndef STRANDSORT_H
#define STRANDSORT_H

#include <vector>
#include <list>
#include <algorithm>
#include <functional>

class StrandSort {
public:
    StrandSort() = default;

    template<typename T>
    std::vector<T> sort(const std::vector<T>& input) {
        return sort(input, std::less<T>());
    }

    template<typename T, typename Compare>
    std::vector<T> sort(const std::vector<T>& input, Compare comp) {
        if (input.empty()) return input;
        
        std::list<T> inputList(input.begin(), input.end());
        std::list<T> result = sortList(inputList, comp);
        return std::vector<T>(result.begin(), result.end());
    }

    template<typename T>
    void sortListInPlace(std::list<T>& input) {
        if (input.empty()) return;
        
        std::list<T> result;
        
        while (!input.empty()) {
            std::list<T> sortedStrand;
            sortedStrand.push_back(input.front());
            input.pop_front();

            auto it = input.begin();
            while (it != input.end()) {
                if (!(*it < sortedStrand.back())) {
                    sortedStrand.push_back(*it);
                    it = input.erase(it);
                } else {
                    ++it;
                }
            }

            if (result.empty()) {
                result = std::move(sortedStrand);
            } else {
                std::list<T> merged;
                auto it1 = result.begin();
                auto it2 = sortedStrand.begin();
                
                while (it1 != result.end() && it2 != sortedStrand.end()) {
                    if (*it1 <= *it2) {
                        merged.push_back(*it1);
                        ++it1;
                    } else {
                        merged.push_back(*it2);
                        ++it2;
                    }
                }

                while (it1 != result.end()) {
                    merged.push_back(*it1);
                    ++it1;
                }
                
                while (it2 != sortedStrand.end()) {
                    merged.push_back(*it2);
                    ++it2;
                }
                
                result = std::move(merged);
            }
        }
        
        input = std::move(result);
    }

    template<typename T>
    bool isSorted(const std::vector<T>& arr) const {
        if (arr.empty()) return true;
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
    static std::vector<T> staticSort(const std::vector<T>& input) {
        StrandSort sorter;
        return sorter.sort(input);
    }

private:
    template<typename T, typename Compare>
    std::list<T> sortList(std::list<T>& input, Compare comp) {
        if (input.empty()) return input;

        std::list<T> result;
        
        while (!input.empty()) {
            std::list<T> sortedStrand;
            sortedStrand.push_back(input.front());
            input.pop_front();

            auto it = input.begin();
            while (it != input.end()) {
                if (!comp(*it, sortedStrand.back())) { // it >= back
                    sortedStrand.push_back(*it);
                    it = input.erase(it);
                } else {
                    ++it;
                }
            }

            if (result.empty()) {
                result = std::move(sortedStrand);
            } else {
                std::list<T> merged;
                auto it1 = result.begin();
                auto it2 = sortedStrand.begin();
                
                while (it1 != result.end() && it2 != sortedStrand.end()) {
                    if (comp(*it1, *it2) || !comp(*it2, *it1)) { // it1 <= it2
                        merged.push_back(*it1);
                        ++it1;
                    } else {
                        merged.push_back(*it2);
                        ++it2;
                    }
                }

                while (it1 != result.end()) {
                    merged.push_back(*it1);
                    ++it1;
                }
                
                while (it2 != sortedStrand.end()) {
                    merged.push_back(*it2);
                    ++it2;
                }
                
                result = std::move(merged);
            }
        }
        
        return result;
    }
};

#endif // STRANDSORT_H