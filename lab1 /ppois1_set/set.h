
#ifndef MN_H
#define MN_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <variant>

class MN {
private:
    using Element = std::variant<char, std::shared_ptr<MN>>;
    std::vector<Element> elements_;

    static constexpr char OPEN_BRACE = '{';
    static constexpr char CLOSE_BRACE = '}';
    static constexpr char SEPARATOR = ',';
    static constexpr char SPACE = ' ';

    // Вспомогательные методы для парсинга
    static void skipSpaces(const std::string& str, size_t& pos);
    static char parseChar(const std::string& str, size_t& pos);
    static Element parseElement(const std::string& str, size_t& pos);
    static MN parseSet(const std::string& str, size_t& pos);

public:
    // Конструкторы
    MN() = default;
    explicit MN(char element);
    explicit MN(const std::shared_ptr<MN>& subset);
    MN(std::initializer_list<Element> elements);

    // Создание из строки
    static MN fromString(const std::string& input);

    // Основные операции
    bool contains(const Element& element) const;
    bool contains(char element) const;
    bool contains(const MN& subset) const;
    void addElement(char element);
    void addElement(const std::shared_ptr<MN>& subset);
    bool removeElement(char element);
    bool removeElement(const std::shared_ptr<MN>& subset);
    size_t cardinality() const;
    bool isEmpty() const;
    void clear();
    MN powerSet() const;

    // Математические операции
    MN operator+(const MN& other) const;  // Объединение
    MN operator*(const MN& other) const;  // Пересечение
    MN operator-(const MN& other) const;  // Разность

    MN& operator+=(const MN& other);
    MN& operator*=(const MN& other);
    MN& operator-=(const MN& other);

    // Сравнение
    bool operator==(const MN& other) const;
    bool operator!=(const MN& other) const;

    // Принадлежность
    bool operator[](char element) const;
    bool operator[](const MN& subset) const;

    // Преобразование в строку
    std::string toString() const;

    // Ввод/вывод
    friend std::ostream& operator<<(std::ostream& os, const MN& set);
    friend std::istream& operator>>(std::istream& is, MN& set);

    // Вспомогательные методы для работы с вариантами
    static bool areEqual(const Element& a, const Element& b);
    static std::string elementToString(const Element& element);
    MN powerSetRecursive(const std::vector<Element>& current_set, size_t index) const;
};
#endif
