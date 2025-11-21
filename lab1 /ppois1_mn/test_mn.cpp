#include <gtest/gtest.h>
#include "mn.h"
#include <sstream>

class MNTest : public ::testing::Test {
protected:
    void SetUp() override {}

    // Вспомогательные методы для тестирования
    void verifyParseAndOutput(const std::string& input, const std::string& expected_output = "") {
        MN parsed_set = MN::fromString(input);
        std::ostringstream output_stream;
        output_stream << parsed_set;

        if (!expected_output.empty()) {
            EXPECT_EQ(output_stream.str(), expected_output);
        }
    }

    void verifyParseThrows(const std::string& invalid_input) {
        EXPECT_THROW(MN::fromString(invalid_input), std::runtime_error);
    }
};

// Базовые тесты
TEST_F(MNTest, EmptySet) {
    MN empty_set_a = MN::fromString("{}");
    MN empty_set_b;
    EXPECT_EQ(empty_set_a, empty_set_b);
}

TEST_F(MNTest, SimpleElementSet) {
    MN set_a = MN::fromString("{a}");
    MN set_b = MN::fromString("{a}");
    EXPECT_EQ(set_a, set_b);
}

TEST_F(MNTest, MultipleElements) {
    EXPECT_EQ(MN::fromString("{a,b,c}"), MN::fromString("{a,b,c}"));
}

TEST_F(MNTest, SimpleNestedSet) {
    EXPECT_EQ(MN::fromString("{a,{b}}"), MN::fromString("{a,{b}}"));
}

TEST_F(MNTest, ComplexNestedSet) {
    EXPECT_EQ(MN::fromString("{a,{b,c},d}"), MN::fromString("{a,{b,c},d}"));
}

TEST_F(MNTest, SetsWithDifferentOrderAreNotEqual) {
    EXPECT_NE(MN::fromString("{a,b}"), MN::fromString("{b,a}"));
}

TEST_F(MNTest, SetsWithDifferentElementsAreNotEqual) {
    EXPECT_NE(MN::fromString("{a,{b}}"), MN::fromString("{a,{c}}"));
}

TEST_F(MNTest, AssignmentOperator) {
    MN original_set = MN::fromString("{a,{b,c}}");
    MN assigned_set;
    assigned_set = original_set;
    EXPECT_EQ(original_set, assigned_set);
}

TEST_F(MNTest, StreamOutputFormatting) {
    MN test_set = MN::fromString("{a,b}");
    std::ostringstream output_stream;
    output_stream << test_set;
    EXPECT_EQ(output_stream.str(), "{a, b}");
}

TEST_F(MNTest, ParseWithSpaces) {
    EXPECT_EQ(MN::fromString("{ a , b }"), MN::fromString("{a,b}"));
}

TEST_F(MNTest, SingleElementSet) {
    EXPECT_EQ(MN::fromString("{x}"), MN::fromString("{x}"));
}

TEST_F(MNTest, TripleNestedSet) {
    EXPECT_EQ(MN::fromString("{a,{b,{c}}}"), MN::fromString("{a,{b,{c}}}"));
}

TEST_F(MNTest, LargeStructure) {
    MN set_a = MN::fromString("{a,{b,c,{d}},e}");
    MN set_b = MN::fromString("{a,{b,c,{d}},e}");
    EXPECT_EQ(set_a, set_b);
}

TEST_F(MNTest, InputStreamOperator) {
    std::istringstream input_stream("{a,b}");
    MN parsed_set;
    input_stream >> parsed_set;
    EXPECT_EQ(parsed_set, MN::fromString("{a,b}"));
}

TEST_F(MNTest, SingleElementsNotEqual) {
    EXPECT_NE(MN('a'), MN('b'));
}

TEST_F(MNTest, EmptySetsEqual) {
    EXPECT_EQ(MN::fromString("{}"), MN::fromString("{}"));
}

TEST_F(MNTest, DeepNestedStructure) {
    MN deep_set_a = MN::fromString("{a,{b,{c,{d}}}}");
    MN deep_set_b = MN::fromString("{a,{b,{c,{d}}}}");
    EXPECT_EQ(deep_set_a, deep_set_b);
}

TEST_F(MNTest, InvalidElementThrowsError) {
    verifyParseThrows("{1}");
}

TEST_F(MNTest, MissingBracketThrowsError) {
    verifyParseThrows("{a,b");
}

TEST_F(MNTest, DifferentNestedElementsNotEqual) {
    EXPECT_NE(MN::fromString("{a,{b,c}}"), MN::fromString("{a,{b,d}}"));
}

TEST_F(MNTest, CompareTwoEmptySets) {
    EXPECT_EQ(MN(), MN());
}

TEST_F(MNTest, CompareSingleElements) {
    EXPECT_EQ(MN('x'), MN('x'));
}

TEST_F(MNTest, EmptySetNotEqualToElement) {
    EXPECT_NE(MN(), MN('a'));
}

TEST_F(MNTest, DifferentSizedSetsNotEqual) {
    EXPECT_NE(MN::fromString("{a,b}"), MN::fromString("{a}"));
}

TEST_F(MNTest, ParseWithMultipleSpaces) {
    EXPECT_EQ(MN::fromString("{   a   ,   b   }"), MN::fromString("{a,b}"));
}

TEST_F(MNTest, ComplexSpacing) {
    EXPECT_EQ(MN::fromString("{ a , { b , c } }"), MN::fromString("{a,{b,c}}"));
}

TEST_F(MNTest, ThreeElementsSet) {
    EXPECT_EQ(MN::fromString("{a,b,c}"), MN::fromString("{a,b,c}"));
}

TEST_F(MNTest, TwoEmptyNestedSets) {
    EXPECT_EQ(MN::fromString("{{},{}}"), MN::fromString("{{},{}}"));
}

TEST_F(MNTest, DifferentOrderInNestedSet) {
    EXPECT_NE(MN::fromString("{a,{b,c}}"), MN::fromString("{a,{c,b}}"));
}

TEST_F(MNTest, SelfAssignment) {
    MN test_set = MN::fromString("{a}");
    test_set = test_set;
    EXPECT_EQ(test_set, MN::fromString("{a}"));
}

TEST_F(MNTest, StreamInputWithSpaces) {
    std::istringstream input_stream("  { a , b }  ");
    MN parsed_set;
    input_stream >> parsed_set;
    EXPECT_EQ(parsed_set, MN::fromString("{a,b}"));
}

TEST_F(MNTest, EmptyInputStreamThrows) {
    std::istringstream empty_stream;
    MN test_set;
    EXPECT_THROW(empty_stream >> test_set, std::runtime_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}