
#include <gtest/gtest.h>
#include "set.h"
#include <sstream>

class MNTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

// Базовые тесты (1-30)
TEST_F(MNTest, Test01_EmptySet) {
    MN empty = MN::fromString("{}");
    EXPECT_TRUE(empty.isEmpty());
    EXPECT_EQ(empty.cardinality(), 0);
}

TEST_F(MNTest, Test02_SingleElement) {
    MN set = MN::fromString("{a}");
    EXPECT_FALSE(set.isEmpty());
    EXPECT_EQ(set.cardinality(), 1);
    EXPECT_TRUE(set.contains('a'));
}

TEST_F(MNTest, Test03_MultipleElements) {
    MN set = MN::fromString("{a,b,c}");
    EXPECT_EQ(set.cardinality(), 3);
    EXPECT_TRUE(set.contains('a'));
    EXPECT_TRUE(set.contains('b'));
    EXPECT_TRUE(set.contains('c'));
}

TEST_F(MNTest, Test04_Contains) {
    MN set = MN::fromString("{a,b,c}");
    EXPECT_TRUE(set.contains('a'));
    EXPECT_TRUE(set['b']);
    EXPECT_FALSE(set.contains('x'));
}

TEST_F(MNTest, Test05_AddElement) {
    MN set = MN::fromString("{a,b}");
    set.addElement('c');
    EXPECT_TRUE(set.contains('c'));
    EXPECT_EQ(set.cardinality(), 3);
}

TEST_F(MNTest, Test06_RemoveElement) {
    MN set = MN::fromString("{a,b,c}");
    EXPECT_TRUE(set.removeElement('b'));
    EXPECT_FALSE(set.contains('b'));
    EXPECT_EQ(set.cardinality(), 2);
}

TEST_F(MNTest, Test07_Clear) {
    MN set = MN::fromString("{a,b,c}");
    set.clear();
    EXPECT_TRUE(set.isEmpty());
    EXPECT_EQ(set.cardinality(), 0);
}

TEST_F(MNTest, Test08_DuplicatePrevention) {
    MN set = MN::fromString("{a,b}");
    set.addElement('a');
    set.addElement('b');
    EXPECT_EQ(set.cardinality(), 2);
}

TEST_F(MNTest, Test09_RemoveNonexistent) {
    MN set = MN::fromString("{a,b}");
    EXPECT_FALSE(set.removeElement('x'));
    EXPECT_EQ(set.cardinality(), 2);
}

TEST_F(MNTest, Test10_Union) {
    MN set1 = MN::fromString("{a,b}");
    MN set2 = MN::fromString("{b,c}");
    MN result = set1 + set2;

    EXPECT_TRUE(result.contains('a'));
    EXPECT_TRUE(result.contains('b'));
    EXPECT_TRUE(result.contains('c'));
    EXPECT_EQ(result.cardinality(), 3);
}

TEST_F(MNTest, Test11_Intersection) {
    MN set1 = MN::fromString("{a,b,c}");
    MN set2 = MN::fromString("{b,c,d}");
    MN result = set1 * set2;

    EXPECT_FALSE(result.contains('a'));
    EXPECT_TRUE(result.contains('b'));
    EXPECT_TRUE(result.contains('c'));
    EXPECT_FALSE(result.contains('d'));
    EXPECT_EQ(result.cardinality(), 2);
}

TEST_F(MNTest, Test12_Difference) {
    MN set1 = MN::fromString("{a,b,c}");
    MN set2 = MN::fromString("{b,c}");
    MN result = set1 - set2;

    EXPECT_TRUE(result.contains('a'));
    EXPECT_FALSE(result.contains('b'));
    EXPECT_FALSE(result.contains('c'));
    EXPECT_EQ(result.cardinality(), 1);
}

TEST_F(MNTest, Test13_UnionAssignment) {
    MN set1 = MN::fromString("{a,b}");
    MN set2 = MN::fromString("{b,c}");
    set1 += set2;

    EXPECT_TRUE(set1.contains('a'));
    EXPECT_TRUE(set1.contains('b'));
    EXPECT_TRUE(set1.contains('c'));
    EXPECT_EQ(set1.cardinality(), 3);
}

TEST_F(MNTest, Test14_Comparison) {
    MN set1 = MN::fromString("{a,b}");
    MN set2 = MN::fromString("{a,b}");
    MN set3 = MN::fromString("{a,c}");
    EXPECT_EQ(set1, set2);
    EXPECT_NE(set1, set3);
}

TEST_F(MNTest, Test15_StreamOperators) {
    MN original = MN::fromString("{a,b,c}");
    std::stringstream ss;
    ss << original;

    MN restored;
    ss >> restored;

    EXPECT_EQ(original, restored);
}

TEST_F(MNTest, Test16_ParseWithSpaces) {
    MN set1 = MN::fromString("{a,b}");
    MN set2 = MN::fromString("{ a , b }");
    EXPECT_EQ(set1, set2);
}

TEST_F(MNTest, Test17_InitializerList) {
    MN set{'a', 'b', 'c'};
    EXPECT_EQ(set.cardinality(), 3);
    EXPECT_TRUE(set.contains('a'));
    EXPECT_TRUE(set.contains('b'));
    EXPECT_TRUE(set.contains('c'));
}

TEST_F(MNTest, Test18_IntersectionAssignment) {
    MN set1 = MN::fromString("{a,b,c}");
    MN set2 = MN::fromString("{b,c,d}");
    set1 *= set2;

    EXPECT_FALSE(set1.contains('a'));
    EXPECT_TRUE(set1.contains('b'));
    EXPECT_TRUE(set1.contains('c'));
    EXPECT_FALSE(set1.contains('d'));
    EXPECT_EQ(set1.cardinality(), 2);
}

TEST_F(MNTest, Test19_DifferenceAssignment) {
    MN set1 = MN::fromString("{a,b,c}");
    MN set2 = MN::fromString("{b,c}");
    set1 -= set2;

    EXPECT_TRUE(set1.contains('a'));
    EXPECT_FALSE(set1.contains('b'));
    EXPECT_FALSE(set1.contains('c'));
    EXPECT_EQ(set1.cardinality(), 1);
}

TEST_F(MNTest, Test20_ComplexOperations) {
    MN set1 = MN::fromString("{a,b,c}");
    MN set2 = MN::fromString("{b,c,d}");
    MN set3 = MN::fromString("{c,d,e}");

    MN result = (set1 + set2) * set3;
    EXPECT_TRUE(result.contains('c'));
    EXPECT_TRUE(result.contains('d'));
    EXPECT_EQ(result.cardinality(), 2);
}

TEST_F(MNTest, Test21_EmptyStringThrows) {
    EXPECT_THROW(MN::fromString(""), std::runtime_error);
}

TEST_F(MNTest, Test22_InvalidElementThrows) {
    EXPECT_THROW(MN::fromString("{1}"), std::runtime_error);
}

TEST_F(MNTest, Test23_UnbalancedBracesThrows) {
    EXPECT_THROW(MN::fromString("{a,b"), std::runtime_error);
}

TEST_F(MNTest, Test24_ConstructorChar) {
    MN set('x');
    EXPECT_EQ(set.cardinality(), 1);
    EXPECT_TRUE(set.contains('x'));
}

TEST_F(MNTest, Test25_OutputFormat) {
    MN set = MN::fromString("{a,b}");
    std::ostringstream os;
    os << set;
    EXPECT_EQ(os.str(), "{a, b}");
}

TEST_F(MNTest, Test26_SelfOperations) {
    MN set = MN::fromString("{a,b}");

    // Объединение с самим собой
    MN union_self = set + set;
    EXPECT_EQ(union_self, set);

    // Пересечение с самим собой
    MN intersect_self = set * set;
    EXPECT_EQ(intersect_self, set);

    // Разность с самим собой
    MN diff_self = set - set;
    EXPECT_TRUE(diff_self.isEmpty());
}

TEST_F(MNTest, Test27_EmptySetOperations) {
    MN empty;
    MN non_empty = MN::fromString("{a,b}");

    // Объединение с пустым
    EXPECT_EQ(empty + non_empty, non_empty);
    EXPECT_EQ(non_empty + empty, non_empty);

    // Пересечение с пустым
    EXPECT_TRUE((empty * non_empty).isEmpty());
    EXPECT_TRUE((non_empty * empty).isEmpty());

    // Разность с пустым
    EXPECT_EQ(non_empty - empty, non_empty);
    EXPECT_TRUE((empty - non_empty).isEmpty());
}

TEST_F(MNTest, Test28_MultipleOperationsChain) {
    MN set1 = MN::fromString("{a,b}");
    MN set2 = MN::fromString("{b,c}");
    MN set3 = MN::fromString("{c,d}");

    // Цепочка операций
    MN result = set1 + set2 - set3;

    EXPECT_TRUE(result.contains('a'));
    EXPECT_TRUE(result.contains('b'));
    EXPECT_FALSE(result.contains('c'));
    EXPECT_FALSE(result.contains('d'));
    EXPECT_EQ(result.cardinality(), 2);
}

TEST_F(MNTest, Test29_LargeSetOperations) {
    MN set1 = MN::fromString("{a,b,c,d,e}");
    MN set2 = MN::fromString("{c,d,e,f,g}");

    MN union_result = set1 + set2;
    EXPECT_EQ(union_result.cardinality(), 7);

    MN intersect_result = set1 * set2;
    EXPECT_EQ(intersect_result.cardinality(), 3);
    EXPECT_TRUE(intersect_result.contains('c'));
    EXPECT_TRUE(intersect_result.contains('d'));
    EXPECT_TRUE(intersect_result.contains('e'));

    MN diff_result = set1 - set2;
    EXPECT_EQ(diff_result.cardinality(), 2);
    EXPECT_TRUE(diff_result.contains('a'));
    EXPECT_TRUE(diff_result.contains('b'));
}

TEST_F(MNTest, Test30_CommutativeProperty) {
    MN set1 = MN::fromString("{a,b}");
    MN set2 = MN::fromString("{c,d}");

    // Объединение коммутативно
    EXPECT_EQ(set1 + set2, set2 + set1);

    // Пересечение коммутативно
    EXPECT_EQ(set1 * set2, set2 * set1);

    // Но разность не коммутативна
    EXPECT_NE(set1 - set2, set2 - set1);
}

// Новые тесты для вложенных множеств (31-40)
TEST_F(MNTest, Test31_NestedSet) {
    MN set = MN::fromString("{a, {b, c}}");
    EXPECT_EQ(set.cardinality(), 2);

    // Проверяем, что содержит символ 'a'
    EXPECT_TRUE(set.contains('a'));

    // Проверяем, что содержит вложенное множество {b, c}
    MN subset = MN::fromString("{b, c}");
    EXPECT_TRUE(set.contains(subset));
}

TEST_F(MNTest, Test32_EmptySubset) {
    MN set = MN::fromString("{a, {}, {b}}");
    EXPECT_EQ(set.cardinality(), 3);

    // Проверяем пустое подмножество
    MN empty_set;
    EXPECT_TRUE(set.contains(empty_set));
}

TEST_F(MNTest, Test33_DeepNesting) {
    MN set = MN::fromString("{a, {b, {c, {d}}}}");
    EXPECT_EQ(set.cardinality(), 2);

    // Проверяем различные уровни вложенности
    MN level1 = MN::fromString("{b, {c, {d}}}");
    MN level2 = MN::fromString("{c, {d}}");
    MN level3 = MN::fromString("{d}");

    EXPECT_TRUE(set.contains(level1));
    EXPECT_TRUE(level1.contains(level2));
    EXPECT_TRUE(level2.contains(level3));
    EXPECT_TRUE(level3.contains('d'));
}

TEST_F(MNTest, Test34_NestedSetOperations) {
    MN set1 = MN::fromString("{a, {b, c}}");
    MN set2 = MN::fromString("{d, {b, c}}");

    // Объединение
    MN union_result = set1 + set2;
    EXPECT_EQ(union_result.cardinality(), 3); // Исправлено: 3 элемента (a, d, {b,c})
    EXPECT_TRUE(union_result.contains('a'));
    EXPECT_TRUE(union_result.contains('d'));

    MN subset = MN::fromString("{b, c}");
    EXPECT_TRUE(union_result.contains(subset));

    // Пересечение
    MN intersect_result = set1 * set2;
    EXPECT_EQ(intersect_result.cardinality(), 1);
    EXPECT_TRUE(intersect_result.contains(subset));
}

TEST_F(MNTest, Test35_ComplexNestedStructure) {
    MN set = MN::fromString("{a, b, {c, d}, {{e, f}, {g, h}}}");
    EXPECT_EQ(set.cardinality(), 4);

    // Проверяем элементы первого уровня
    EXPECT_TRUE(set.contains('a'));
    EXPECT_TRUE(set.contains('b'));

    // Проверяем подмножество первого уровня
    MN subset1 = MN::fromString("{c, d}");
    EXPECT_TRUE(set.contains(subset1));

    // Проверяем подмножество второго уровня
    MN subset2 = MN::fromString("{{e, f}, {g, h}}");
    EXPECT_TRUE(set.contains(subset2));

    // Проверяем элементы третьего уровня через subset2
    MN subset3 = MN::fromString("{e, f}");
    MN subset4 = MN::fromString("{g, h}");
    EXPECT_TRUE(subset2.contains(subset3));
    EXPECT_TRUE(subset2.contains(subset4));
}

TEST_F(MNTest, Test36_NestedSetComparison) {
    MN set1 = MN::fromString("{a, {b, c}}");
    MN set2 = MN::fromString("{a, {b, c}}");
    MN set3 = MN::fromString("{a, {b, d}}");

    EXPECT_EQ(set1, set2);
    EXPECT_NE(set1, set3);
}

TEST_F(MNTest, Test37_NestedSetToString) {
    MN set = MN::fromString("{a, {b, c}}");
    std::string str = set.toString();

    // Проверяем базовый формат
    EXPECT_TRUE(str.find('a') != std::string::npos);
    EXPECT_TRUE(str.find('b') != std::string::npos);
    EXPECT_TRUE(str.find('c') != std::string::npos);

    // Восстановление из строки
    MN restored = MN::fromString(str);
    EXPECT_EQ(set, restored);
}

TEST_F(MNTest, Test38_MixedOperations) {
    MN set1 = MN::fromString("{a, {b}}");
    MN set2 = MN::fromString("{c, {b}}");
    MN set3 = MN::fromString("{a, c}");

    // Сложные операции с вложенными множествами
    MN result = (set1 + set2) * set3;
    EXPECT_TRUE(result.contains('a'));
    EXPECT_TRUE(result.contains('c'));
    EXPECT_FALSE(result.contains('b')); // 'b' не на верхнем уровне
}

TEST_F(MNTest, Test39_AddRemoveNested) {
    MN set;

    // Добавляем вложенное множество
    MN subset = MN::fromString("{x, y}");
    auto subset_ptr = std::make_shared<MN>(subset);
    set.addElement(subset_ptr);

    EXPECT_TRUE(set.contains(subset));
    EXPECT_EQ(set.cardinality(), 1);

    // Удаляем вложенное множество
    EXPECT_TRUE(set.removeElement(subset_ptr));
    EXPECT_FALSE(set.contains(subset));
    EXPECT_TRUE(set.isEmpty());
}

TEST_F(MNTest, Test40_VeryComplexStructure) {
    // Очень сложная вложенная структура как в примере из задания
    MN set = MN::fromString("{a, b, c, {a, b}, {}, {a, {c}}}");
    EXPECT_EQ(set.cardinality(), 6);

    // Проверяем различные элементы
    EXPECT_TRUE(set.contains('a'));
    EXPECT_TRUE(set.contains('b'));
    EXPECT_TRUE(set.contains('c'));

    // Проверяем подмножества
    MN subset1 = MN::fromString("{a, b}");
    MN empty_set;
    MN subset2 = MN::fromString("{a, {c}}");

    EXPECT_TRUE(set.contains(subset1));
    EXPECT_TRUE(set.contains(empty_set));
    EXPECT_TRUE(set.contains(subset2));

    // Проверяем глубокую вложенность
    MN subset3 = MN::fromString("{c}");
    EXPECT_TRUE(subset2.contains(subset3));
}

TEST_F(MNTest, Test41_PowerSetEmpty) {
    MN empty;
    MN power_set = empty.powerSet();

    // Булеан пустого множества = { {} }
    EXPECT_EQ(power_set.cardinality(), 1);

    MN empty_subset;
    EXPECT_TRUE(power_set.contains(empty_subset));
}

TEST_F(MNTest, Test42_PowerSetSingleElement) {
    MN set = MN::fromString("{a}");
    MN power_set = set.powerSet();

    // Булеан {a} = { {}, {a} }
    EXPECT_EQ(power_set.cardinality(), 2);

    MN empty_set;
    MN single_set = MN::fromString("{a}");

    EXPECT_TRUE(power_set.contains(empty_set));
    EXPECT_TRUE(power_set.contains(single_set));
}

TEST_F(MNTest, Test43_PowerSetTwoElements) {
    MN set = MN::fromString("{a,b}");
    MN power_set = set.powerSet();

    // Булеан {a,b} = { {}, {a}, {b}, {a,b} }
    EXPECT_EQ(power_set.cardinality(), 4);

    EXPECT_TRUE(power_set.contains(MN::fromString("{}")));
    EXPECT_TRUE(power_set.contains(MN::fromString("{a}")));
    EXPECT_TRUE(power_set.contains(MN::fromString("{b}")));
    EXPECT_TRUE(power_set.contains(MN::fromString("{a,b}")));
}

TEST_F(MNTest, Test44_PowerSetCardinality) {
    // Мощность булеана = 2^n
    MN set1 = MN::fromString("{a,b,c}");
    EXPECT_EQ(set1.powerSet().cardinality(), 8); // 2^3 = 8

    MN set2 = MN::fromString("{a,b,c,d}");
    EXPECT_EQ(set2.powerSet().cardinality(), 16); // 2^4 = 16
}

TEST_F(MNTest, Test45_PowerSetWithNested) {
    MN set = MN::fromString("{a, {b}}");
    MN power_set = set.powerSet();

    // Должен содержать все подмножества
    EXPECT_TRUE(power_set.contains(MN::fromString("{}")));
    EXPECT_TRUE(power_set.contains(MN::fromString("{a}")));
    EXPECT_TRUE(power_set.contains(MN::fromString("{{b}}")));
    EXPECT_TRUE(power_set.contains(MN::fromString("{a, {b}}")));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
