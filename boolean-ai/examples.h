#ifndef EXAMPLES_H
#define EXAMPLES_H

#include <vector>
#include <stddef.h>
#include <filesystem>
#include "cache.h"

namespace digital_ai
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
        template<class Archive>
        void serialize(Archive& a_archive)
        {
            a_archive(m_input, m_output);
        }
    };

    // Input type for the generalized function.
    typedef std::vector<bool> input;

    // Describes an input as it pertains to a certain cache (by key).
    typedef cache<std::filesystem::path, input>::latent latent_input;

    /// @brief This struct has a set of unsatisfying inputs, and a set of satisfying inputs.
    ///        These sets will be used for generalization later.
    struct partitioned_example_set
    {
        std::vector<latent_input> m_unsatisfying_inputs;
        std::vector<latent_input> m_satisfying_inputs;
        cache<std::filesystem::path, input>& m_input_cache;
        
        partitioned_example_set(
            cache<std::filesystem::path, input>& a_input_cache
        ) :
            m_input_cache(a_input_cache)
        {

        }

        template<typename Archive>
        void save(
            Archive& a_archive
        )
        {
            // First, deal with unsatisfying inputs.
            a_archive(m_unsatisfying_inputs.size());
            for (const auto& l_latent : m_unsatisfying_inputs)
                a_archive(l_latent.key());

            // Second, deal with satisfying inputs.
            a_archive(m_satisfying_inputs.size());
            for (const auto& l_latent : m_satisfying_inputs)
                a_archive(l_latent.key());
            
        }

        template<typename Archive>
        void load(
            Archive& a_archive
        )
        {
            size_t l_unsatisfying_size = 0;
            a_archive(l_unsatisfying_size);
            
            for (int i = 0; i < l_unsatisfying_size; i++)
            {
                std::filesystem::path l_key;
                a_archive(l_key);
                m_unsatisfying_inputs.push_back(m_input_cache.make_latent(l_key));
            }

            size_t l_satisfying_size = 0;
            a_archive(l_satisfying_size);

            for (int i = 0; i < l_satisfying_size; i++)
            {
                std::filesystem::path l_key;
                a_archive(l_key);
                m_satisfying_inputs.push_back(m_input_cache.make_latent(l_key));
            }

        }

    };

}

template<typename Archive>
void save(
    Archive& a_archive,
    const digital_ai::input& a_vector
)
{
    a_archive(a_vector.size());
    for (int i = 0; i < a_vector.size(); i++)
        a_archive(a_vector[i]);
}

template<typename Archive>
void load(
    Archive& a_archive,
    digital_ai::input& a_vector
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
