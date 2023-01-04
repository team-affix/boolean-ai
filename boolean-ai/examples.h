#ifndef EXAMPLES_H
#define EXAMPLES_H

#include <vector>
#include <stddef.h>
#include <filesystem>
#include "cache.h"

namespace boolean_ai
{
    /// @brief This struct has a bit string as input and output.
    struct raw_example
    {
        std::vector<bool> m_input;
        std::vector<bool> m_output;
    public:
        bool operator==(
            const raw_example& a_raw_example
        )
        {
            return
                m_input == a_raw_example.m_input &&
                m_output == a_raw_example.m_output;
        }
    };

    // Input type for the generalized function.
    typedef std::vector<bool> input;

}

template<typename Archive>
void save(
    Archive& a_archive,
    const boolean_ai::input& a_vector
)
{
    a_archive(a_vector.size());
    for (int i = 0; i < a_vector.size(); i++)
        a_archive(a_vector[i]);
}

template<typename Archive>
void load(
    Archive& a_archive,
    boolean_ai::input& a_vector
)
{
    size_t l_vector_size = 0;
    a_archive(l_vector_size);
    for (int i = 0; i < l_vector_size; i++)
    {
        bool l_bool = false;
        a_archive(l_bool);
        a_vector.push_back(l_bool);
    }
}

#endif
