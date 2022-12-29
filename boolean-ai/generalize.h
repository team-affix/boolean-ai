#ifndef GENERALIZE_H
#define GENERALIZE_H

#include "examples.h"
#include <assert.h>
#include <algorithm>
#include <map>
#include <chrono>
#include <iostream>
#include <math.h>

namespace digital_ai
{
    /// @brief A class which selects a bit by index and also states whether or not it will be inverting that bit.
    class literal
    {
    private:
        size_t m_index = 0;
        bool m_invert = false;

    public:
        literal(

        )
        {
            
        }

        literal(
            const size_t& a_input_index,
            const bool& a_invert
        ) :
            m_index(a_input_index),
            m_invert(a_invert)
        {
            
        }

        bool evaluate(
            const std::vector<bool>& a_input
        )
        {
            // Perform an XOR in order to conditionally invert the
            // input boolean.
            return a_input[m_index] != m_invert;
        }

        bool operator==(
            const literal& a_literal
        ) const
        {
            return 
                m_index == a_literal.m_index &&
                m_invert      == a_literal.m_invert;
        }

        const size_t& index(

        ) const
        {
            return m_index;
        }

        const bool& invert(

        ) const
        {
            return m_invert;
        }

        bool operator<(
            const literal& a_literal
        ) const
        {
            if (m_index < a_literal.index())
                return true;
            if (m_index > a_literal.index())
                return false;
            
            // If we make it here, then the indices were equal. Now, compare m_invert.

            if (!m_invert && a_literal.invert())
                return true;

            // Default is false, since it is a non-inclusive inequality.
            return false;

        }


    };

    /// @brief A class which acts as a boolean product of literals.
    class literal_product
    {
    private:
        std::vector<literal> m_literals;
        
    public:
        literal_product(
            const std::vector<literal>& a_literals
        ) :
            m_literals(a_literals)
        {

        }

        literal_product(
            const std::vector<bool>& a_state
        )
        {
            for (int i = 0; i < a_state.size(); i++)
                m_literals.push_back(literal(i, !a_state[i]));
        }

        bool evaluate(
            const std::vector<bool>& a_input
        )
        {
            for (literal& l_literal : m_literals)
                if (!l_literal.evaluate(a_input))
                    return false;
            return true;
        }

        /// @brief This operator determines if the argued literal product is
        ///        functionally equal to the local literal product.
        /// @param a_literal_product 
        /// @return 
        bool operator==(
            const literal_product& a_literal_product
        ) const
        {
            if (m_literals.size() != a_literal_product.m_literals.size())
                return false;

            // Make sure that every local literal is in the other literal product.
            for (const literal& l_literal_0 : m_literals)
            {
                bool l_literal_is_shared = false;

                for (const literal& l_literal_1 : a_literal_product.m_literals)
                {
                    if (l_literal_1 == l_literal_0)
                    {
                        l_literal_is_shared = true;
                        break;
                    }
                }

                if (!l_literal_is_shared)
                    return false;

            }

            // Make sure that every literal in the other product is shared locally.
            for (const literal& l_literal_0 : a_literal_product.m_literals)
            {
                bool l_literal_is_shared = false;

                for (const literal& l_literal_1 : m_literals)
                {
                    if (l_literal_1 == l_literal_0)
                    {
                        l_literal_is_shared = true;
                        break;
                    }
                }

                if (!l_literal_is_shared)
                    return false;

            }

            return true;
            
        }

        bool operator!=(
            const literal_product& a_literal_product
        ) const
        {
            return !operator==(a_literal_product);
        }

        const std::vector<literal>& literals(

        ) const
        {
            return m_literals;
        }

    };

    /// @brief A class which acts as a boolean sum of products of literals.
    class sum_of_products
    {
    private:
        std::vector<literal_product> m_literal_products;
        
    public:
        sum_of_products(
            const std::vector<literal_product>& a_literal_products
        ) :
            m_literal_products(a_literal_products)
        {

        }

        bool evaluate(
            const std::vector<bool>& a_input
        )
        {
            for (literal_product& l_literal_product : m_literal_products)
                if (l_literal_product.evaluate(a_input))
                    return true;
            return false;
        }

        /// @brief This method currently eliminates duplicate literal products.
        void simplify(

        )
        {
            for (int i = m_literal_products.size() - 1; i >= 0; i--)
            {
                auto l_literal_product_0_iterator = m_literal_products.begin() + i;
                
                bool l_repeated_literal_product = false;
                
                for (int j = 0; j < i; j++)
                {
                    auto l_literal_product_1_iterator = m_literal_products.begin() + j;

                    if (*l_literal_product_1_iterator == *l_literal_product_0_iterator)
                    {
                        l_repeated_literal_product = true;
                        break;
                    }
                }

                // Erase a single occurance of the repeated literal product.
                if (l_repeated_literal_product)
                {
                    m_literal_products.erase(l_literal_product_0_iterator);
                }

            }

        }

        const std::vector<literal_product>& literal_products(

        ) const
        {
            return m_literal_products;
        }

    };

    /// @brief A class which acts as a vector of sums of products of literals.
    ///        (Used for multiple-output functions)
    class sum_of_products_string
    {
    private:
        std::vector<sum_of_products> m_binary_functions;

    public:
        sum_of_products_string(
            const std::vector<sum_of_products>& a_binary_functions
        ) :
            m_binary_functions(a_binary_functions)
        {
            
        }

        std::vector<bool> evaluate(
            const std::vector<bool>& a_input
        )
        {
            std::vector<bool> l_result(m_binary_functions.size());
            for (int i = 0; i < l_result.size(); i++)
                l_result[i] = m_binary_functions[i].evaluate(a_input);
            return l_result;
        }

        void simplify(

        )
        {
            for (auto& l_sum : m_binary_functions)
                l_sum.simplify();
        }

        const std::vector<sum_of_products>& sums_of_products(

        ) const
        {
            return m_binary_functions;
        }

    };

    std::map<literal, std::vector<unsatisfying_input*>> literal_coverage(
        const std::vector<unsatisfying_input*>& a_covered_unsatisfying_inputs
    )
    {
        // Create the "what-if" literal coverage vector which describes the
        // unsatisfying inputs that will be covered given the selection of any given literal
        // and the current covering product's literals.
        std::map<literal, std::vector<unsatisfying_input*>> l_result;

        for (int i = 0; i < a_covered_unsatisfying_inputs[0]->size(); i++)
        {
            l_result.emplace(literal(i, false), std::vector<unsatisfying_input*>());
            l_result.emplace(literal(i, true), std::vector<unsatisfying_input*>());
        }
        
        for (unsatisfying_input* l_unsatisfying_input : a_covered_unsatisfying_inputs)
        {
            for (int i = 0; i < l_unsatisfying_input->size(); i++)
            {
                if (l_unsatisfying_input->at(i))
                    // Make a note that this unsatisfying input contains the non-inverted literal.
                    l_result[literal(i, false)].push_back(l_unsatisfying_input);
                else
                    // Make a note that this unsatisfying input contains the inverted literal.
                    l_result[literal(i, true)].push_back(l_unsatisfying_input);
            }
        }

        return l_result;

    }

    class literal_coverage_tree
    {
    private:
        std::vector<literal> m_covering_literals;
        size_t m_coverage_size = 0;
        std::vector<unsatisfying_input*> m_unprocessed_coverage;
        std::map<literal, literal_coverage_tree> m_subcoverages;

    public:
        literal_coverage_tree(
            const std::vector<literal>& a_covering_literals = {}
        ) :
            m_covering_literals(a_covering_literals)
        {
            
        }

        void add_coverage(
            const std::vector<unsatisfying_input*>& a_additional_coverage
        )
        {
            // Insert the additional coverage into the vector of total unsatisfying coverage
            // for this node.
            m_unprocessed_coverage.insert(m_unprocessed_coverage.end(), a_additional_coverage.begin(), a_additional_coverage.end());
            
            // Increase the total coverage size.
            m_coverage_size += a_additional_coverage.size();

        }

        literal_product covering_product(
            const satisfying_input* a_satisfying_input
        )
        {
            literal_coverage_tree* l_minimal_literal_coverage_tree = this;

            while(l_minimal_literal_coverage_tree->m_coverage_size > 0)
            {
                l_minimal_literal_coverage_tree =
                    &l_minimal_literal_coverage_tree->minimal_literal_coverage_tree(a_satisfying_input);
            }

            return literal_product(l_minimal_literal_coverage_tree->m_covering_literals);
            
        }

    private:
        void realize_subcoverages(
            
        )
        {
            // First, get the raw literal coverage.
            std::map<literal, std::vector<unsatisfying_input*>> l_literal_coverage = literal_coverage(
                m_unprocessed_coverage
            );

            for (auto l_it = l_literal_coverage.begin(); l_it != l_literal_coverage.end(); l_it++)
            {
                if (m_covering_literals.end() != std::find(
                        m_covering_literals.begin(), m_covering_literals.end(), 
                        l_it->first))
                    // Just ignore this literal since one of the ancestors to this node 
                    // already was this literal.
                    continue;

                if (m_covering_literals.end() != std::find(
                        m_covering_literals.begin(), m_covering_literals.end(),
                        literal(l_it->first.index(), !l_it->first.invert())))
                    // Just ignore this literal since one of the ancestors to this node 
                    // already was the inverted form of this literal.
                    continue;

                std::vector<literal> l_covering_literals = m_covering_literals;

                l_covering_literals.push_back(l_it->first);

                // Generate subtrees for each literal coverage.
                std::map<literal, literal_coverage_tree>::iterator l_subtree = m_subcoverages.find(l_it->first);
                
                if (l_subtree == m_subcoverages.end())
                {
                    l_subtree =
                        m_subcoverages.emplace(l_it->first, literal_coverage_tree(l_covering_literals)).first;
                }

                l_subtree->second.add_coverage(l_literal_coverage[l_it->first]);

            }

            // The unprocessed coverage of the lineage of literals leading up to this node
            // is no longer useful. Erase it to preserve memory.
            m_unprocessed_coverage.clear();

        }

        literal_coverage_tree& minimal_literal_coverage_tree(
            const satisfying_input* a_satisfying_input
        )
        {
            // This function call will realize the subcoverages, before returning the
            // tree whose root has minimal literal coverage.

            if (m_unprocessed_coverage.size() > 0)
            {
                // Only realize the subcoverages if it must be done,
                // assuming the unprocessed coverage has a size larger than zero.
                realize_subcoverages();
            }
            
            literal_coverage_tree* l_minimal_literal_coverage_tree = nullptr;

            for (auto l_it = m_subcoverages.begin(); l_it != m_subcoverages.end(); l_it++)
            {
                // First, check to make sure that the literal exists within
                // the satisfying input.
                bool l_literal_exists_within_satisfying_input = 
                    a_satisfying_input->at(l_it->first.index()) != l_it->first.invert();

                if (l_literal_exists_within_satisfying_input &&
                    ((l_minimal_literal_coverage_tree == nullptr) ||
                    l_it->second.m_coverage_size < l_minimal_literal_coverage_tree->m_coverage_size))
                {
                    l_minimal_literal_coverage_tree = &l_it->second;
                }
            }

            return *l_minimal_literal_coverage_tree;
            
        }

    };

    /// @brief Gets a sum of covering products which when coupled, will cover all satisfying inputs.
    ///        this will not cover any unsatisfying inputs. It will however cover some unresolved inputs.
    ///        This is where the "generalization" occurred.
    /// @param a_partitioned_example_set
    /// @return
    sum_of_products generalize(
        const partitioned_example_set& a_partitioned_example_set
    )
    {
        literal_coverage_tree l_literal_coverage_tree;

        l_literal_coverage_tree.add_coverage(a_partitioned_example_set.m_unsatisfying_inputs);

        std::vector<literal_product> l_covering_products;
        
        for (int i = 0; i < a_partitioned_example_set.m_satisfying_inputs.size(); i++)
        {
            l_covering_products.push_back(
                l_literal_coverage_tree.covering_product(a_partitioned_example_set.m_satisfying_inputs[i]));
        }

        return sum_of_products(l_covering_products);

    }

    /// @brief This will produce a vector of covering sums which each are designated to the bit output
    ///        associated with their indices.
    /// @param a_additional_raw_examples 
    /// @return 
    sum_of_products_string generalize(
        const std::vector<raw_example>& a_additional_raw_examples
    )
    {
        std::vector<sum_of_products> l_output_bit_functions;

        for (int i = 0; i < a_additional_raw_examples[0].m_output.size(); i++)
        {
            l_output_bit_functions.push_back(generalize(partitioned_example_set(a_additional_raw_examples, i)));
        }

        return sum_of_products_string(l_output_bit_functions);

    }

}

#endif
