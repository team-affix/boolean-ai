#ifndef GENERALIZE_H
#define GENERALIZE_H

#include "examples.h"
#include "cache.h"
#include <assert.h>
#include <algorithm>
#include <map>
#include <math.h>
#include <fstream>
#include "cereal/types/vector.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/map.hpp"
#include "cereal/types/deque.hpp"
#include "cereal/types/string.hpp"

namespace cereal
{
    template<typename ARCHIVE>
    void save(
        ARCHIVE& a_archive,
        const std::filesystem::path& a_path
    )
    {
        a_archive(a_path.generic_string());
    }

    template<typename ARCHIVE>
    void load(
        ARCHIVE& a_archive,
        std::filesystem::path& a_path
    )
    {
        std::string l_string;
        a_archive(l_string);
        a_path = std::filesystem::path(l_string);
    }
}

namespace boolean_ai
{
    // Extern fields which should be user-defined.
    extern size_t INPUT_CACHE_SIZE;
    extern size_t TREE_CACHE_SIZE;

    // Inline fields
    inline std::string i_batch_index_file_name = "batch-index.bin";

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

        template<typename Archive>
        void serialize(Archive& a_archive)
        {
            a_archive(m_index, m_invert);
        }

    };

    /// @brief A class which acts as a boolean product of literals.
    class literal_product
    {
    private:
        std::vector<literal> m_literals;
        
    public:
        literal_product(

        )
        {

        }

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

        template<typename Archive>
        void serialize(Archive& a_archive)
        {
            a_archive(m_literals);
        }

    };

    /// @brief A class which acts as a boolean sum of products of literals.
    class sum_of_products
    {
    private:
        std::vector<literal_product> m_literal_products;
        
    public:
        sum_of_products(

        )
        {

        }

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

        template<typename Archive>
        void serialize(Archive& a_archive)
        {
            a_archive(m_literal_products);
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

        )
        {

        }

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

        template<typename Archive>
        void serialize(Archive& a_archive)
        {
            a_archive(m_binary_functions);
        }

    };

    std::map<literal, std::vector<std::filesystem::path>> literal_coverage(
        cache<std::filesystem::path, input>& a_cache,
        std::vector<std::filesystem::path>& a_covered_unsatisfying_inputs
    )
    {
        // Create the "what-if" literal coverage vector which describes the
        // unsatisfying inputs that will be covered given the selection of any given literal
        // and the current covering product's literals.
        std::map<literal, std::vector<std::filesystem::path>> l_result;

        cache<std::filesystem::path, input>::entry l_first_input = a_cache.get(a_covered_unsatisfying_inputs.front());

        for (int i = 0; i < l_first_input->size(); i++)
        {
            l_result.emplace(literal(i, false), std::vector<std::filesystem::path>());
            l_result.emplace(literal(i, true), std::vector<std::filesystem::path>());
        }
        
        for (std::filesystem::path& l_input_path : a_covered_unsatisfying_inputs)
        {
            cache<std::filesystem::path, input>::entry l_input = a_cache.get(l_input_path);
            
            for (int i = 0; i < l_input->size(); i++)
            {
                if (l_input->at(i))
                    // Make a note that this unsatisfying input contains the non-inverted literal.
                    l_result[literal(i, false)].push_back(l_input_path);
                else
                    // Make a note that this unsatisfying input contains the inverted literal.
                    l_result[literal(i, true)].push_back(l_input_path);
            }
        }

        return l_result;

    }

    /// @brief Tree for use in speeding up generation of covering products.
    class unsatisfying_coverage_tree
    {
    private:
        cache<std::filesystem::path, input>&     m_input_cache;
        std::vector<literal>                     m_covering_literals;
        std::vector<std::filesystem::path>       m_unprocessed_coverage;
        size_t                                   m_coverage_size = 0;
        std::map<literal, size_t>                m_subcoverage_sizes;
        std::map<literal, std::filesystem::path> m_subcoverage_paths;

    public:
        unsatisfying_coverage_tree(
            cache<std::filesystem::path, input>& a_input_cache,
            const std::vector<literal>& a_covering_literals = {}
        ) :
            m_input_cache(a_input_cache),
            m_covering_literals(a_covering_literals)
        {
            
        }
        
        const size_t& coverage_size(

        ) const
        {
            return m_coverage_size;
        }

        const std::vector<literal>& covering_literals(

        ) const
        {
            return m_covering_literals;
        }

        void add_coverage(
            const std::vector<std::filesystem::path>& a_additional_coverage
        )
        {
            // Insert the additional coverage into the vector of total unsatisfying coverage
            // for this node.
            m_unprocessed_coverage.insert(m_unprocessed_coverage.end(), a_additional_coverage.begin(), a_additional_coverage.end());
            
            // Add to the current known coverage size.
            m_coverage_size += a_additional_coverage.size();

        }

        std::string coverage_minimizing_subtree_file_name(
            cache<std::filesystem::path, unsatisfying_coverage_tree>& a_tree_cache,
            cache<std::filesystem::path, input>::entry a_satisfying_input
        )
        {
            // This function call will realize the subcoverages, before returning the
            // tree whose root has minimal literal coverage.

            if (m_unprocessed_coverage.size() > 0)
            {
                // Only realize the subcoverages if it must be done,
                // assuming the unprocessed coverage has a size larger than zero.
                propagate_coverage(a_tree_cache);
            }
            
            std::map<literal, size_t>::iterator l_coverage_minimizing_iterator = m_subcoverage_sizes.end();

            for (auto l_it = m_subcoverage_sizes.begin(); l_it != m_subcoverage_sizes.end(); l_it++)
            {
                if (l_it == l_coverage_minimizing_iterator)
                    // Just skip this iteration. The current literal is equal to the literal
                    // represented by this iteration.
                    continue;

                // First, check to make sure that the literal exists within
                // the satisfying input.
                bool l_literal_exists_within_satisfying_input = 
                    a_satisfying_input->at(l_it->first.index()) != l_it->first.invert();

                if (l_literal_exists_within_satisfying_input &&
                    (l_coverage_minimizing_iterator == m_subcoverage_sizes.end() || 
                    l_it->second < l_coverage_minimizing_iterator->second))
                {
                    l_coverage_minimizing_iterator = l_it;
                }
            }

            return m_subcoverage_paths[l_coverage_minimizing_iterator->first];
            
        }

        template<typename ARCHIVE>
        void serialize(
            ARCHIVE& a_archive
        )
        {
            a_archive(m_covering_literals);
            a_archive(m_unprocessed_coverage);
            a_archive(m_coverage_size);
            a_archive(m_subcoverage_sizes);
            a_archive(m_subcoverage_paths);
        }

    private:
        void propagate_coverage(
            cache<std::filesystem::path, unsatisfying_coverage_tree>& a_tree_cache
        )
        {
            // First, get the raw literal coverage.
            std::map<literal, std::vector<std::filesystem::path>> l_literal_coverage = literal_coverage(
                m_input_cache,
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
                std::map<literal, std::filesystem::path>::iterator l_subtree_iterator = m_subcoverage_paths.find(l_it->first);
                
                if (l_subtree_iterator == m_subcoverage_paths.end())
                {
                    // Create the subtree
                    unsatisfying_coverage_tree l_subtree(
                        m_input_cache, l_covering_literals);

                    // Insert the subtree into cache
                    a_tree_cache.insert(l_subtree.file_name(), l_subtree);

                    // Insert the subtree path into the list of subtree paths.
                    l_subtree_iterator =
                        m_subcoverage_paths.emplace(l_it->first, l_subtree.file_name()).first;

                    // Create the entry in the coverage size map
                    m_subcoverage_sizes.emplace(l_it->first, 0);

                }

                cache<std::filesystem::path, unsatisfying_coverage_tree>::entry l_subtree_entry =
                    a_tree_cache.get(l_subtree_iterator->second);

                m_subcoverage_sizes[l_it->first] += l_it->second.size();

                l_subtree_entry->add_coverage(l_it->second);

            }

            // The unprocessed coverage of the lineage of literals leading up to this node
            // is no longer useful. Erase it to preserve memory.
            m_unprocessed_coverage.clear();

        }

        std::string file_name(
            
        )
        {
            std::string l_file_name;

            for (const literal& l_literal : m_covering_literals)
            {
                if (l_literal.invert())
                {
                    l_file_name += '-';
                }

                l_file_name += std::to_string(l_literal.index());
                
                l_file_name += '_';
            }
            
            l_file_name += ".bin";

            return l_file_name;

        }

    };

    /// @brief Provides methods for managing unsatisfying coverage trees,
    ///        as well as the satisfying input paths vector. Also provides
    ///        functionality for creating a generalizing sum.
    class output_bit_manager
    {
    private:
        // Static fields.
        static std::string s_tree_root_file_name;
        static std::string s_satisfying_input_paths_file_name;

        // Member variables which are defined at construction time
        std::filesystem::path m_output_bit_folder_path;
        cache<std::filesystem::path, input>& m_input_cache;
        cache<std::filesystem::path, unsatisfying_coverage_tree> m_tree_cache;

        // Member variables which are defined on import from file.
        std::vector<std::filesystem::path> m_satisfying_input_paths;

    public:
        /// @brief On destruction, the satisfying input paths vector will be written to file.
        virtual ~output_bit_manager(

        )
        {
            // On destruction, export the satisfying vector of input paths to file.

            std::ofstream l_ofs(m_output_bit_folder_path / s_satisfying_input_paths_file_name, std::ios::binary);

            cereal::BinaryOutputArchive l_archive(l_ofs);

            l_archive(m_satisfying_input_paths);

            l_ofs.close();

        }

        /// @brief On construction, the tree cache is created, and the satisfying input path
        ///        vector is imported from file.
        /// @param a_output_bit_folder_path 
        /// @param a_input_cache 
        output_bit_manager(
            const std::filesystem::path& a_output_bit_folder_path,
            cache<std::filesystem::path, input>& a_input_cache
        ) :
            m_output_bit_folder_path(a_output_bit_folder_path),
            m_input_cache(a_input_cache),
            m_tree_cache(TREE_CACHE_SIZE,
                [&, a_output_bit_folder_path](
                    const std::filesystem::path& a_path
                )
                {
                    std::ifstream l_ifs(m_output_bit_folder_path / a_path, std::ios::binary);
                    
                    cereal::BinaryInputArchive l_archive(l_ifs);

                    unsatisfying_coverage_tree l_tree(a_input_cache);

                    l_archive(l_tree);

                    l_ifs.close();

                    return std::move(l_tree);

                },
                [&, a_output_bit_folder_path](
                    const std::filesystem::path& a_path, const unsatisfying_coverage_tree& a_tree
                )
                {
                    std::ofstream l_ofs(m_output_bit_folder_path / a_path, std::ios::binary);

                    cereal::BinaryOutputArchive l_archive(l_ofs);

                    l_archive(a_tree);

                    l_ofs.close();

                }
            )
        {
            if (!std::filesystem::exists(a_output_bit_folder_path))
            {
                // If the local folder does not exist, create it, then return. Do not try to load anything.
                std::filesystem::create_directory(a_output_bit_folder_path);

                // Create the root node of the unsatisfying coverage tree.
                m_tree_cache.insert(
                    s_tree_root_file_name,
                    unsatisfying_coverage_tree(a_input_cache)
                );
                
                return;

            }

            // If the file does exist, load the satisfying input paths vector.
            
            std::ifstream l_ifs(a_output_bit_folder_path / s_satisfying_input_paths_file_name, std::ios::binary);
            
            cereal::BinaryInputArchive l_archive(l_ifs);

            l_archive(m_satisfying_input_paths);

            l_ifs.close();

        }

        /// @brief This function adds satisfying input paths to a vector which exists in LTS. 
        /// @param a_satisfying_input_paths 
        void add_satisfying_coverage(
            const std::vector<std::filesystem::path>& a_satisfying_input_paths
        )
        {
            // Push the satisfying inputs onto the list.
            m_satisfying_input_paths.insert(
                m_satisfying_input_paths.end(),
                a_satisfying_input_paths.begin(),
                a_satisfying_input_paths.end());

        }

        /// @brief This function adds unsatisfying coverage to the tree which exists in LTS.
        /// @param a_unsatisfying_input_paths 
        void add_unsatisfying_coverage(
            const std::vector<std::filesystem::path>& a_unsatisfying_input_paths
        )
        {
            cache<std::filesystem::path, unsatisfying_coverage_tree>::entry l_tree =
                m_tree_cache.get(s_tree_root_file_name);

            l_tree->add_coverage(a_unsatisfying_input_paths);

        }

        /// @brief Gets a sum of products which will cover all satisfying inputs.
        ///        This will not cover any unsatisfying inputs. It will however cover some unresolved inputs.
        ///        This is where the "generalization" occurred.
        /// @param a_partitioned_example_set
        /// @return
        sum_of_products generalize(
            
        )
        {
            std::vector<literal_product> l_covering_products;
            
            for (int i = 0; i < m_satisfying_input_paths.size(); i++)
            {
                l_covering_products.push_back(
                    covering_product(
                        s_tree_root_file_name,
                        m_satisfying_input_paths[i]
                    ));
            }

            return sum_of_products(l_covering_products);

        }

    private:
        /// @brief This function retrieves a covering product with few literals to use
        ///        in the covering sum.
        /// @param a_tree_path 
        /// @param a_satisfying_input_path 
        /// @return 
        literal_product covering_product(
            const std::string& a_tree_file_name,
            const std::filesystem::path& a_satisfying_input_path
        )
        {
            std::filesystem::path l_coverage_minimizing_tree_file_name;

            // LEAVE THIS IT'S OWN SCOPE. IT IS IMPORTANT
            // We want the cache::entry types to fall out of scope as that will unlock
            // the actual entries in the cache. If we do not do this before
            // Continuing the recursion, then all nodes used in the recursion will have their
            // respective entries locked in cache.
            {
                // Get the input from cache
                cache<std::filesystem::path, input>::entry l_satisfying_input = 
                    m_input_cache.get(a_satisfying_input_path);

                // Get this node from cache.
                cache<std::filesystem::path, unsatisfying_coverage_tree>::entry l_tree = 
                    m_tree_cache.get(a_tree_file_name);

                if (l_tree->coverage_size() == 0)
                {
                    // This node holds information about the minimally covering product.
                    return literal_product(l_tree->covering_literals());
                }

                l_coverage_minimizing_tree_file_name = l_tree->coverage_minimizing_subtree_file_name(m_tree_cache, l_satisfying_input);

            }

            // Use recursion.
            return covering_product(l_coverage_minimizing_tree_file_name, a_satisfying_input_path);

        }

    };

    // Define static fields of output_bit_manager
    std::string output_bit_manager::s_tree_root_file_name("root.bin");
    std::string output_bit_manager::s_satisfying_input_paths_file_name("satisfying-input-paths.bin");

    /// @brief This class is responsible for maintaining
    ///        all input files, and utilizing the
    ///        output_bit_manager class to create generalizing sums of products.
    class solution_manager
    {
    private:
        // Static fields.
        static std::string s_batch_index_file_name;

        // Member variables defined at construction-time
        std::filesystem::path m_solution_path;
        cache<std::filesystem::path, input> m_input_cache;
        size_t m_output_bit_count = 0;

        // Member variables defined by importing from file.
        size_t m_batch_index = 0;
        
    public:
        /// @brief The destruction of this object will cause the member variable m_batch_index to be
        ///        written to file. Then, in the future it will be recalled from LTS.
        virtual ~solution_manager(

        )
        {
            std::ofstream l_ofs(m_solution_path / i_batch_index_file_name, std::ios::binary);

            cereal::BinaryOutputArchive l_archive(l_ofs);

            l_archive(m_batch_index);

            l_ofs.close();

        }

        /// @brief Constructs the input cache, and imports the variable m_batch_size from file.
        /// @param a_solution_path 
        /// @param a_output_bit_count 
        solution_manager(
            const std::filesystem::path& a_solution_path,
            const size_t& a_output_bit_count
        ) :
            m_solution_path(a_solution_path),
            m_output_bit_count(a_output_bit_count),
            m_input_cache(
                INPUT_CACHE_SIZE,
                [&, a_solution_path](
                    const std::filesystem::path& a_path
                )
                {
                    std::ifstream l_ifs(a_solution_path / a_path, std::ios::binary);

                    cereal::BinaryInputArchive l_archive(l_ifs);

                    input l_input;

                    l_archive(l_input);

                    l_ifs.close();

                    return std::move(l_input);

                },
                [&, a_solution_path](
                    const std::filesystem::path& a_path, const input& a_input
                )
                {
                    std::ofstream l_ofs(a_solution_path / a_path, std::ios::binary);

                    cereal::BinaryOutputArchive l_archive(l_ofs);

                    l_archive(a_input);

                    l_ofs.close();

                }
            )
        {
            if (!std::filesystem::exists(a_solution_path))
            {
                // Create the directory if it does not already exist.
                std::filesystem::create_directory(a_solution_path);
                return;
            }

            std::ifstream l_ifs(a_solution_path / i_batch_index_file_name, std::ios::binary);

            cereal::BinaryInputArchive l_archive(l_ifs);

            l_archive(m_batch_index);

            l_ifs.close();

        }

        /// @brief Writes the inputs to unique files, detecting for each output bit
        ///        whether or not the input is a satisfying or unsatisfying input,
        ///        and distributes the path to those inputs properly to each output_bit_manager.
        /// @param a_raw_examples 
        void add_examples(
            const std::vector<raw_example>& a_raw_examples
        )
        {
            // Create the vectors which will hold the new additions of input paths.
            std::vector<std::vector<std::filesystem::path>> l_unsatisfying_input_paths(m_output_bit_count);
            std::vector<std::vector<std::filesystem::path>> l_satisfying_input_paths(m_output_bit_count);

            for (int i = 0; i < a_raw_examples.size(); i++)
            {
                const raw_example& l_raw_example = a_raw_examples[i];

                // Create the unique file name for the input.
                std::filesystem::path l_input_path =
                    (std::to_string(m_batch_index) + "_" + std::to_string(i) + ".bin");

                // Insert the file name, input pair into the cache. Once this cache
                // falls out of scope, the file name, input pair will be written to disk.
                m_input_cache.insert(l_input_path, l_raw_example.m_input);

                for (int j = 0; j < m_output_bit_count; j++)
                {
                    if (l_raw_example.m_output[j])
                    {
                        // Satisfying input
                        l_satisfying_input_paths[j].push_back(l_input_path);
                    }
                    else
                    {
                        // Unsatisfying input
                        l_unsatisfying_input_paths[j].push_back(l_input_path);
                    }
                }

            }

            for (int i = 0; i < m_output_bit_count; i++)
            {
                // Add the satisfying and unsatisfying coverage to each output bit manager.
                output_bit_manager l_output_bit_manager = output_bit(i);
                l_output_bit_manager.add_satisfying_coverage(l_satisfying_input_paths[i]);
                l_output_bit_manager.add_unsatisfying_coverage(l_unsatisfying_input_paths[i]);
            }

            // Increment the batch index to prevent file name conflicts
            // for future example additions.
            m_batch_index++;

        }

        /// @brief This function forces each output_bit_manager to build a generalizing sum of products (SOP),
        ///        which it will then concatenate into a final multi-output function.
        /// @return 
        sum_of_products_string generalize(
            
        )
        {
            std::vector<sum_of_products> l_sums_of_products;

            for (int i = 0; i < m_output_bit_count; i++)
            {
                l_sums_of_products.push_back(
                    output_bit(i).generalize()
                );
            }

            return sum_of_products_string(l_sums_of_products);

        }

    private:
        output_bit_manager output_bit(
            const size_t& a_index
        )
        {
            return output_bit_manager(
                m_solution_path / std::to_string(a_index),
                m_input_cache
            );
        }

    };

    // Define static fields of solution_manager
    std::string solution_manager::s_batch_index_file_name("batch-index.bin");

}

#endif
