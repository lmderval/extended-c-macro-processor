#pragma once

#include <stdexcept>

#include "scoped_map.hh"

namespace misc {
    template <typename Key, typename T>
    inline void scoped_map<Key, T>::enter() {
        scoped_map_.emplace_back();
    }

    template <typename Key, typename T>
    inline void scoped_map<Key, T>::leave() {
        scoped_map_.pop_back();
    }

    template <typename Key, typename T>
    inline bool scoped_map<Key, T>::top_contains(const key_type& key) const {
        return !scoped_map_.empty() && scoped_map_.back().contains(key);
    }

    template <typename Key, typename T>
    const scoped_map<Key, T>::mapped_type&
    scoped_map<Key, T>::at(const key_type& key) const {
        for (auto it = scoped_map_.crbegin(); it != scoped_map_.crend(); it++) {
            if (it->contains(key)) return it->at(key);
        }
        throw std::out_of_range(
            "specified key cannot be found inside scoped map");
    }

    template <typename Key, typename T>
    scoped_map<Key, T>::mapped_type&
    scoped_map<Key, T>::at(const key_type& key) {
        for (auto it = scoped_map_.rbegin(); it != scoped_map_.rend(); it++) {
            if (it->contains(key)) return it->at(key);
        }
        throw std::out_of_range(
            "specified key cannot be found inside scoped map");
    }

    template <typename Key, typename T>
    bool scoped_map<Key, T>::empty() const {
        return scoped_map_.empty();
    }

    template <typename Key, typename T>
    inline void scoped_map<Key, T>::clear() {
        scoped_map_.clear();
    }

    template <typename Key, typename T>
    inline bool scoped_map<Key, T>::insert(const value_type& value) {
        return !scoped_map_.empty() && scoped_map_.back().insert(value).second;
    }

    template <typename Key, typename T>
    inline bool scoped_map<Key, T>::insert(value_type&& value) {
        return !scoped_map_.empty() && scoped_map_.back().insert(value).second;
    }

    template <typename Key, typename T>
    template <class... Args>
    inline bool scoped_map<Key, T>::emplace(Args&&... args) {
        return !scoped_map_.empty()
            && scoped_map_.back().emplace(args...).second;
    }

    template <typename Key, typename T>
    int scoped_map<Key, T>::erase(const key_type& key) {
        if (scoped_map_.empty()) return 0;
        return scoped_map_.back().erase(key);
    }

    template <typename Key, typename T>
    bool scoped_map<Key, T>::contains(const key_type& key) const {
        for (auto it = scoped_map_.crbegin(); it != scoped_map_.crend(); it++) {
            if (it->contains(key)) return true;
        }
        return false;
    }
} // namespace misc
