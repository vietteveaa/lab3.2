#pragma once
#include <string>

int hash_function_string(const std::string& s, int table_size, const int key);
int hash_function_int(int val, const int key);
template<class T>
struct HashFunction1 {
    int operator()(const std::string& s, int table_size) const {
        return hash_function_string(s, table_size, table_size - 1);
    }
    int operator()(int val, int table_size) {
        return hash_function_int(val, table_size - 1)
    }
};
template<class T>
struct HashFunction2 {
    int operator()(const std::string& s, int table_size) const; {
        return hash_function_string(s, table_size, table_size + 1);
    }
    int operator()(int val, int table_size) {
        return hash_function_int(val, table_size + 1)
    }
};

template<class T, class THash1 = HashFunction1<T>, class THash2 = HashFunction2<T>>
class HashTable {
private:
    static const int default_size = 10;
    constexpr static const double rehash_size = 0.75;
    struct HT_Node {
        T value;
        bool state;
        HT_Node(const T& value_) : value(value_), state(true) {}
    };
    HT_Node** array;
    int size;
    int buffer_size;
    int size_all_non_nullptr;
public:
    HashTable();
    ~HashTable();
    void resize();
    void rehash();
    bool find(const T& value, const THash1& hash1 = THash1(), const THash2& hash2 = THash2());
    bool remove(const T& value, const THash1& hash1 = THash1(), const THash2& hash2 = THash2());
    bool add(const T& value, const THash1& hash1 = THash1(), const THash2& hash2 = THash2());
};