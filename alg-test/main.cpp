#include "boolean-ai/generalize.h"
#include <iostream>
#include <assert.h>
#include <numeric>

size_t boolean_ai::INPUT_CACHE_SIZE = 1000;
size_t boolean_ai::TREE_CACHE_SIZE = 1000;

void test_literal_product_equivalence(

)
{
    {
        boolean_ai::literal_product l_literal_product_0({
            boolean_ai::literal(0, false),
            boolean_ai::literal(1, false),
            boolean_ai::literal(2, false),});
        boolean_ai::literal_product l_literal_product_1({
            boolean_ai::literal(0, false),
            boolean_ai::literal(2, false),
            boolean_ai::literal(1, false),});
        assert(l_literal_product_0 == l_literal_product_1);
    }

    {
        boolean_ai::literal_product l_literal_product_0({
            boolean_ai::literal(0, false),
            boolean_ai::literal(1, false),
            boolean_ai::literal(2, false),});
        boolean_ai::literal_product l_literal_product_1({
            boolean_ai::literal(2, false),
            boolean_ai::literal(1, false),});
        assert(l_literal_product_0 != l_literal_product_1);
    }

    {
        boolean_ai::literal_product l_literal_product_0({
            boolean_ai::literal(0, false),
            boolean_ai::literal(3, true),
            boolean_ai::literal(2, false),});
        boolean_ai::literal_product l_literal_product_1({
            boolean_ai::literal(3, true),
            boolean_ai::literal(2, false),
            boolean_ai::literal(0, false),});
        assert(l_literal_product_0 == l_literal_product_1);
    }

    {
        boolean_ai::literal_product l_literal_product_0({
            boolean_ai::literal(0, false),
            boolean_ai::literal(3, true),
            boolean_ai::literal(2, false)
        });
        boolean_ai::literal_product l_literal_product_1({
            boolean_ai::literal(3, false),
            boolean_ai::literal(2, false),
            boolean_ai::literal(0, false)
        });
        assert(l_literal_product_0 != l_literal_product_1);
    }
}

void test_cache(

)
{
    namespace fs = std::filesystem;

    // Construct and use the cache inside its own scope.

    {
        cache<fs::path, std::string> l_cache(
            3,
            [&](const fs::path& a_path)
            {
                std::ifstream l_ifs(a_path, std::ios::binary);

                cereal::BinaryInputArchive l_archive(l_ifs);

                std::string l_result;

                l_archive(l_result);

                l_ifs.close();

                return std::move(l_result);

            },
            [&](const fs::path& a_path, const std::string& a_string)
            {
                std::ofstream l_ofs(a_path, std::ios::binary);

                cereal::BinaryOutputArchive l_archive(l_ofs);

                l_archive(a_string);

                l_ofs.close();

            }
        );

        try
        {
            // Test lookup of non-existent key
            l_cache.get("testing123.bin");
            throw std::runtime_error("Error: lookup of non-existent key worked.");
        }
        catch(std::exception)
        { }

        {
            // Test insertion

            cache<fs::path, std::string>::entry l_insertion_0 = l_cache.insert("testing1.bin", "jake1");
            
            // Test modification of the entry returned by the insertion.
            l_insertion_0->push_back('1');

            l_cache.insert("testing2.bin", "jake2");
            l_cache.insert("testing3.bin", "jake3");

            // This file should not yet exist. Only after the size
            // of the cache has been exceeded will it be created.
            assert(!fs::exists("testing2.bin"));

            // On this insertion, the least recently accessed (inserted) entry should be pushed to LTS,
            // since the maximum size of the cache is 3 and this will be the 4th insertion.
            l_cache.insert("testing4.bin", "jake4");

            // This file should now exist.
            // The reason why testing1.bin would not have been created is because we still have
            // an instance of a cache::entry pointing to that entry, locking it.
            assert(fs::exists("testing2.bin"));
            
            l_cache.insert("testing5.bin", "jake5");

        }

        {
            // Test lookup

            cache<fs::path, std::string>::entry l_lookup_0 = l_cache.get("testing1.bin");

            assert(*l_lookup_0 == "jake11");

            cache<fs::path, std::string>::entry l_lookup_1 = l_cache.get("testing2.bin");

            assert(*l_lookup_1 == "jake2");

            cache<fs::path, std::string>::entry l_lookup_2 = l_cache.get("testing5.bin");

            assert(*l_lookup_2 == "jake5");

        }

    }

    // Since cache was destructed, all contents should exist in LTS.
    
    assert(fs::exists("testing1.bin"));
    assert(fs::exists("testing2.bin"));
    assert(fs::exists("testing3.bin"));
    assert(fs::exists("testing4.bin"));
    assert(fs::exists("testing5.bin"));

    // Clean up after unit testing.
    fs::remove("testing1.bin");
    fs::remove("testing2.bin");
    fs::remove("testing3.bin");
    fs::remove("testing4.bin");
    fs::remove("testing5.bin");

}

void test_generalize_multi_output(

)
{
    std::vector<boolean_ai::raw_example> l_raw_examples =
    {
        {{0, 0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0, 1}, {0, 0, 1}},
        {{0, 0, 1, 1}, {0, 1, 1}},
        {{0, 1, 1, 1}, {0, 1, 0}},
        {{1, 0, 0, 1}, {1, 1, 0}},
        {{1, 0, 1, 0}, {1, 1, 0}},
        {{1, 0, 1, 1}, {0, 1, 0}},
        {{1, 1, 1, 0}, {1, 1, 0}},
        {{1, 1, 1, 1}, {0, 1, 0}},
    };

    boolean_ai::sum_of_products_string l_generalized;

    {
        boolean_ai::solution_manager l_solution_manager("example-solution/", 3);

        l_solution_manager.add_examples(l_raw_examples);

        // This call takes into account all satisfying inputs and ALL unsatisfying inputs.
        l_generalized = l_solution_manager.generalize();
    }

    assert(std::filesystem::exists("example-solution/"));
    assert(std::filesystem::exists("example-solution/0"));

    // There should be three covering products since there are three satisfying inputs for
    // output bit 0.
    assert(l_generalized.sums_of_products()[0].literal_products().size() == 3);

    assert(l_generalized.sums_of_products()[0].literal_products()[0].literals() == std::vector({
        boolean_ai::literal(0, false), boolean_ai::literal(2, true)
    }));
    
    assert(l_generalized.sums_of_products()[0].literal_products()[1].literals() == std::vector({
        boolean_ai::literal(3, true), boolean_ai::literal(0, false)
    }));

    assert(l_generalized.sums_of_products()[0].literal_products()[2].literals() == std::vector({
        boolean_ai::literal(3, true), boolean_ai::literal(0, false)
    }));

    // There should be seven covering products since there are seven satisfying inputs for
    // output bit 1.
    assert(l_generalized.sums_of_products()[1].literal_products().size() == 7);

    assert(l_generalized.sums_of_products()[1].literal_products()[0].literals() == std::vector({
        boolean_ai::literal(2, false)
    }));
    
    assert(l_generalized.sums_of_products()[1].literal_products()[1].literals() == std::vector({
        boolean_ai::literal(1, false)
    }));

    assert(l_generalized.sums_of_products()[1].literal_products()[2].literals() == std::vector({
        boolean_ai::literal(0, false)
    }));

    assert(l_generalized.sums_of_products()[1].literal_products()[3].literals() == std::vector({
        boolean_ai::literal(0, false)
    }));

    assert(l_generalized.sums_of_products()[1].literal_products()[4].literals() == std::vector({
        boolean_ai::literal(0, false)
    }));

    assert(l_generalized.sums_of_products()[1].literal_products()[5].literals() == std::vector({
        boolean_ai::literal(0, false)
    }));

    assert(l_generalized.sums_of_products()[1].literal_products()[6].literals() == std::vector({
        boolean_ai::literal(0, false)
    }));

    // There should be two covering products since there are two satisfying inputs for
    // output bit 2.
    assert(l_generalized.sums_of_products()[2].literal_products().size() == 2);

    assert(l_generalized.sums_of_products()[2].literal_products()[0].literals() == std::vector({
        boolean_ai::literal(0, true), boolean_ai::literal(1, true), boolean_ai::literal(3, false)
    }));
    
    assert(l_generalized.sums_of_products()[2].literal_products()[1].literals() == std::vector({
        boolean_ai::literal(0, true), boolean_ai::literal(1, true), boolean_ai::literal(2, false)
    }));


    l_generalized.simplify();

    assert(l_generalized.sums_of_products()[0].literal_products().size() == 2);
    assert(l_generalized.sums_of_products()[1].literal_products().size() == 3);
    assert(l_generalized.sums_of_products()[2].literal_products().size() == 2);

    std::filesystem::remove_all("example-solution/");

}

void unit_test_main(

)
{
    test_literal_product_equivalence();
    test_cache();
    test_generalize_multi_output();
}

void add_8_bit_numbers_test(
    
)
{
    auto l_random_byte = [](

    )
    {
        std::vector<bool> l_result(8);
        for (int i = 0; i < l_result.size(); i++)
            l_result[i] = (rand() % 2);
        return l_result;
    };

    auto l_sum_and_carry = [](
        const std::vector<bool>& a_byte_0,
        const std::vector<bool>& a_byte_1
    )
    {
        std::vector<bool> l_result(9);
        bool l_c = false;
        for (int i = a_byte_0.size() - 1; i >= 0; i--)
        {
            bool l_s = 
                !a_byte_0[i] && !a_byte_1[i] && l_c ||
                !a_byte_0[i] && a_byte_1[i] && !l_c ||
                a_byte_0[i] && !a_byte_1[i] && !l_c ||
                a_byte_0[i] && a_byte_1[i] && l_c;
            l_result[i] = l_s;
            l_c =
                !a_byte_0[i] && a_byte_1[i] && l_c ||
                a_byte_0[i] && !a_byte_1[i] && l_c ||
                a_byte_0[i] && a_byte_1[i] && !l_c ||
                a_byte_0[i] && a_byte_1[i] && l_c;
        }
        l_result.back() = l_c;
        return l_result;
    };

    // Input is 16-bit (two 8-bit strings)
    // Output is 9-bit (8-bit sum and 1-bit carry)

    for (size_t l_training_sets_count = 1000; l_training_sets_count < 100000; l_training_sets_count += 1000)
    {
        std::vector<boolean_ai::raw_example> l_raw_examples;

        for (int i = 0; i < l_training_sets_count; i++)
        {
            std::vector<bool> l_byte_0 = l_random_byte();
            std::vector<bool> l_byte_1 = l_random_byte();

            std::vector<bool> l_concatenated_input;
            l_concatenated_input.insert(l_concatenated_input.end(), l_byte_0.begin(), l_byte_0.end());
            l_concatenated_input.insert(l_concatenated_input.end(), l_byte_1.begin(), l_byte_1.end());

            std::vector<bool> l_desired = l_sum_and_carry(l_byte_0, l_byte_1);

            l_raw_examples.push_back({l_concatenated_input, l_desired});

        }

        auto l_start =  std::chrono::high_resolution_clock::now();

        boolean_ai::sum_of_products_string l_sops;

        {
            boolean_ai::solution_manager l_solution_manager("add-8-bit-numbers/", 9);

            l_solution_manager.add_examples(l_raw_examples);

            l_sops = l_solution_manager.generalize();
        }
        
        auto l_stop =  std::chrono::high_resolution_clock::now();

        // Delete the solution directory
        std::filesystem::remove_all("add-8-bit-numbers/");

        auto l_duration = std::chrono::duration_cast<std::chrono::milliseconds>(l_stop - l_start).count();

        std::cout << "TRAINING SETS                : " << l_training_sets_count << std::endl;
        std::cout << "DURATION OF GENERALIZATION   : " << l_duration << std::endl;
        std::cout << "TRAINING SETS PER MILLISECOND: " << (double)l_training_sets_count / (double)l_duration << std::endl;

        l_sops.simplify();

        std::cout << l_sops.sums_of_products()[0].literal_products()[0].literals().size() << std::endl;

        const size_t l_number_of_tests = 3;

        for (int i = 0; i < l_number_of_tests; i++)
        {
            std::vector<bool> l_test_byte_0 = l_random_byte();
            std::vector<bool> l_test_byte_1 = l_random_byte();

            std::vector<bool> l_concatenated;
            l_concatenated.insert(l_concatenated.end(), l_test_byte_0.begin(), l_test_byte_0.end());
            l_concatenated.insert(l_concatenated.end(), l_test_byte_1.begin(), l_test_byte_1.end());

            std::vector<bool> l_evaluated = l_sops.evaluate(l_concatenated);

            for (const bool& l_bool : l_test_byte_0)
                std::cout << l_bool << " ";
            std::cout << std::endl;

            for (const bool& l_bool : l_test_byte_1)
                std::cout << l_bool << " ";
            std::cout << std::endl;

            for (const bool& l_bool : l_evaluated)
                std::cout << l_bool << " ";
            std::cout << std::endl << std::endl;

        }

    }

    // std::cout << "yo test" << std::endl;

}

int main(

)
{
    //add_8_bit_numbers_test();
    unit_test_main();

    return 0;

}
