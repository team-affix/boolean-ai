#ifndef CACHE_H
#define CACHE_H

#include <map>
#include <functional>
#include <string>
#include <exception>
#include <stdexcept>
#include <memory>
#include <list>

template<typename KEY, typename VALUE>
class cache
{
public:
    class entry
    {
    private:
        cache&     m_cache;
        const KEY& m_key;
        VALUE&     m_reference;

    public:
        virtual ~entry(

        )
        {
            m_cache.unlock(m_key);
        }

        entry(
            cache& a_cache,
            const KEY& a_key,
            VALUE& a_reference
        ) :
            m_cache(a_cache),
            m_key(a_key),
            m_reference(a_reference)
        {
            m_cache.lock(a_key);
        }

        entry(
            const entry& a_entry
        ) :
            m_cache(a_entry.m_cache),
            m_key(a_entry.m_key),
            m_reference(a_entry.m_reference)
        {
            m_cache.lock(a_entry.m_key);
        }
        
        VALUE& operator*(

        )
        {
            return m_reference;
        }

        VALUE* operator->(

        )
        {
            return &m_reference;
        }

    };

private:
    size_t m_max_size = 0;

    std::map<KEY, std::shared_ptr<VALUE>>            m_map;
    std::map<KEY, size_t>                            m_lock_counts;
    std::map<KEY, typename std::list<KEY>::iterator> m_access_order_map;
    std::list<KEY>                                   m_access_order;

    std::function<VALUE(const KEY&)> m_pull;
    std::function<void(const KEY&, const VALUE&)> m_push;

public:
    virtual ~cache(

    )
    {
        while(m_map.size() > 0)
        {
            // If there is an element still locked, this will throw an exception.
            unload_first();
        }
    }

    cache(
        const size_t& a_max_size,
        const std::function<VALUE(const KEY&)>& a_pull,
        const std::function<void(const KEY&, const VALUE&)>& a_push
    ) :
        m_max_size(a_max_size),
        m_pull(a_pull),
        m_push(a_push)
    {

    }

    entry get(
        const KEY& a_key
    )
    {
        typename std::map<KEY, std::shared_ptr<VALUE>>::iterator l_map_location = m_map.find(a_key);

        if (l_map_location == m_map.end())
        {
            // Miss!

            l_map_location = _insert(a_key, m_pull(a_key));

        }

        return entry(*this, l_map_location->first, *l_map_location->second);

    }

    entry insert(
        const KEY& a_key,
        const VALUE& a_value
    )
    {
        typename std::map<KEY, std::shared_ptr<VALUE>>::iterator l_map_location = m_map.find(a_key);

        if (l_map_location != m_map.end())
        {
            // Hit!
            throw std::runtime_error("Error: key was already found in map.");
        }

        // Miss!

        l_map_location = _insert(a_key, a_value);

        return entry(*this, l_map_location->first, *l_map_location->second);

    }

private:
    std::map<KEY, std::shared_ptr<VALUE>>::iterator _insert(
        const KEY& a_key,
        const VALUE& a_value
    )
    {
        if (m_map.size() == m_max_size)
        {
            unload_first();
        }

        // Inserts the key value pair into the map, and saves the iterator
        // The value must be dynamically allocated so as to preserve the memory address
        // of the actual value in the case of a move event by the overhead of the std::map.
        typename std::map<KEY, std::shared_ptr<VALUE>>::iterator l_map_location = m_map.emplace(
            a_key,
            std::shared_ptr<VALUE>(new VALUE(a_value))
        ).first;

        // Make note that this was the most recently accessed key value pair.
        m_access_order.push_back(a_key);
        m_access_order_map.emplace(a_key, std::prev(m_access_order.end()));

        // Insert the number of locks on this value. It should start out being zero.
        m_lock_counts.emplace(a_key, 0);

        return l_map_location;

    }

    void lock(
        const KEY& a_key
    )
    {
        if (m_lock_counts[a_key] == 0)
        {
            // Remove key from access order and access order map
            m_access_order.erase(m_access_order_map[a_key]);
            m_access_order_map.erase(a_key);
        }

        m_lock_counts[a_key]++;

    }

    void unlock(
        const KEY& a_key
    )
    {
        m_lock_counts[a_key]--;

        if (m_lock_counts[a_key] == 0)
        {
            // Add key to access order
            m_access_order.push_back(a_key);
            m_access_order_map[a_key] = std::prev(m_access_order.end());
        }

    }

    void unload_first(
        
    )
    {
        if (m_access_order.size() == 0)
            throw std::runtime_error("Error: could not unload any elements. No elements were unlocked.");

        KEY l_key = m_access_order.front();

        // Push the value from cache to remote.
        m_push(l_key, *m_map[l_key]);

        // Erase the least recently accessed KVP.
        m_map.erase(l_key);
        m_lock_counts.erase(l_key);

        // Finally, erase the key from the front of the access order.
        m_access_order.pop_front();
        m_access_order_map.erase(l_key);

    }

};

#endif
