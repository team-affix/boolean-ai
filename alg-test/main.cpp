#include "digital-ai/generalize.h"
#include <iostream>
#include <assert.h>

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

void test_literal_difference(

)
{
    {
        digital_ai::satisfying_input l_satisfying_input =     { 0, 0, 1, 0, 0 };
        digital_ai::unsatisfying_input l_unsatisfying_input = { 0, 0, 0, 0, 0 };
        std::vector<digital_ai::literal> l_literal_difference = digital_ai::literal_difference(l_satisfying_input, l_unsatisfying_input);
        assert(l_literal_difference.size() == 1);
        assert(l_literal_difference[0] == digital_ai::literal(2, false));
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

void test_difference_product(

)
{
    {
        digital_ai::satisfying_input   l_satisfying_input({0, 1, 1, 0});
        digital_ai::unsatisfying_input l_unsatisfying_input({0, 1, 1, 1});

        digital_ai::literal_product l_difference_product =
            digital_ai::literal_difference(l_satisfying_input, l_unsatisfying_input);

        assert(l_difference_product.literals().size() == 1);
        assert(l_difference_product.literals()[0] == digital_ai::literal(3, true));
    }

    {
        digital_ai::satisfying_input   l_satisfying_input({0, 1, 1, 0});
        digital_ai::unsatisfying_input l_unsatisfying_input({0, 0, 1, 1});

        digital_ai::literal_product l_literal_difference =
            digital_ai::literal_difference(l_satisfying_input, l_unsatisfying_input);

        assert(l_literal_difference.literals().size() == 2);
        assert(l_literal_difference.literals()[0] == digital_ai::literal(1, false));
        assert(l_literal_difference.literals()[1] == digital_ai::literal(3, true));
    }

}

void test_try_get_maximally_covering_literal(

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

        digital_ai::literal l_maximally_covering_literal;

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        bool l_success = digital_ai::try_get_maximally_covering_literal({},
            l_partitioned_example_set.m_unsatisfying_inputs,
            l_partitioned_example_set.m_satisfying_inputs[0], l_maximally_covering_literal);
    
        assert(l_success);
        assert(l_maximally_covering_literal == digital_ai::literal(0, false));

    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{0, 0, 0, 1}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{0, 0, 1, 0}, {1, 1, 0}},
            {{0, 0, 1, 1}, {0, 1, 0}},
            {{0, 1, 1, 0}, {1, 1, 0}},
            {{0, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        digital_ai::literal l_maximally_covering_literal;

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        bool l_success = digital_ai::try_get_maximally_covering_literal({},
            l_partitioned_example_set.m_unsatisfying_inputs,
            l_partitioned_example_set.m_satisfying_inputs[0], l_maximally_covering_literal);
    
        assert(l_success);
        assert(l_maximally_covering_literal == digital_ai::literal(2, true));

    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{0, 0, 0, 1}, {1, 1, 0}},
            {{0, 0, 1, 0}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{0, 0, 1, 1}, {0, 1, 0}},
            {{0, 1, 1, 0}, {1, 1, 0}},
            {{0, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        digital_ai::literal l_maximally_covering_literal;

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        bool l_success = digital_ai::try_get_maximally_covering_literal({},
            l_partitioned_example_set.m_unsatisfying_inputs,
            l_partitioned_example_set.m_satisfying_inputs[1], l_maximally_covering_literal);
    
        assert(l_success);
        assert(l_maximally_covering_literal == digital_ai::literal(3, true));

    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 1, 0, 0}, {0, 0, 0}},
            {{0, 1, 0, 1}, {0, 0, 1}},
            {{0, 1, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{0, 1, 0, 1}, {1, 1, 0}},
            {{0, 0, 1, 0}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{0, 1, 1, 0}, {1, 1, 0}},
            {{0, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        digital_ai::literal l_maximally_covering_literal;

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        bool l_success = digital_ai::try_get_maximally_covering_literal({},
            l_partitioned_example_set.m_unsatisfying_inputs,
            l_partitioned_example_set.m_satisfying_inputs[1], l_maximally_covering_literal);
    
        assert(l_success);
        assert(l_maximally_covering_literal == digital_ai::literal(1, true));

    }

}

void test_cover(

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

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = digital_ai::cover(
            l_partitioned_example_set.m_unsatisfying_inputs,l_partitioned_example_set.m_satisfying_inputs[0]);
    
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

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = digital_ai::cover(
            l_partitioned_example_set.m_unsatisfying_inputs,l_partitioned_example_set.m_satisfying_inputs[1]);
    
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

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = digital_ai::cover(
            l_partitioned_example_set.m_unsatisfying_inputs,l_partitioned_example_set.m_satisfying_inputs[2]);
    
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

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = digital_ai::cover(
            l_partitioned_example_set.m_unsatisfying_inputs,l_partitioned_example_set.m_satisfying_inputs[0]);
    
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

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = digital_ai::cover(
            l_partitioned_example_set.m_unsatisfying_inputs,l_partitioned_example_set.m_satisfying_inputs[0]);
    
        assert(l_covering_product.literals() == 
            std::vector({ 
                digital_ai::literal(1, false),
                digital_ai::literal(2, false),
                digital_ai::literal(3, true),
                digital_ai::literal(0, true) }));
    
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

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = digital_ai::cover(
            l_partitioned_example_set.m_unsatisfying_inputs,l_partitioned_example_set.m_satisfying_inputs[0]);
    
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
        digital_ai::sum_of_products l_generalized = digital_ai::generalize(l_partitioned_example_set);
    
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
            digital_ai::literal(2, true), digital_ai::literal(3, false), digital_ai::literal(0, true)
        }));
        
        assert(l_generalized.literal_products()[1].literals() == std::vector({
            digital_ai::literal(0, true), digital_ai::literal(2, false), digital_ai::literal(1, true)
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
        digital_ai::literal(2, true), digital_ai::literal(3, false), digital_ai::literal(0, true)
    }));
    
    assert(l_generalized.sums_of_products()[2].literal_products()[1].literals() == std::vector({
        digital_ai::literal(0, true), digital_ai::literal(2, false), digital_ai::literal(1, true)
    }));


    l_generalized.simplify();

    assert(l_generalized.sums_of_products()[0].literal_products().size() == 2);
    assert(l_generalized.sums_of_products()[1].literal_products().size() == 3);
    assert(l_generalized.sums_of_products()[2].literal_products().size() == 2);

}

void unit_test_main(

)
{
    test_literal_product_equivalence();
    test_literal_difference();
    test_data_partitioning();
    test_difference_product();
    test_try_get_maximally_covering_literal();
    test_cover();
    test_generalize();
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
    std::vector<digital_ai::raw_example> l_raw_examples;

    size_t l_training_sets_count = 500;

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

    digital_ai::sum_of_products_string l_sops = digital_ai::generalize(l_raw_examples);
    l_sops.simplify();

    std::vector<bool> l_test_byte_0 = l_random_byte();
    std::vector<bool> l_test_byte_1 = l_random_byte();

    std::vector<bool> l_concatenated;
    l_concatenated.insert(l_concatenated.end(), l_test_byte_0.begin(), l_test_byte_0.end());
    l_concatenated.insert(l_concatenated.end(), l_test_byte_1.begin(), l_test_byte_1.end());

    std::vector<bool> l_evaluated = l_sops.evaluate(l_concatenated);

}

int main(

)
{
    add_8_bit_numbers_test();
    //unit_test_main();

    return 0;
}
