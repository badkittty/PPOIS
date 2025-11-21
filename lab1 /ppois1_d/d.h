#ifndef D_H
#define D_H

#include <string>
#include <memory>
#include <utility> // для std::pair

class D {
public:
    D();

    // Основные методы
    void addWord(const std::string& english, const std::string& russian);
    void removeWord(const std::string& english);
    std::string getTranslation(const std::string& english) const;
    bool updateTranslation(const std::string& english, const std::string& russian);
    bool contains(const std::string& english) const;
    size_t getWordCount() const;
    bool isEmpty() const;

    // Загрузка из файла
    bool loadFromFile(const std::string& filename);

    // Требуемые операторы
    D& operator+=(const std::pair<const char*, const char*>& words); // для char*
    D& operator+=(const std::pair<std::string, std::string>& words); // для std::string
    D& operator-=(const std::string& english); // удаление слова
    std::string operator[](const std::string& english) const; // поиск перевода
    std::string& operator[](const std::string& english); // замена перевода

private:
    struct Node {
        std::string english;
        std::string russian;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        Node(const std::string& e, const std::string& r) : english(e), russian(r) {}
    };

    std::shared_ptr<Node> root;
    size_t wordCount;

    // Вспомогательные методы
    std::shared_ptr<Node> insert(std::shared_ptr<Node> node, const std::string& english, const std::string& russian);
    std::shared_ptr<Node> remove(std::shared_ptr<Node> node, const std::string& english);
    std::shared_ptr<Node> find(std::shared_ptr<Node> node, const std::string& english) const;
    std::shared_ptr<Node> findMin(std::shared_ptr<Node> node) const;
};

#endif