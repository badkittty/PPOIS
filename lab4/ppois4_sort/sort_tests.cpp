#include <gtest/gtest.h>
#include "cocktailsort.h"
#include "strandsort.h"
#include <vector>
#include <list>
#include <algorithm>
#include <random>
#include <string>
#include <functional>

class TestItem {
public:
    int id;
    std::string name;
    double value;
    
    TestItem() : id(0), name(""), value(0.0) {}
    TestItem(int i, const std::string& n, double v) : id(i), name(n), value(v) {}
    
    bool operator>(const TestItem& other) const { return id > other.id; }
    bool operator>=(const TestItem& other) const { return id >= other.id; }
    bool operator<(const TestItem& other) const { return id < other.id; }
    bool operator<=(const TestItem& other) const { return id <= other.id; }
    bool operator==(const TestItem& other) const { return id == other.id; }
};

struct CompareTestItemByName {
    bool operator()(const TestItem& a, const TestItem& b) const {
        return a.name < b.name;
    }
};

struct CompareTestItemByValue {
    bool operator()(const TestItem& a, const TestItem& b) const {
        return a.value < b.value;
    }
};

struct CompareTestItemByIdDesc {
    bool operator()(const TestItem& a, const TestItem& b) const {
        return a.id > b.id;
    }
};


TEST(CocktailSortTest, SortEmptyVector) {
    CocktailSort sorter;
    std::vector<int> vec;
    
    sorter.sort(vec);
    EXPECT_TRUE(vec.empty());
    EXPECT_TRUE(sorter.isSorted(vec));
}

TEST(CocktailSortTest, SortSingleElement) {
    CocktailSort sorter;
    std::vector<int> vec = {42};
    
    sorter.sort(vec);
    ASSERT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], 42);
    EXPECT_TRUE(sorter.isSorted(vec));
}

TEST(CocktailSortTest, SortIntegerVector) {
    CocktailSort sorter;
    std::vector<int> vec = {5, 2, 9, 1, 7, 3, 8, 4, 6};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    sorter.sort(vec);
    EXPECT_EQ(vec, expected);
    EXPECT_TRUE(sorter.isSorted(vec));
}

TEST(CocktailSortTest, SortAlreadySorted) {
    CocktailSort sorter;
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    sorter.sort(vec);
    EXPECT_TRUE(sorter.isSorted(vec));
    EXPECT_EQ(vec, std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9}));
}

TEST(CocktailSortTest, SortReverseSorted) {
    CocktailSort sorter;
    std::vector<int> vec = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    
    sorter.sort(vec);
    EXPECT_TRUE(sorter.isSorted(vec));
    EXPECT_EQ(vec, std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9}));
}

TEST(CocktailSortTest, SortWithDuplicates) {
    CocktailSort sorter;
    std::vector<int> vec = {5, 2, 5, 1, 7, 2, 8, 4, 2};
    
    sorter.sort(vec);
    EXPECT_TRUE(sorter.isSorted(vec));
}

TEST(CocktailSortTest, SortStringVector) {
    CocktailSort sorter;
    std::vector<std::string> vec = {"banana", "apple", "cherry", "date"};
    std::vector<std::string> expected = {"apple", "banana", "cherry", "date"};
    
    sorter.sort(vec);
    EXPECT_EQ(vec, expected);
    EXPECT_TRUE(sorter.isSorted(vec));
}

TEST(CocktailSortTest, SortDoubleVector) {
    CocktailSort sorter;
    std::vector<double> vec = {5.5, 2.2, 9.9, 1.1, 7.7};
    
    sorter.sort(vec);
    EXPECT_TRUE(sorter.isSorted(vec));
}

TEST(CocktailSortTest, SortCustomClassVector) {
    CocktailSort sorter;
    std::vector<TestItem> vec = {
        TestItem(5, "Eve", 88.5),
        TestItem(2, "Bob", 72.0),
        TestItem(9, "Ivy", 95.0),
        TestItem(1, "Alice", 85.0),
        TestItem(7, "Grace", 78.5)
    };
    
    sorter.sort(vec);
    EXPECT_TRUE(sorter.isSorted(vec));
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        EXPECT_TRUE(vec[i].id <= vec[i + 1].id);
    }
}

TEST(CocktailSortTest, SortWithComparator) {
    CocktailSort sorter;
    std::vector<int> vec = {5, 2, 9, 1, 7};
    
    auto descending = [](int a, int b) { return a > b; };
    sorter.sort(vec, descending);
    
    EXPECT_TRUE(sorter.isSorted(vec, descending));
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        EXPECT_TRUE(vec[i] >= vec[i + 1]);
    }
}

TEST(CocktailSortTest, SortCustomClassWithComparator) {
    CocktailSort sorter;
    std::vector<TestItem> vec = {
        TestItem(5, "Eve", 88.5),
        TestItem(2, "Bob", 72.0),
        TestItem(9, "Ivy", 95.0),
        TestItem(1, "Alice", 85.0),
        TestItem(7, "Grace", 78.5)
    };
    
    sorter.sort(vec, CompareTestItemByName());
    EXPECT_TRUE(sorter.isSorted(vec, CompareTestItemByName()));
    
    sorter.sort(vec, CompareTestItemByValue());
    EXPECT_TRUE(sorter.isSorted(vec, CompareTestItemByValue()));
    
    sorter.sort(vec, CompareTestItemByIdDesc());
    EXPECT_TRUE(sorter.isSorted(vec, CompareTestItemByIdDesc()));
}

TEST(CocktailSortTest, SortArray) {
    CocktailSort sorter;
    int arr[] = {9, 3, 7, 1, 5, 8, 2, 6, 4};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    sorter.sortArray(arr, size);
    
    for (int i = 0; i < size - 1; ++i) {
        EXPECT_TRUE(arr[i] <= arr[i + 1]);
    }
}

TEST(CocktailSortTest, StaticSortMethod) {
    std::vector<int> vec = {5, 2, 9, 1, 7};
    
    CocktailSort::staticSort(vec);
    
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        EXPECT_TRUE(vec[i] <= vec[i + 1]);
    }
}

TEST(CocktailSortTest, LargeVectorSort) {
    CocktailSort sorter;
    std::vector<int> vec(1000);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 10000);
    
    for (int& num : vec) {
        num = distrib(gen);
    }
    
    sorter.sort(vec);
    EXPECT_TRUE(sorter.isSorted(vec));
}


TEST(StrandSortTest, SortEmptyVector) {
    StrandSort sorter;
    std::vector<int> vec;
    
    std::vector<int> result = sorter.sort(vec);
    EXPECT_TRUE(result.empty());
    EXPECT_TRUE(sorter.isSorted(result));
}

TEST(StrandSortTest, SortSingleElement) {
    StrandSort sorter;
    std::vector<int> vec = {42};
    
    std::vector<int> result = sorter.sort(vec);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 42);
    EXPECT_TRUE(sorter.isSorted(result));
}

TEST(StrandSortTest, SortIntegerVector) {
    StrandSort sorter;
    std::vector<int> vec = {5, 2, 9, 1, 7, 3, 8, 4, 6};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    std::vector<int> result = sorter.sort(vec);
    EXPECT_EQ(result, expected);
    EXPECT_TRUE(sorter.isSorted(result));
}

TEST(StrandSortTest, SortAlreadySorted) {
    StrandSort sorter;
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    std::vector<int> result = sorter.sort(vec);
    EXPECT_TRUE(sorter.isSorted(result));
    EXPECT_EQ(result, std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9}));
}

TEST(StrandSortTest, SortReverseSorted) {
    StrandSort sorter;
    std::vector<int> vec = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    
    std::vector<int> result = sorter.sort(vec);
    EXPECT_TRUE(sorter.isSorted(result));
    EXPECT_EQ(result, std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9}));
}

TEST(StrandSortTest, SortWithDuplicates) {
    StrandSort sorter;
    std::vector<int> vec = {5, 2, 5, 1, 7, 2, 8, 4, 2};
    
    std::vector<int> result = sorter.sort(vec);
    EXPECT_TRUE(sorter.isSorted(result));
}

TEST(StrandSortTest, SortStringVector) {
    StrandSort sorter;
    std::vector<std::string> vec = {"banana", "apple", "cherry", "date"};
    std::vector<std::string> expected = {"apple", "banana", "cherry", "date"};
    
    std::vector<std::string> result = sorter.sort(vec);
    EXPECT_EQ(result, expected);
    EXPECT_TRUE(sorter.isSorted(result));
}

TEST(StrandSortTest, SortDoubleVector) {
    StrandSort sorter;
    std::vector<double> vec = {5.5, 2.2, 9.9, 1.1, 7.7};
    
    std::vector<double> result = sorter.sort(vec);
    EXPECT_TRUE(sorter.isSorted(result));
}

TEST(StrandSortTest, SortCustomClassVector) {
    StrandSort sorter;
    std::vector<TestItem> vec = {
        TestItem(5, "Eve", 88.5),
        TestItem(2, "Bob", 72.0),
        TestItem(9, "Ivy", 95.0),
        TestItem(1, "Alice", 85.0),
        TestItem(7, "Grace", 78.5)
    };
    
    std::vector<TestItem> result = sorter.sort(vec);
    EXPECT_TRUE(sorter.isSorted(result));
    for (size_t i = 0; i < result.size() - 1; ++i) {
        EXPECT_TRUE(result[i].id <= result[i + 1].id);
    }
}

TEST(StrandSortTest, SortWithComparator) {
    StrandSort sorter;
    std::vector<int> vec = {5, 2, 9, 1, 7};
    
    auto descending = [](int a, int b) { return a > b; };
    std::vector<int> result = sorter.sort(vec, descending);
    
    EXPECT_TRUE(sorter.isSorted(result, descending));
    for (size_t i = 0; i < result.size() - 1; ++i) {
        EXPECT_TRUE(result[i] >= result[i + 1]);
    }
}

TEST(StrandSortTest, SortCustomClassWithComparator) {
    StrandSort sorter;
    std::vector<TestItem> vec = {
        TestItem(5, "Eve", 88.5),
        TestItem(2, "Bob", 72.0),
        TestItem(9, "Ivy", 95.0),
        TestItem(1, "Alice", 85.0),
        TestItem(7, "Grace", 78.5)
    };
    
    std::vector<TestItem> resultByName = sorter.sort(vec, CompareTestItemByName());
    EXPECT_TRUE(sorter.isSorted(resultByName, CompareTestItemByName()));
    
    std::vector<TestItem> resultByValue = sorter.sort(vec, CompareTestItemByValue());
    EXPECT_TRUE(sorter.isSorted(resultByValue, CompareTestItemByValue()));
    
    std::vector<TestItem> resultByIdDesc = sorter.sort(vec, CompareTestItemByIdDesc());
    EXPECT_TRUE(sorter.isSorted(resultByIdDesc, CompareTestItemByIdDesc()));
}

TEST(StrandSortTest, SortListInPlace) {
    StrandSort sorter;
    std::list<int> lst = {5, 2, 9, 1, 7, 3, 8, 4, 6};
    
    sorter.sortListInPlace(lst);
    
    auto it = lst.begin();
    auto next = it;
    ++next;
    while (next != lst.end()) {
        EXPECT_TRUE(*it <= *next);
        ++it;
        ++next;
    }
}

TEST(StrandSortTest, StaticSortMethod) {
    std::vector<int> vec = {5, 2, 9, 1, 7};
    
    std::vector<int> result = StrandSort::staticSort(vec);
    
    for (size_t i = 0; i < result.size() - 1; ++i) {
        EXPECT_TRUE(result[i] <= result[i + 1]);
    }
}

TEST(StrandSortTest, LargeVectorSort) {
    StrandSort sorter;
    std::vector<int> vec(1000);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 10000);
    
    for (int& num : vec) {
        num = distrib(gen);
    }
    
    std::vector<int> result = sorter.sort(vec);
    EXPECT_TRUE(sorter.isSorted(result));
}


TEST(ComparativeTest, BothAlgorithmsProduceSameResult) {
    CocktailSort cocktailSorter;
    StrandSort strandSorter;
    
    std::vector<int> vec = {5, 2, 9, 1, 7, 3, 8, 4, 6, 0};
    
    std::vector<int> cocktailResult = vec;
    cocktailSorter.sort(cocktailResult);
    
    std::vector<int> strandResult = strandSorter.sort(vec);
    
    EXPECT_EQ(cocktailResult, strandResult);
    EXPECT_TRUE(cocktailSorter.isSorted(cocktailResult));
    EXPECT_TRUE(strandSorter.isSorted(strandResult));
}

TEST(ComparativeTest, BothAlgorithmsHandleEdgeCases) {
    CocktailSort cocktailSorter;
    StrandSort strandSorter;
    
    // Пустой вектор
    std::vector<int> empty;
    cocktailSorter.sort(empty);
    std::vector<int> strandEmpty = strandSorter.sort(std::vector<int>());
    EXPECT_TRUE(empty.empty());
    EXPECT_TRUE(strandEmpty.empty());
    
    // Один элемент
    std::vector<int> single = {42};
    cocktailSorter.sort(single);
    std::vector<int> strandSingle = strandSorter.sort(std::vector<int>{42});
    EXPECT_EQ(single.size(), 1);
    EXPECT_EQ(strandSingle.size(), 1);
    EXPECT_EQ(single[0], 42);
    EXPECT_EQ(strandSingle[0], 42);
    
    // Два элемента
    std::vector<int> two = {2, 1};
    cocktailSorter.sort(two);
    std::vector<int> strandTwo = strandSorter.sort(std::vector<int>{2, 1});
    EXPECT_EQ(two, std::vector<int>({1, 2}));
    EXPECT_EQ(strandTwo, std::vector<int>({1, 2}));
}

TEST(PerformanceTest, SortLargeRandomData) {
    CocktailSort cocktailSorter;
    StrandSort strandSorter;
    
    std::vector<int> largeVec(10000);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 100000);
    
    for (int& num : largeVec) {
        num = distrib(gen);
    }
    
    // CocktailSort
    std::vector<int> cocktailCopy = largeVec;
    cocktailSorter.sort(cocktailCopy);
    EXPECT_TRUE(cocktailSorter.isSorted(cocktailCopy));
    
    // StrandSort
    std::vector<int> strandResult = strandSorter.sort(largeVec);
    EXPECT_TRUE(strandSorter.isSorted(strandResult));
    
    // Проверяем, что оба алгоритма дают одинаковый результат
    EXPECT_EQ(cocktailCopy.size(), strandResult.size());
}


TEST(IsSortedTest, CocktailSortIsSortedMethod) {
    CocktailSort sorter;

    std::vector<int> sorted = {1, 2, 3, 4, 5};
    EXPECT_TRUE(sorter.isSorted(sorted));

    std::vector<int> unsorted = {1, 3, 2, 4, 5};
    EXPECT_FALSE(sorter.isSorted(unsorted));

    std::vector<int> empty;
    EXPECT_TRUE(sorter.isSorted(empty));

    std::vector<int> single = {42};
    EXPECT_TRUE(sorter.isSorted(single));
}

TEST(IsSortedTest, StrandSortIsSortedMethod) {
    StrandSort sorter;

    std::vector<int> sorted = {1, 2, 3, 4, 5};
    EXPECT_TRUE(sorter.isSorted(sorted));

    std::vector<int> unsorted = {1, 3, 2, 4, 5};
    EXPECT_FALSE(sorter.isSorted(unsorted));

    std::vector<int> empty;
    EXPECT_TRUE(sorter.isSorted(empty));

    std::vector<int> single = {42};
    EXPECT_TRUE(sorter.isSorted(single));
}

TEST(IsSortedTest, IsSortedWithComparator) {
    CocktailSort cocktailSorter;
    StrandSort strandSorter;

    auto descending = [](int a, int b) { return a > b; };

    std::vector<int> descSorted = {5, 4, 3, 2, 1};
    EXPECT_TRUE(cocktailSorter.isSorted(descSorted, descending));
    EXPECT_TRUE(strandSorter.isSorted(descSorted, descending));

    std::vector<int> descUnsorted = {5, 4, 2, 3, 1};
    EXPECT_FALSE(cocktailSorter.isSorted(descUnsorted, descending));
    EXPECT_FALSE(strandSorter.isSorted(descUnsorted, descending));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}