// Note - the autograder expects this exact include for your catch code! If you use the manual catch compilation method, be sure you put this include back before submitting.
#include <catch2/catch_test_macros.hpp>

#include "GatorBST.h"

using namespace std;

// the syntax for defining a test is below. It is important for the name to be unique, but you can group multiple tests with [tags]. A test can have [multiple][tags] using that syntax.
TEST_CASE("Empty tree properties", "[constructor][height][search]") {
    GatorBST bst;

    REQUIRE(bst.Height() == 0);
    REQUIRE_FALSE(bst.SearchID(100).has_value());
    REQUIRE(bst.TraverseInorder().empty());
}

TEST_CASE("Insert and search basic functionality", "[insert][search][height]") {
    GatorBST bst;

    REQUIRE(bst.Insert(500, "Root"));
    REQUIRE(bst.Height() == 1);

    auto result = bst.SearchID(500);
    REQUIRE(result.has_value());
    REQUIRE(result.value() == "Root");
}

TEST_CASE("Duplicate UFID does not insert or overwrite", "[insert]") {
    GatorBST bst;

    REQUIRE(bst.Insert(100, "Alice"));
    REQUIRE_FALSE(bst.Insert(100, "Bob"));

    auto result = bst.SearchID(100);
    REQUIRE(result.has_value());
    REQUIRE(result.value() == "Alice");
}

TEST_CASE("SearchName returns sorted matching UFIDs", "[search]") {
    GatorBST bst;

    bst.Insert(30, "John");
    bst.Insert(10, "John");
    bst.Insert(20, "Alice");

    vector<int> ids = bst.SearchName("John");

    REQUIRE(ids.size() == 2);
    REQUIRE(ids == vector<int>{10, 30});
}

TEST_CASE("Height updates in skewed tree", "[height][insert]") {
    GatorBST bst;

    bst.Insert(10, "A");
    bst.Insert(20, "B");
    bst.Insert(30, "C");
    bst.Insert(40, "D");

    REQUIRE(bst.Height() == 4);
}

TEST_CASE("Remove leaf node", "[remove]") {
    GatorBST bst;

    bst.Insert(40, "A");
    bst.Insert(20, "B");

    REQUIRE(bst.Remove(20));
    REQUIRE_FALSE(bst.SearchID(20).has_value());
    REQUIRE(bst.Height() == 1);
}

TEST_CASE("Remove node with one child", "[remove]") {
    GatorBST bst;

    bst.Insert(50, "A");
    bst.Insert(25, "B");
    bst.Insert(10, "C");

    REQUIRE(bst.Remove(25));
    REQUIRE_FALSE(bst.SearchID(25).has_value());
    REQUIRE(bst.SearchID(10).has_value());
}

TEST_CASE("Remove node with two children uses inorder successor", "[remove][structure]") {
    GatorBST bst;

    bst.Insert(50, "A");
    bst.Insert(30, "B");
    bst.Insert(70, "C");
    bst.Insert(60, "D");
    bst.Insert(80, "E");

    REQUIRE(bst.Remove(70));
    REQUIRE_FALSE(bst.SearchID(70).has_value());

    // Tree must still contain these
    REQUIRE(bst.SearchID(60).has_value());
    REQUIRE(bst.SearchID(80).has_value());

    // Inorder must remain sorted
    auto inorder = bst.TraverseInorder();
    for (size_t i = 1; i < inorder.size(); i++) {
        REQUIRE(inorder[i-1]->ufid < inorder[i]->ufid);
    }
}

TEST_CASE("Inorder traversal remains sorted after mixed operations", "[traversal][remove][insert]") {
    GatorBST bst;

    bst.Insert(40, "A");
    bst.Insert(20, "B");
    bst.Insert(60, "C");
    bst.Insert(10, "D");
    bst.Insert(30, "E");

    bst.Remove(20);

    auto inorder = bst.TraverseInorder();

    for (size_t i = 1; i < inorder.size(); i++) {
        REQUIRE(inorder[i-1]->ufid < inorder[i]->ufid);
    }
}


// You are free to write as many tests as you want. Your credit for this
// assignment will be based on the proportion of passing tests as well as the
// number of mutants caught, so it might be a good idea to write many small,
// focused tests over a few large, potentially buggy tests...
