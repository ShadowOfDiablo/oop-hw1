#pragma once
#include "Vector.hpp"
template <typename K, typename V>
struct Pair {
    K key;
    V value;
};
template <typename K, typename V>

class Map {
private:
    Vector<Pair<K, V>> data;

public:
    void insert(const K& p_key, const V& p_value) {
        for (size_t u32_idx = 0U; u32_idx < data.getSize(); ++u32_idx) {
            if (data[u32_idx].key == p_key) {
                data[u32_idx].value = p_value;
                return;
            }
        }
        data.push_back({ p_key, p_value });
    }

    bool contains(const K& p_key) const {
        bool b_found = false;
        for (size_t u32_idx = 0U; u32_idx < data.getSize(); ++u32_idx) {
            if (data[u32_idx].key == p_key) {
                b_found = true;
                break;
            }
        }
        return b_found;
    }

    V& operator[](const K& p_key) {
        for (size_t u32_idx = 0U; u32_idx < data.getSize(); ++u32_idx) {
            if (data[u32_idx].key == p_key) {
                return data[u32_idx].value;
            }
        }
        data.push_back({ p_key, V() });
        return data[data.getSize() - 1U].value;
    }

    size_t getSize() const { return data.getSize(); }
};