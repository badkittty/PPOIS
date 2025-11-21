#include <gtest/gtest.h>
#include "d.h"
#include <fstream>
#include <filesystem>

namespace TestConstants {
    constexpr int SMALL_TEST_SIZE = 5;
    constexpr int MEDIUM_TEST_SIZE = 10;
    constexpr int LARGE_TEST_SIZE = 20;
    constexpr int STRESS_TEST_SIZE = 15;
}

class DictionaryTest : public ::testing::Test {
protected:
    void SetUp() override {
        dict.addWord("apple", "яблоко");
        dict.addWord("book", "книга");
        dict.addWord("cat", "кот");
    }

    void TearDown() override {
        // Удаляем тестовые файлы после каждого теста
        std::remove("test_dict.txt");
        std::remove("empty_test.txt");
    }

    D dict;
};

// 1. Добавление нового слова
TEST_F(DictionaryTest, AddNewWord) {
    dict.addWord("dog", "собака");
    EXPECT_EQ(dict.getTranslation("dog"), "собака");
    EXPECT_EQ(dict.getWordCount(), 4);
}

// 2. Добавление дубликата слова обновляет перевод
TEST_F(DictionaryTest, AddDuplicateWordUpdatesTranslation) {
    dict.addWord("apple", "новое яблоко");
    EXPECT_EQ(dict.getTranslation("apple"), "новое яблоко");
    EXPECT_EQ(dict.getWordCount(), 3);
}

// 3. Получение перевода существующего слова
TEST_F(DictionaryTest, GetExistingTranslation) {
    EXPECT_EQ(dict.getTranslation("apple"), "яблоко");
    EXPECT_EQ(dict.getTranslation("book"), "книга");
    EXPECT_EQ(dict.getTranslation("cat"), "кот");
}

// 4. Получение перевода несуществующего слова
TEST_F(DictionaryTest, GetNonExistentTranslation) {
    EXPECT_TRUE(dict.getTranslation("unknown").empty());
}

// 5. Проверка наличия существующего слова
TEST_F(DictionaryTest, ContainsExistingWord) {
    EXPECT_TRUE(dict.contains("apple"));
    EXPECT_TRUE(dict.contains("book"));
    EXPECT_TRUE(dict.contains("cat"));
}

// 6. Проверка отсутствия несуществующего слова
TEST_F(DictionaryTest, ContainsNonExistentWord) {
    EXPECT_FALSE(dict.contains("unknown"));
}

// 7. Подсчет количества слов
TEST_F(DictionaryTest, WordCount) {
    EXPECT_EQ(dict.getWordCount(), 3);
}

// 8. Проверка пустого словаря
TEST_F(DictionaryTest, IsEmptyOnNewDictionary) {
    D emptyDict;
    EXPECT_TRUE(emptyDict.isEmpty());
}

// 9. Проверка непустого словаря
TEST_F(DictionaryTest, IsNotEmptyOnFilledDictionary) {
    EXPECT_FALSE(dict.isEmpty());
}

// 10. Чувствительность к регистру
TEST_F(DictionaryTest, CaseSensitiveWords) {
    dict.addWord("Apple", "большое яблоко");
    EXPECT_TRUE(dict.contains("Apple"));
    EXPECT_TRUE(dict.contains("apple"));
    EXPECT_NE(dict.getTranslation("Apple"), dict.getTranslation("apple"));
}

// 11. Слова с пробелами
TEST_F(DictionaryTest, WordsWithSpaces) {
    dict.addWord("hello world", "привет мир");
    EXPECT_EQ(dict.getTranslation("hello world"), "привет мир");
}

// 12. Специальные символы в словах
TEST_F(DictionaryTest, SpecialCharactersInWords) {
    dict.addWord("e-mail", "электронная почта");
    dict.addWord("user_name", "имя пользователя");
    EXPECT_EQ(dict.getTranslation("e-mail"), "электронная почта");
    EXPECT_EQ(dict.getTranslation("user_name"), "имя пользователя");
}

// 13. Операции с пустым словарем
TEST_F(DictionaryTest, EmptyDictionaryOperations) {
    D emptyDict;
    EXPECT_TRUE(emptyDict.getTranslation("test").empty());
    EXPECT_FALSE(emptyDict.contains("test"));
    EXPECT_TRUE(emptyDict.isEmpty());
}

// 14. Множественные операции добавления
TEST_F(DictionaryTest, MultipleAddOperations) {
    D multiDict;

    for (int i = 0; i < TestConstants::SMALL_TEST_SIZE; ++i) {
        multiDict.addWord("word" + std::to_string(i), "перевод" + std::to_string(i));
    }

    EXPECT_EQ(multiDict.getWordCount(), TestConstants::SMALL_TEST_SIZE);
    EXPECT_TRUE(multiDict.contains("word3"));
    EXPECT_EQ(multiDict.getTranslation("word2"), "перевод2");
}

// 15. Точность подсчета слов
TEST_F(DictionaryTest, WordCountAccuracy) {
    size_t startCount = dict.getWordCount();
    dict.addWord("new", "новый");
    EXPECT_EQ(dict.getWordCount(), startCount + 1);
}

// 16. Слова с одинаковым префиксом
TEST_F(DictionaryTest, WordsWithSamePrefix) {
    dict.addWord("app", "приложение");
    dict.addWord("application", "заявка");

    EXPECT_EQ(dict.getTranslation("app"), "приложение");
    EXPECT_EQ(dict.getTranslation("application"), "заявка");
    EXPECT_EQ(dict.getTranslation("apple"), "яблоко");
}

// 17. Слова в разных регистрах
TEST_F(DictionaryTest, DifferentCaseWordsStoredSeparately) {
    dict.addWord("Computer", "компьютер");
    dict.addWord("COMPUTER", "БОЛЬШОЙ КОМПЬЮТЕР");

    EXPECT_EQ(dict.getTranslation("Computer"), "компьютер");
    EXPECT_EQ(dict.getTranslation("COMPUTER"), "БОЛЬШОЙ КОМПЬЮТЕР");
}

// 18. Большое количество слов
TEST_F(DictionaryTest, LargeNumberOfWords) {
    D largeDict;
    const int totalWords = TestConstants::LARGE_TEST_SIZE;

    for (int i = 0; i < totalWords; ++i) {
        largeDict.addWord("key" + std::to_string(i), "значение" + std::to_string(i));
    }

    EXPECT_EQ(largeDict.getWordCount(), totalWords);
    EXPECT_EQ(largeDict.getTranslation("key10"), "значение10");
}

// 19. Сложные операции добавления
TEST_F(DictionaryTest, ComplexAddOperations) {
    D complexDict;

    for (int i = 0; i < TestConstants::MEDIUM_TEST_SIZE; ++i) {
        complexDict.addWord("word" + std::to_string(i), "перевод" + std::to_string(i));
    }

    for (int i = TestConstants::MEDIUM_TEST_SIZE; i < TestConstants::STRESS_TEST_SIZE; ++i) {
        complexDict.addWord("word" + std::to_string(i), "новый_перевод" + std::to_string(i));
    }

    EXPECT_EQ(complexDict.getWordCount(), TestConstants::STRESS_TEST_SIZE);
    EXPECT_TRUE(complexDict.contains("word12"));
}

// 20. Проверка сложного сценария
TEST_F(DictionaryTest, VerifyComplexScenario) {
    D scenarioDict;

    scenarioDict.addWord("one", "один");
    scenarioDict.addWord("two", "два");
    scenarioDict.addWord("three", "три");
    scenarioDict.addWord("four", "четыре");

    EXPECT_TRUE(scenarioDict.contains("one"));
    EXPECT_EQ(scenarioDict.getTranslation("two"), "два");
    EXPECT_EQ(scenarioDict.getTranslation("three"), "три");
    EXPECT_EQ(scenarioDict.getTranslation("four"), "четыре");
    EXPECT_EQ(scenarioDict.getWordCount(), 4);
}

// 21. Длинные слова
TEST_F(DictionaryTest, LongWordSupport) {
    std::string longWord(30, 'x');
    std::string longTranslation(30, 'y');

    dict.addWord(longWord, longTranslation);
    EXPECT_EQ(dict.getTranslation(longWord), longTranslation);
}

// 22. Смешанные языки
TEST_F(DictionaryTest, MixedLanguages) {
    dict.addWord("computer", "компьютер");
    dict.addWord("программа", "program");

    EXPECT_EQ(dict.getTranslation("computer"), "компьютер");
    EXPECT_EQ(dict.getTranslation("программа"), "program");
}

// 23. Стресс-тест добавления
TEST_F(DictionaryTest, StressTestAdd) {
    D stressDict;

    for (int i = 0; i < TestConstants::STRESS_TEST_SIZE; ++i) {
        stressDict.addWord("key" + std::to_string(i), "value" + std::to_string(i));
    }

    EXPECT_EQ(stressDict.getWordCount(), TestConstants::STRESS_TEST_SIZE);
    EXPECT_EQ(stressDict.getTranslation("key7"), "value7");
}

// 24. Проверка после множественных добавлений
TEST_F(DictionaryTest, CheckAfterMultipleAdditions) {
    D testDict;

    testDict.addWord("a", "а");
    testDict.addWord("b", "б");
    testDict.addWord("c", "в");
    testDict.addWord("d", "г");

    EXPECT_EQ(testDict.getWordCount(), 4);
    EXPECT_TRUE(testDict.contains("c"));
    EXPECT_EQ(testDict.getTranslation("b"), "б");
}

// 25. Добавление слов с цифрами
TEST_F(DictionaryTest, WordsWithNumbers) {
    dict.addWord("word1", "слово1");
    dict.addWord("word2", "слово2");

    EXPECT_EQ(dict.getTranslation("word1"), "слово1");
    EXPECT_EQ(dict.getTranslation("word2"), "слово2");
}

// 26. Проверка пустых строк
TEST_F(DictionaryTest, EmptyStringCheck) {
    D emptyDict;
    EXPECT_TRUE(emptyDict.getTranslation("").empty());
    EXPECT_FALSE(emptyDict.contains(""));
}

// 27. Добавление и проверка специальных символов
TEST_F(DictionaryTest, SpecialCharacters) {
    dict.addWord("c++", "си плюс плюс");
    dict.addWord("c#", "си шарп");

    EXPECT_EQ(dict.getTranslation("c++"), "си плюс плюс");
    EXPECT_EQ(dict.getTranslation("c#"), "си шарп");
}

// 28. Многократное добавление одного слова
TEST_F(DictionaryTest, MultipleAddSameWord) {
    dict.addWord("test", "тест1");
    dict.addWord("test", "тест2");
    dict.addWord("test", "тест3");

    EXPECT_EQ(dict.getTranslation("test"), "тест3");
    EXPECT_EQ(dict.getWordCount(), 4); // apple, book, cat, test
}

// 29. Проверка порядка добавления
TEST_F(DictionaryTest, AdditionOrder) {
    D orderDict;

    orderDict.addWord("z", "з");
    orderDict.addWord("a", "а");
    orderDict.addWord("m", "м");

    EXPECT_TRUE(orderDict.contains("a"));
    EXPECT_TRUE(orderDict.contains("m"));
    EXPECT_TRUE(orderDict.contains("z"));
    EXPECT_EQ(orderDict.getWordCount(), 3);
}

// 30. Финальный комплексный тест
TEST_F(DictionaryTest, FinalComplexTest) {
    D finalDict;

    // Добавляем разнообразные слова
    finalDict.addWord("hello", "привет");
    finalDict.addWord("world", "мир");
    finalDict.addWord("test", "тест");
    finalDict.addWord("example", "пример");
    finalDict.addWord("data", "данные");

    // Проверяем все добавленные слова
    EXPECT_EQ(finalDict.getTranslation("hello"), "привет");
    EXPECT_EQ(finalDict.getTranslation("world"), "мир");
    EXPECT_EQ(finalDict.getTranslation("test"), "тест");
    EXPECT_EQ(finalDict.getTranslation("example"), "пример");
    EXPECT_EQ(finalDict.getTranslation("data"), "данные");

    // Проверяем счетчик
    EXPECT_EQ(finalDict.getWordCount(), 5);

    // Проверяем наличие слов
    EXPECT_TRUE(finalDict.contains("hello"));
    EXPECT_TRUE(finalDict.contains("data"));
    EXPECT_FALSE(finalDict.contains("unknown"));

    // Проверяем что словарь не пустой
    EXPECT_FALSE(finalDict.isEmpty());
}

// 31. Оператор += для std::string
TEST_F(DictionaryTest, OperatorPlusEqualsString) {
    dict += std::make_pair(std::string("dog"), std::string("собака"));
    EXPECT_EQ(dict["dog"], "собака");
}

// 32. Оператор += для const char*
TEST_F(DictionaryTest, OperatorPlusEqualsCharPtr) {
    dict += std::make_pair("dog", "собака");
    EXPECT_EQ(dict["dog"], "собака");
}

// 33. Оператор -= для удаления слова
TEST_F(DictionaryTest, OperatorMinusEquals) {
    dict -= "apple";
    EXPECT_FALSE(dict.contains("apple"));
    EXPECT_EQ(dict.getWordCount(), 2);
}

// 34. Оператор [] для поиска перевода
TEST_F(DictionaryTest, OperatorBracketForSearch) {
    EXPECT_EQ(dict["apple"], "яблоко");
    EXPECT_EQ(dict["nonexistent"], "");
}

// 35. Оператор [] для замены перевода
TEST_F(DictionaryTest, OperatorBracketForUpdate) {
    dict["apple"] = "новое яблоко";
    EXPECT_EQ(dict["apple"], "новое яблоко");
}

// 36. Оператор [] добавляет слово если его нет
TEST_F(DictionaryTest, OperatorBracketAddsNewWord) {
    dict["newword"] = "новое слово";
    EXPECT_TRUE(dict.contains("newword"));
    EXPECT_EQ(dict["newword"], "новое слово");
}

// 37. Тест метода updateTranslation
TEST_F(DictionaryTest, UpdateTranslation) {
    // Обновление существующего слова
    EXPECT_TRUE(dict.updateTranslation("apple", "новый перевод"));
    EXPECT_EQ(dict.getTranslation("apple"), "новый перевод");

    // Попытка обновления несуществующего слова
    EXPECT_FALSE(dict.updateTranslation("nonexistent", "перевод"));
}

// 38. Тест удаления слов
TEST_F(DictionaryTest, RemoveWord) {
    EXPECT_TRUE(dict.contains("apple"));
    dict.removeWord("apple");
    EXPECT_FALSE(dict.contains("apple"));
    EXPECT_EQ(dict.getWordCount(), 2);
}

// 39. Удаление несуществующего слова
TEST_F(DictionaryTest, RemoveNonExistentWord) {
    size_t initialCount = dict.getWordCount();
    dict.removeWord("nonexistent");
    EXPECT_EQ(dict.getWordCount(), initialCount); // счетчик не должен измениться
}

// 40. Тест загрузки из файла
TEST_F(DictionaryTest, LoadFromFile) {
    // Создаем тестовый файл
    std::ofstream file("test_dict.txt");
    file << "dog собака\n";
    file << "house дом\n";
    file << "car машина\n";
    file.close();

    D fileDict;
    EXPECT_TRUE(fileDict.loadFromFile("test_dict.txt"));
    EXPECT_EQ(fileDict.getWordCount(), 3);
    EXPECT_EQ(fileDict.getTranslation("dog"), "собака");
    EXPECT_EQ(fileDict.getTranslation("house"), "дом");
    EXPECT_EQ(fileDict.getTranslation("car"), "машина");
}

// 41. Загрузка из несуществующего файла
TEST_F(DictionaryTest, LoadFromNonExistentFile) {
    D fileDict;
    EXPECT_FALSE(fileDict.loadFromFile("nonexistent.txt"));
}

// 42. Загрузка из пустого файла
TEST_F(DictionaryTest, LoadFromEmptyFile) {
    // Создаем пустой файл
    std::ofstream file("empty_test.txt");
    file.close();

    D emptyDict;
    EXPECT_TRUE(emptyDict.loadFromFile("empty_test.txt"));
    EXPECT_TRUE(emptyDict.isEmpty());
}

// 43. Комбинированное использование операторов
TEST_F(DictionaryTest, CombinedOperatorsUsage) {
    D testDict;

    testDict += std::make_pair("one", "один");
    testDict += std::make_pair("two", "два");

    EXPECT_EQ(testDict["one"], "один");

    testDict["one"] = "единица";
    EXPECT_EQ(testDict["one"], "единица");

    testDict -= "two";
    EXPECT_FALSE(testDict.contains("two"));
}

// 44. Тест на удаление корневого узла
TEST_F(DictionaryTest, RemoveRootNode) {
    D singleDict;
    singleDict.addWord("root", "корень");
    singleDict.removeWord("root");
    EXPECT_FALSE(singleDict.contains("root"));
    EXPECT_TRUE(singleDict.isEmpty());
}

// 45. Тест на удаление узла с двумя детьми
TEST_F(DictionaryTest, RemoveNodeWithTwoChildren) {
    D treeDict;
    // Создаем BST: b (root), a (left), c (right)
    treeDict.addWord("b", "б");
    treeDict.addWord("a", "а");
    treeDict.addWord("c", "в");

    treeDict.removeWord("b"); // Удаляем корень с двумя детьми
    EXPECT_FALSE(treeDict.contains("b"));
    EXPECT_TRUE(treeDict.contains("a"));
    EXPECT_TRUE(treeDict.contains("c"));
    EXPECT_EQ(treeDict.getWordCount(), 2);
}

// 46. Тест на граничные случаи при удалении
TEST_F(DictionaryTest, BoundaryCasesForRemove) {
    D testDict;

    // Удаление из пустого словаря
    testDict.removeWord("anything");
    EXPECT_TRUE(testDict.isEmpty());

    // Добавляем и удаляем один элемент
    testDict.addWord("single", "один");
    testDict.removeWord("single");
    EXPECT_TRUE(testDict.isEmpty());
}

// 47. Тест метода findMin
TEST_F(DictionaryTest, FindMinTest) {
    D sortedDict;
    sortedDict.addWord("z", "з");
    sortedDict.addWord("a", "а");
    sortedDict.addWord("m", "м");

    // Проверяем что минимальный элемент найден правильно
    EXPECT_EQ(sortedDict.getTranslation("a"), "а");
}

// 48. Тест на последовательное добавление и удаление
TEST_F(DictionaryTest, SequentialAddRemove) {
    D seqDict;

    for (int i = 0; i < TestConstants::SMALL_TEST_SIZE; i++) {
        seqDict.addWord("word" + std::to_string(i), "trans" + std::to_string(i));
    }

    EXPECT_EQ(seqDict.getWordCount(), TestConstants::SMALL_TEST_SIZE);

    for (int i = 0; i < 3; i++) {
        seqDict.removeWord("word" + std::to_string(i));
    }

    EXPECT_EQ(seqDict.getWordCount(), TestConstants::SMALL_TEST_SIZE - 3);
    EXPECT_TRUE(seqDict.contains("word3"));
    EXPECT_TRUE(seqDict.contains("word4"));
}

// 49. Тест на стабильность после множественных операций
TEST_F(DictionaryTest, StabilityAfterMultipleOperations) {
    D stableDict;

    // Множественные добавления
    for (int i = 0; i < TestConstants::MEDIUM_TEST_SIZE; i++) {
        stableDict.addWord("key" + std::to_string(i), "value" + std::to_string(i));
    }

    // Множественные обновления
    for (int i = 0; i < TestConstants::SMALL_TEST_SIZE; i++) {
        stableDict.addWord("key" + std::to_string(i), "new_value" + std::to_string(i));
    }

    // Множественные удаления
    for (int i = 0; i < 3; i++) {
        stableDict.removeWord("key" + std::to_string(i));
    }

    EXPECT_EQ(stableDict.getWordCount(), TestConstants::MEDIUM_TEST_SIZE - 3);
    EXPECT_FALSE(stableDict.contains("key0"));
    EXPECT_TRUE(stableDict.contains("key9"));
    EXPECT_EQ(stableDict.getTranslation("key4"), "new_value4");
}

// 50. Финальный интеграционный тест
TEST_F(DictionaryTest, IntegrationTest) {
    D integrationDict;

    // Этап 1: Добавление
    integrationDict.addWord("start", "начало");
    integrationDict.addWord("end", "конец");
    integrationDict.addWord("process", "процесс");

    EXPECT_EQ(integrationDict.getWordCount(), 3);
    EXPECT_TRUE(integrationDict.contains("process"));

    // Этап 2: Обновление через разные методы
    integrationDict.addWord("start", "старт");
    integrationDict.updateTranslation("end", "финиш");
    integrationDict["process"] = "обработка";

    EXPECT_EQ(integrationDict.getTranslation("start"), "старт");
    EXPECT_EQ(integrationDict.getTranslation("end"), "финиш");
    EXPECT_EQ(integrationDict.getTranslation("process"), "обработка");

    // Этап 3: Использование операторов
    integrationDict += std::make_pair("new", "новый");
    integrationDict -= "end";

    EXPECT_TRUE(integrationDict.contains("new"));
    EXPECT_FALSE(integrationDict.contains("end"));
    EXPECT_EQ(integrationDict.getWordCount(), 3);

    // Этап 4: Проверка всех методов
    EXPECT_FALSE(integrationDict.isEmpty());
    EXPECT_FALSE(integrationDict.contains("missing"));
    EXPECT_TRUE(integrationDict.getTranslation("missing").empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}