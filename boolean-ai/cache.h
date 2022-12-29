#ifndef CACHE_H
#define CACHE_H

#include "cereal/cereal.hpp"
#include <filesystem>
#include <map>
#include <deque>

namespace digital_ai
{
    template<typename KEY, typename VALUE>
    class temporal_cache
    {
    private:
        std::function<VALUE(const KEY&)> m_function;
        size_t m_max_size = 0;
        std::map<KEY, VALUE> m_map;
        std::deque<KEY> m_access_order;

    public:
        temporal_cache(
            const std::function<VALUE(const KEY&)>& a_function,
            const size_t& a_max_size
        ) :
            m_function(a_function),
            m_max_size(a_max_size)
        {

        }

        const VALUE& evaluate(
            const KEY& a_key
        )
        {
            typename std::map<KEY, VALUE>::iterator l_map_location = m_map.find(a_key);
            
            if (l_map_location == m_map.end())
            {
                // Hit!

                if (m_map.size() == m_max_size)
                {
                    // Erase the least recently accessed KVP.
                    m_map.erase(m_access_order.front());
                    m_access_order.pop_front();
                }

                // Inserts the key value pair into the map, and saves the iterator
                l_map_location = m_map.emplace(a_key, m_function(a_key)).first;

                // Make note that this was the most recently accessed key value pair.
                m_access_order.push_back(a_key);

                return l_map_location->second;

            }

            // Miss!

            typename std::deque<KEY>::iterator l_access_order_location =
                std::find(m_access_order.begin(), m_access_order.end(), a_key);

            // Make note that this was the most recently accessed key value pair.
            m_access_order.erase(l_access_order_location);
            m_access_order.push_back(a_key);

            return l_map_location->second;

        }

    };
}


#endif
