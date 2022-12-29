#ifndef EXAMPLES_H
#define EXAMPLES_H

#include <vector>
#include <stddef.h>

template<typename Archive>
void save(
    Archive& a_archive,
    const std::vector<bool>& a_vector
)
{
    a_archive(a_vector.size());
    for (int i = 0; i < a_vector.size(); i++)
        a_archive(a_vector[i]);
}

template<typename Archive>
void load(
    Archive& a_archive,
    std::vector<bool>& a_vector
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

namespace digital_ai
{
    /// @brief This struct has a bit string as input and output.
    struct raw_example
    {
        std::vector<bool> m_input;
        std::vector<bool> m_output;
    public:
        template<class Archive>
        void serialize(Archive& a_archive)
        {
            a_archive(m_input, m_output);
        }
        bool operator==(
            const raw_example& a_raw_example
        )
        {
            return
                m_input == a_raw_example.m_input &&
                m_output == a_raw_example.m_output;
        }
    };

    /// @brief This is a bit-string which does not satisfy the function which it is being inputted into.
    typedef std::vector<bool> unsatisfying_input;

    /// @brief This is a bit-string which does satisfy the function which it is being inputted into.
    typedef std::vector<bool> satisfying_input;

    /// @brief This struct has a set of unsatisfying inputs, and a set of satisfying inputs.
    ///        These sets will be used for generalization later.
    struct partitioned_example_set
    {
        std::vector<unsatisfying_input*> m_unsatisfying_inputs;
        std::vector<satisfying_input*> m_satisfying_inputs;
        partitioned_example_set(
            const std::vector<raw_example>& a_raw_examples,
            const size_t& a_index
        )
        {
            for (const raw_example& l_raw_example : a_raw_examples)
            {
                // Use pointers to preserve memory.
                if (l_raw_example.m_output[a_index])
                    m_satisfying_inputs.push_back((satisfying_input*)&l_raw_example.m_input);
                else
                    m_unsatisfying_inputs.push_back((unsatisfying_input*)&l_raw_example.m_input);
            }
        }
    };

}

#endif
