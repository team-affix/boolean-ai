#include "boolean-ai/generalize.h"
#include <iostream>
#include <assert.h>
#include <numeric>
#include "boolean-ai/cache.h"
#include <fstream>
#include "cereal/archives/binary.hpp"

void test_literal_product_equivalence(

)
{
    {
        digital_ai::literal_product l_literal_product_0({
            digital_ai::literal(0, false),
            digital_ai::literal(1, false),
            digital_ai::literal(2, false),});
        digital_ai::literal_product l_literal_product_1({
            digital_ai::literal(0, false),
            digital_ai::literal(2, false),
            digital_ai::literal(1, false),});
        assert(l_literal_product_0 == l_literal_product_1);
    }

    {
        digital_ai::literal_product l_literal_product_0({
            digital_ai::literal(0, false),
            digital_ai::literal(1, false),
            digital_ai::literal(2, false),});
        digital_ai::literal_product l_literal_product_1({
            digital_ai::literal(2, false),
            digital_ai::literal(1, false),});
        assert(l_literal_product_0 != l_literal_product_1);
    }

    {
        digital_ai::literal_product l_literal_product_0({
            digital_ai::literal(0, false),
            digital_ai::literal(3, true),
            digital_ai::literal(2, false),});
        digital_ai::literal_product l_literal_product_1({
            digital_ai::literal(3, true),
            digital_ai::literal(2, false),
            digital_ai::literal(0, false),});
        assert(l_literal_product_0 == l_literal_product_1);
    }

    {
        digital_ai::literal_product l_literal_product_0({
            digital_ai::literal(0, false),
            digital_ai::literal(3, true),
            digital_ai::literal(2, false)
        });
        digital_ai::literal_product l_literal_product_1({
            digital_ai::literal(3, false),
            digital_ai::literal(2, false),
            digital_ai::literal(0, false)
        });
        assert(l_literal_product_0 != l_literal_product_1);
    }
}

void test_data_partitioning(

)
{
    std::vector<digital_ai::raw_example> l_raw_examples =
    {
        {{0, 0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0, 1}, {0, 0, 1}},
        {{0, 0, 1, 0}, {0, 1, 0}},
        {{0, 0, 1, 1}, {0, 1, 1}},
        {{0, 1, 0, 0}, {0, 1, 0}},
        {{0, 1, 0, 1}, {0, 1, 0}},
        {{0, 1, 1, 0}, {1, 1, 0}},
        {{0, 1, 1, 1}, {0, 1, 0}},
        {{1, 0, 0, 0}, {1, 1, 0}},
        {{1, 0, 0, 1}, {1, 1, 0}},
        {{1, 0, 1, 0}, {1, 1, 0}},
        {{1, 0, 1, 1}, {0, 1, 0}},
        {{1, 1, 0, 0}, {1, 1, 0}},
        {{1, 1, 0, 1}, {1, 1, 1}},
        {{1, 1, 1, 0}, {1, 1, 0}},
        {{1, 1, 1, 1}, {0, 1, 0}},
    };

    {
        auto l_first_satisfying = &l_raw_examples[6].m_input;
        auto l_last_satisfying = &l_raw_examples[14].m_input;

        auto l_first_unsatisfying = &l_raw_examples[0].m_input;
        auto l_last_unsatisfying = &l_raw_examples[15].m_input;

        digital_ai::partitioned_example_set l_partitioned_example_set(
            l_raw_examples,
            0
        );
        
        assert((std::vector<bool>*)l_partitioned_example_set.m_satisfying_inputs.front() == l_first_satisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_satisfying_inputs.back() == l_last_satisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_unsatisfying_inputs.front() == l_first_unsatisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_unsatisfying_inputs.back() == l_last_unsatisfying);

    }

    {
        auto l_first_satisfying = &l_raw_examples[2].m_input;
        auto l_last_satisfying = &l_raw_examples[15].m_input;

        auto l_first_unsatisfying = &l_raw_examples[0].m_input;
        auto l_last_unsatisfying = &l_raw_examples[1].m_input;

        digital_ai::partitioned_example_set l_partitioned_example_set(
            l_raw_examples,
            1
        );
        
        assert((std::vector<bool>*)l_partitioned_example_set.m_satisfying_inputs.front() == l_first_satisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_satisfying_inputs.back() == l_last_satisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_unsatisfying_inputs.front() == l_first_unsatisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_unsatisfying_inputs.back() == l_last_unsatisfying);

    }

    {
        auto l_first_satisfying = &l_raw_examples[1].m_input;
        auto l_last_satisfying = &l_raw_examples[13].m_input;

        auto l_first_unsatisfying = &l_raw_examples[0].m_input;
        auto l_last_unsatisfying = &l_raw_examples[15].m_input;

        digital_ai::partitioned_example_set l_partitioned_example_set(
            l_raw_examples,
            2
        );
        
        assert((std::vector<bool>*)l_partitioned_example_set.m_satisfying_inputs.front() == l_first_satisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_satisfying_inputs.back() == l_last_satisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_unsatisfying_inputs.front() == l_first_unsatisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_unsatisfying_inputs.back() == l_last_unsatisfying);

    }

}

void test_get_covering_product(

)
{
    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{1, 0, 0, 1}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{1, 0, 1, 0}, {1, 1, 0}},
            {{1, 0, 1, 1}, {0, 1, 0}},
            {{1, 1, 1, 0}, {1, 1, 0}},
            {{1, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        digital_ai::literal_coverage_tree l_literal_coverage_tree;

        l_literal_coverage_tree.add_coverage(l_partitioned_example_set.m_unsatisfying_inputs);

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = l_literal_coverage_tree.covering_product(
            l_partitioned_example_set.m_satisfying_inputs[0]);
    
        assert(l_covering_product.literals() == std::vector({ digital_ai::literal(0, false), digital_ai::literal(2, true) }));
    
    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{1, 0, 0, 1}, {1, 1, 0}}, 
            {{1, 0, 1, 0}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{1, 0, 1, 1}, {0, 1, 0}},
            {{1, 1, 1, 0}, {1, 1, 0}},
            {{1, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        digital_ai::literal_coverage_tree l_literal_coverage_tree;

        l_literal_coverage_tree.add_coverage(l_partitioned_example_set.m_unsatisfying_inputs);

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = l_literal_coverage_tree.covering_product(
            l_partitioned_example_set.m_satisfying_inputs[1]);
    
        assert(l_covering_product.literals() == std::vector({ digital_ai::literal(3, true), digital_ai::literal(0, false) }));
    
    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{1, 0, 0, 1}, {1, 1, 0}}, 
            {{1, 0, 1, 0}, {1, 1, 0}},
            {{1, 0, 1, 1}, {0, 1, 0}},
            {{1, 1, 1, 0}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{1, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        digital_ai::literal_coverage_tree l_literal_coverage_tree;

        l_literal_coverage_tree.add_coverage(l_partitioned_example_set.m_unsatisfying_inputs);

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = l_literal_coverage_tree.covering_product(
            l_partitioned_example_set.m_satisfying_inputs[2]);
    
        assert(l_covering_product.literals() == std::vector({ digital_ai::literal(3, true), digital_ai::literal(0, false) }));
    
    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 0}, {0, 1, 0}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 0, 0}, {0, 1, 0}},
            {{0, 1, 0, 1}, {0, 1, 0}},
            {{0, 1, 1, 0}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{1, 0, 0, 0}, {1, 1, 0}},
            {{1, 0, 0, 1}, {1, 1, 0}},
            {{1, 0, 1, 0}, {1, 1, 0}},
            {{1, 0, 1, 1}, {0, 1, 0}},
            {{1, 1, 0, 0}, {1, 1, 0}},
            {{1, 1, 0, 1}, {1, 1, 1}},
            {{1, 1, 1, 0}, {1, 1, 0}},
            {{1, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        digital_ai::literal_coverage_tree l_literal_coverage_tree;

        l_literal_coverage_tree.add_coverage(l_partitioned_example_set.m_unsatisfying_inputs);

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = l_literal_coverage_tree.covering_product(
            l_partitioned_example_set.m_satisfying_inputs[0]);
    
        assert(l_covering_product.literals() == std::vector({ digital_ai::literal(3, true), digital_ai::literal(1, false), digital_ai::literal(2, false) }));
    
    }

    {
        // In this example, the only position in which there is a satisfying input is when minterm 0'123' is satisfied.
        // Therefore, what we derive should equal this minterm. No generalization can take place here.
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 0}, {0, 1, 0}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 0, 0}, {0, 1, 0}},
            {{0, 1, 0, 1}, {0, 1, 0}},
            {{0, 1, 1, 0}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{1, 0, 0, 0}, {0, 1, 0}},
            {{1, 0, 0, 1}, {0, 1, 0}},
            {{1, 0, 1, 0}, {0, 1, 0}},
            {{1, 0, 1, 1}, {0, 1, 0}},
            {{1, 1, 0, 0}, {0, 1, 0}},
            {{1, 1, 0, 1}, {0, 1, 1}},
            {{1, 1, 1, 0}, {0, 1, 0}},
            {{1, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        digital_ai::literal_coverage_tree l_literal_coverage_tree;

        l_literal_coverage_tree.add_coverage(l_partitioned_example_set.m_unsatisfying_inputs);

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = l_literal_coverage_tree.covering_product(
            l_partitioned_example_set.m_satisfying_inputs[0]);
    
        assert(l_covering_product.literals() == 
            std::vector({ 
                digital_ai::literal(0, true),
                digital_ai::literal(1, false),
                digital_ai::literal(2, false),
                digital_ai::literal(3, true) }));
    
    }

    {
        // In this example, the only position in which there is a satisfying input is when minterm 0'123' is satisfied.
        // Therefore, what we derive should equal this minterm. No generalization can take place here.
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 0}, {0, 1, 0}}, // Using this one as our satisfying example of focus
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 0, 0}, {0, 1, 0}},
            {{0, 1, 0, 1}, {0, 1, 0}},
            {{0, 1, 1, 0}, {1, 1, 0}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{1, 0, 0, 0}, {0, 1, 0}},
            {{1, 0, 0, 1}, {0, 1, 0}},
            {{1, 0, 1, 0}, {0, 1, 0}},
            {{1, 0, 1, 1}, {0, 1, 0}},
            {{1, 1, 0, 0}, {0, 1, 0}},
            {{1, 1, 0, 1}, {0, 1, 1}},
            {{1, 1, 1, 0}, {0, 1, 0}},
            {{1, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 1);

        digital_ai::literal_coverage_tree l_literal_coverage_tree;

        l_literal_coverage_tree.add_coverage(l_partitioned_example_set.m_unsatisfying_inputs);

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = l_literal_coverage_tree.covering_product(
            l_partitioned_example_set.m_satisfying_inputs[0]);
    
        assert(l_covering_product.literals() == 
            std::vector({ 
                digital_ai::literal(2, false) }));
    
    }

}

void test_generalize(

)
{
    {
        std::vector<digital_ai::raw_example> l_raw_examples =
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

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        // This call takes into account all satisfying inputs and ALL unsatisfying inputs.
        digital_ai::sum_of_products l_generalized = digital_ai::generalize(
            l_partitioned_example_set);
    
        // There should be three covering products since there are three satisfying inputs for
        // output bit 0.
        assert(l_generalized.literal_products().size() == 3);

        assert(l_generalized.literal_products()[0].literals() == std::vector({
            digital_ai::literal(0, false), digital_ai::literal(2, true)
        }));
        
        assert(l_generalized.literal_products()[1].literals() == std::vector({
            digital_ai::literal(3, true), digital_ai::literal(0, false)
        }));

        assert(l_generalized.literal_products()[2].literals() == std::vector({
            digital_ai::literal(3, true), digital_ai::literal(0, false)
        }));
    
    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
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

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 1);

        // This call takes into account all satisfying inputs and ALL unsatisfying inputs.
        digital_ai::sum_of_products l_generalized = digital_ai::generalize(l_partitioned_example_set);
    
        // There should be seven covering products since there are seven satisfying inputs for
        // output bit 1.
        assert(l_generalized.literal_products().size() == 7);

        assert(l_generalized.literal_products()[0].literals() == std::vector({
            digital_ai::literal(2, false)
        }));
        
        assert(l_generalized.literal_products()[1].literals() == std::vector({
            digital_ai::literal(1, false)
        }));

        assert(l_generalized.literal_products()[2].literals() == std::vector({
            digital_ai::literal(0, false)
        }));

        assert(l_generalized.literal_products()[3].literals() == std::vector({
            digital_ai::literal(0, false)
        }));

        assert(l_generalized.literal_products()[4].literals() == std::vector({
            digital_ai::literal(0, false)
        }));

        assert(l_generalized.literal_products()[5].literals() == std::vector({
            digital_ai::literal(0, false)
        }));

        assert(l_generalized.literal_products()[6].literals() == std::vector({
            digital_ai::literal(0, false)
        }));
    
    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
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

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 2);

        // This call takes into account all satisfying inputs and ALL unsatisfying inputs.
        digital_ai::sum_of_products l_generalized = digital_ai::generalize(l_partitioned_example_set);
    
        // There should be two covering products since there are two satisfying inputs for
        // output bit 2.
        assert(l_generalized.literal_products().size() == 2);

        assert(l_generalized.literal_products()[0].literals() == std::vector({
            digital_ai::literal(0, true), digital_ai::literal(1, true), digital_ai::literal(3, false)
        }));
        
        assert(l_generalized.literal_products()[1].literals() == std::vector({
            digital_ai::literal(0, true), digital_ai::literal(1, true), digital_ai::literal(2, false)
        }));

    }
}

void test_generalize_multi_output(

)
{
    std::vector<digital_ai::raw_example> l_raw_examples =
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

    // This call takes into account all satisfying inputs and ALL unsatisfying inputs.
    digital_ai::sum_of_products_string l_generalized = digital_ai::generalize(l_raw_examples);

    // There should be three covering products since there are three satisfying inputs for
    // output bit 0.
    assert(l_generalized.sums_of_products()[0].literal_products().size() == 3);

    assert(l_generalized.sums_of_products()[0].literal_products()[0].literals() == std::vector({
        digital_ai::literal(0, false), digital_ai::literal(2, true)
    }));
    
    assert(l_generalized.sums_of_products()[0].literal_products()[1].literals() == std::vector({
        digital_ai::literal(3, true), digital_ai::literal(0, false)
    }));

    assert(l_generalized.sums_of_products()[0].literal_products()[2].literals() == std::vector({
        digital_ai::literal(3, true), digital_ai::literal(0, false)
    }));

    // There should be seven covering products since there are seven satisfying inputs for
    // output bit 1.
    assert(l_generalized.sums_of_products()[1].literal_products().size() == 7);

    assert(l_generalized.sums_of_products()[1].literal_products()[0].literals() == std::vector({
        digital_ai::literal(2, false)
    }));
    
    assert(l_generalized.sums_of_products()[1].literal_products()[1].literals() == std::vector({
        digital_ai::literal(1, false)
    }));

    assert(l_generalized.sums_of_products()[1].literal_products()[2].literals() == std::vector({
        digital_ai::literal(0, false)
    }));

    assert(l_generalized.sums_of_products()[1].literal_products()[3].literals() == std::vector({
        digital_ai::literal(0, false)
    }));

    assert(l_generalized.sums_of_products()[1].literal_products()[4].literals() == std::vector({
        digital_ai::literal(0, false)
    }));

    assert(l_generalized.sums_of_products()[1].literal_products()[5].literals() == std::vector({
        digital_ai::literal(0, false)
    }));

    assert(l_generalized.sums_of_products()[1].literal_products()[6].literals() == std::vector({
        digital_ai::literal(0, false)
    }));

    // There should be two covering products since there are two satisfying inputs for
    // output bit 2.
    assert(l_generalized.sums_of_products()[2].literal_products().size() == 2);

    assert(l_generalized.sums_of_products()[2].literal_products()[0].literals() == std::vector({
        digital_ai::literal(0, true), digital_ai::literal(1, true), digital_ai::literal(3, false)
    }));
    
    assert(l_generalized.sums_of_products()[2].literal_products()[1].literals() == std::vector({
        digital_ai::literal(0, true), digital_ai::literal(1, true), digital_ai::literal(2, false)
    }));


    l_generalized.simplify();

    assert(l_generalized.sums_of_products()[0].literal_products().size() == 2);
    assert(l_generalized.sums_of_products()[1].literal_products().size() == 3);
    assert(l_generalized.sums_of_products()[2].literal_products().size() == 2);

}

void test_temporal_cache(

)
{
    namespace fs = std::filesystem;

    std::filesystem::path l_raw_example_0_path = std::filesystem::absolute("test_disk_lookup_0.bin");
    std::filesystem::path l_raw_example_1_path = std::filesystem::absolute("test_disk_lookup_1.bin");
    std::filesystem::path l_raw_example_2_path = std::filesystem::absolute("test_disk_lookup_2.bin");

    digital_ai::raw_example l_raw_example_0({{1, 0, 0, 1}, {0, 1, 1}});
    digital_ai::raw_example l_raw_example_1({{1, 1, 1, 1}, {0, 0, 0}});
    digital_ai::raw_example l_raw_example_2({{1, 1, 0, 1}, {0, 0, 1}});

    {
        // Serialize the first raw example
        std::ofstream l_ofs(l_raw_example_0_path, std::ios::binary);
        cereal::BinaryOutputArchive l_output_archive(l_ofs);
        l_output_archive(l_raw_example_0);
    }

    {
        // Serialize the second raw example
        std::ofstream l_ofs(l_raw_example_1_path, std::ios::binary);
        cereal::BinaryOutputArchive l_output_archive(l_ofs);
        l_output_archive(l_raw_example_1);
    }

    {
        // Serialize the third raw example
        std::ofstream l_ofs(l_raw_example_2_path, std::ios::binary);
        cereal::BinaryOutputArchive l_output_archive(l_ofs);
        l_output_archive(l_raw_example_2);
    }

    auto l_perform_disk_lookup = [&](
            const std::filesystem::path& a_file_path
        )
        {
            // Look up raw example set on disk.
            digital_ai::raw_example l_result;

            if (!fs::exists(a_file_path))
                throw std::runtime_error("Error: could not find file.");

            std::ifstream l_ifs(a_file_path.c_str(), std::ios::binary);
            
            cereal::BinaryInputArchive l_archive(l_ifs);

            l_archive(l_result);

            return l_result;

        };
        
    digital_ai::temporal_cache<std::filesystem::path, digital_ai::raw_example> l_raw_example_cache(
        l_perform_disk_lookup,
        2
    );

    digital_ai::raw_example l_raw_example_0_recovered_0 = l_raw_example_cache.evaluate(l_raw_example_0_path);

    fs::remove(l_raw_example_0_path);

    // The only way this should be able to recover the data is from cache,
    // since the file has been removed from disk.
    digital_ai::raw_example l_raw_example_0_recovered_1 = l_raw_example_cache.evaluate(l_raw_example_0_path);

    assert(l_raw_example_0 == l_raw_example_0_recovered_0 && l_raw_example_0 == l_raw_example_0_recovered_1);

    digital_ai::raw_example l_raw_example_1_recovered = l_raw_example_cache.evaluate(l_raw_example_1_path);

    assert(l_raw_example_1 == l_raw_example_1_recovered);

    // This will be the third call to evaluate the cache. The cache is size 2.
    // This means that the first raw example should no longer be in the cache.
    digital_ai::raw_example l_raw_example_2_recovered = l_raw_example_cache.evaluate(l_raw_example_2_path);

    assert(l_raw_example_2 == l_raw_example_2_recovered);

    try
    {
        digital_ai::raw_example l_raw_example_0_recovered_2 = l_raw_example_cache.evaluate(l_raw_example_0_path);
        
        // We should not make it to this point.
        throw std::runtime_error("Error: temporal cache unit test failed.");

    }
    catch(std::exception) {}

}

void unit_test_main(

)
{
    test_literal_product_equivalence();
    test_data_partitioning();
    test_get_covering_product();
    test_generalize();
    test_generalize_multi_output();
    test_temporal_cache();
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
        std::vector<digital_ai::raw_example> l_raw_examples;

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

        digital_ai::sum_of_products_string l_sops = digital_ai::generalize(l_raw_examples);
        
        auto l_stop =  std::chrono::high_resolution_clock::now();

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
