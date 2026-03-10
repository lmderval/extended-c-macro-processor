#pragma once

#include <map>
#include <vector>

namespace misc {
    template <typename Key, typename T>
    class scoped_map {
    public:
        using key_type = Key;
        using mapped_type = T;
        using value_type = std::pair<const Key, T>;

    public:
        void enter();
        void leave();

        bool top_contains(const key_type& key) const;

    public:
        const mapped_type& at(const key_type& key) const;
        mapped_type& at(const key_type& key);

        bool empty() const;

        void clear();

        bool insert(const value_type& value);
        bool insert(value_type&& value);

        template <class... Args>
        bool emplace(Args&&... args);

        int erase(const key_type& key);

        bool contains(const key_type& key) const;

    private:
        std::vector<std::map<key_type, mapped_type>> scoped_map_;
    };
} // namespace misc

#include "scoped_map.hxx"
