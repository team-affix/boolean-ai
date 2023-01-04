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

    template<typename Archive>
    void save(
        Archive& a_archive,
        const input& a_vector
    )
    {
        a_archive((const std::vector<bool>&)a_vector);
    }

    template<typename Archive>
    void load(
        Archive& a_archive,
        input& a_vector
    )
    {
        a_archive((std::vector<bool>&)a_vector);
    }

}

#endif
