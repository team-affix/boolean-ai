#ifndef CACHE_H
#define CACHE_H

#include <map>
#include <deque>
#include <functional>
#include <string>
#include <exception>
#include <stdexcept>
#include <memory>

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

    std::map<KEY, std::shared_ptr<VALUE>>  m_map;
    std::deque<KEY>                        m_access_order;
    std::map<KEY, size_t>                  m_lock_counts;

    std::function<VALUE(const KEY&)> m_pull;
    std::function<void(const KEY&, const VALUE&)> m_push;

public:
    virtual ~cache(

    )
    {
        while(m_map.size() > 0)
        {
            // If there is an element still locked, this will throw an exception.
            unload(m_access_order.begin());
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

            if (m_map.size() == m_max_size)
            {
                unload(first_unlocked());
            }

            // Inserts the key value pair into the map, and saves the iterator
            // The value must be dynamically allocated so as to preserve the memory address
            // of the actual value in the case of a move event by the overhead of the std::map.
            l_map_location = m_map.emplace(
                a_key,
                std::shared_ptr<VALUE>(new VALUE(m_pull(a_key)))
            ).first;

            // Make note that this was the most recently accessed key value pair.
            m_access_order.push_back(a_key);

            // Insert the number of locks on this value. It should start out being zero.
            m_lock_counts.emplace(a_key, 0);

        }
        else
        {
            typename std::deque<KEY>::iterator l_access_order_location =
                std::find(m_access_order.begin(), m_access_order.end(), a_key);

            // Make note that this was the most recently accessed key value pair.
            m_access_order.erase(l_access_order_location);
            m_access_order.push_back(a_key);

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

        if (m_map.size() == m_max_size)
        {
            unload(first_unlocked());
        }

        // Inserts the key value pair into the map, and saves the iterator
        // The value must be dynamically allocated so as to preserve the memory address
        // of the actual value in the case of a move event by the overhead of the std::map.
        l_map_location = m_map.emplace(
            a_key,
            std::shared_ptr<VALUE>(new VALUE(a_value))
        ).first;

        // Make note that this was the most recently accessed key value pair.
        m_access_order.push_back(a_key);

        // Insert the number of locks on this value. It should start out being zero.
        m_lock_counts.emplace(a_key, 0);

        return entry(*this, l_map_location->first, *l_map_location->second);

    }

private:
    void lock(
        const KEY& a_key
    )
    {
        m_lock_counts[a_key]++;
    }

    void unlock(
        const KEY& a_key
    )
    {
        m_lock_counts[a_key]--;
    }

    void unload(
        std::deque<KEY>::iterator a_access_order_position
    )
    {
        if (m_lock_counts[*a_access_order_position] > 0)
            throw std::runtime_error("Error: lock count was greater than zero on unload() call.");

        // Push the value from cache to remote.
        m_push(*a_access_order_position, *m_map[*a_access_order_position]);

        // Erase the least recently accessed KVP.
        m_map.erase(*a_access_order_position);
        m_lock_counts.erase(*a_access_order_position);

        // This must be done last, since this will invalidate the iterator.
        m_access_order.erase(a_access_order_position);

    }

    std::deque<KEY>::iterator first_unlocked(

    )
    {
        for (auto l_it = m_access_order.begin(); l_it != m_access_order.end(); l_it++)
        {
            if (m_lock_counts[*l_it] == 0)
                return l_it;
        }
        throw std::runtime_error("Error: all keys locked.");
    }

};

#endif
