/*
    This file is distributed under MIT License.

    Copyright (c) 2020 draghan

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <nanobench.h>
#include <iostream>
#include "../MetadataTree.hpp"

int main()
{
    constexpr int elements = 100000;
    constexpr int middle_element = elements / 2;
    auto tree = MetadataTree{NodeType::sequence};
    for (int i = 0; i < elements; ++i)
    {
        tree.add_node(NodeType::action, MetadataTree::root_id);
    }

    std::cerr << "init done\n";

    constexpr int min_epoch_iterations = 300;
    ankerl::nanobench::Bench().minEpochIterations(min_epoch_iterations).run("get all children of root", [&]
    {
        auto children = tree.get_children_of(MetadataTree::root_id);
    });

    ankerl::nanobench::Bench().minEpochIterations(min_epoch_iterations).run("get all children of middle node", [&]
    {
        auto children = tree.get_children_of(middle_element);
    });

    ankerl::nanobench::Bench().minEpochIterations(min_epoch_iterations).run(("get all children of non existing node"), [&]
    {
        try
        {
            auto children = tree.get_children_of(elements + 10);
        }
        catch (...)
        {
        }
    });

    ankerl::nanobench::Bench().minEpochIterations(min_epoch_iterations).run("get left sibling of middle node", [&]
    {
        auto children = tree.get_left_sibling_of(middle_element);
    });

    ankerl::nanobench::Bench().minEpochIterations(min_epoch_iterations).run("get right sibling of middle node", [&]
    {
        auto children = tree.get_right_sibling_of(middle_element);
    });

    ankerl::nanobench::Bench().minEpochIterations(min_epoch_iterations).run("get parent of middle node", [&]
    {
        auto children = tree.get_parent_of(middle_element);
    });

    ankerl::nanobench::Bench().minEpochIterations(min_epoch_iterations).run("get parent of first node", [&]
    {
        auto children = tree.get_parent_of(MetadataTree::root_id + 1);
    });

    ankerl::nanobench::Bench().minEpochIterations(min_epoch_iterations).run("get parent of last node", [&]
    {
        auto children = tree.get_parent_of(elements);
    });

    ankerl::nanobench::Bench().minEpochIterations(min_epoch_iterations).run("get number of children - root", [&]
    {
        auto children = tree.get_number_of_children(MetadataTree::root_id);
    });

    ankerl::nanobench::Bench().minEpochIterations(min_epoch_iterations).run("get number of children - middle", [&]
    {
        auto children = tree.get_number_of_children(middle_element);
    });

    ankerl::nanobench::Bench().minEpochIterations(min_epoch_iterations).run("get number of children - last", [&]
    {
        auto children = tree.get_number_of_children(elements);
    });
}

