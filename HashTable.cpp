#include "HashTable.h"



int hash_function_string(const std::string& s, int table_size, const int key) {
    int hash_result = 0;
    for (int i = 0; s[i] != s.size(); ++i)
        hash_result = (key * hash_result + s[i]) % table_size;
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}
int hash_function_int(int val, const int key) {
    return val % key;
}



template<class T, class THash1, class THash2>
HashTable<T, THash1, THash2>::HashTable() {
    this->buffer_size = this->default_size;
    this->size = 0;
    this->size_all_non_nullptr = 0;
    this->array = new HT_Node*[buffer_size];
    for (int i = 0; i < this->buffer_size; ++i)
        this->array[i] = nullptr;
}
template<class T, class THash1, class THash2>
HashTable<T, THash1, THash2>::~HashTable() {
    for (int i = 0; i < this->buffer_size; ++i) {
        if (this->array[i])
            delete this->array[i];
        delete[] this->array;
    }
}



template<class T, class THash1, class THash2>
void HashTable<T, THash1, THash2>::resize() {
    int past_buffer_size = this->buffer_size;
    this->buffer_size *= 2;
    this->size_all_non_nullptr = 0;
    this->size = 0;
    HT_Node** new_array = new HT_Node * [this->buffer_size];
    for (int i = 0; i < this->buffer_size; ++i)
        new_array[i] = nullptr;
    std::swap(this->array, new_array);
    for (int i = 0; i < past_buffer_size; ++i) {
        if (new_array[i] && new_array[i]->state)
            this->add(new_array[i]->value);
    }
    for (int i = 0; i < past_buffer_size; ++i)
        if (new_array[i])
            delete new_array[i];
    delete[] new_array;
}
template<class T, class THash1, class THash2>
void HashTable<T, THash1, THash2>::rehash() {
    this->size_all_non_nullptr = 0;
    this->size = 0;
    HT_Node** new_array = new HT_Node * [this->buffer_size];
    for (int i = 0; i < this->buffer_size; ++i)
        new_array[i] = nullptr;
    std::swap(this->array, new_array);
    for (int i = 0; i < this->buffer_size; ++i) {
        if (new_array[i] && new_array[i]->state)
            this->add(new_array[i]->value);
    }
    for (int i = 0; i < this->buffer_size; ++i)
        if (new_array[i])
            delete new_array[i];
    delete[] new_array;
}
template<class T, class THash1, class THash2>
bool HashTable<T, THash1, THash2>::find(const T &value, const THash1 &hash1, const THash2 &hash2) {
    int h1 = hash1(value, this->buffer_size);
    int h2 = hash2(value, this->buffer_size);
    int i = 0;
    while (this->array[h1] != nullptr && i < this->buffer_size) {
        if (this->array[h1]->value == value && this->array[h2]->state)
            return true;
        h1 = (h1 + h2) % this->buffer_size;
        ++i;
    }
    return false;
}
template<class T, class THash1, class THash2>
bool HashTable<T, THash1, THash2>::remove(const T &value, const THash1 &hash1, const THash2 &hash2) {
    int h1 = hash1(value, this->buffer_size);
    int h2 = hash2(value, this->buffer_size);
    int i = 0;
    while (this->array[h1] != nullptr && i < this->buffer_size) {
        if (this->array[h1]->value == value && this->array[h1]->state) {
            this->array[h1]->state = false;
            this->size -= 1;
            return true;
        }
        h1 = (h1 + h2) % this->buffer_size;
        ++i;
    }
    return false;
}
template<class T, class THash1, class THash2>
bool HashTable<T, THash1, THash2>::add(const T &value, const THash1 &hash1, const THash2 &hash2) {
    if (this->size + 1 > int(this->rehash_size * this->buffer_size))
        this->resize();
    else if (this->size_all_non_nullptr > 2 * this->size)
        this->rehash();
    int h1 = hash1(value, this->buffer_size);
    int h2 = hash2(value, this->buffer_size);
    int i = 0;
    int first_deleted = -1;
    while (this->array[h1] != nullptr && i < this->buffer_size) {
        if (this->array[h1]->value == value && i < this->buffer_size)
            return false;
        if (!this->array[h1]->state && first_deleted == -1)
            first_deleted = h1;
        h1 = (h1 + h2) % this->buffer_size;
        ++i;
    }
    if (first_deleted == -1) {
        this->array[h1] = new HT_Node(value);
        ++this->size_all_non_nullptr;
    }
    else {
        this->array[first_deleted]->value = value;
        this->array[first_deleted]->state = true;
    }
    ++this->size;
    return true;
}