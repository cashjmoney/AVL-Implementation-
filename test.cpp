/*#include <catch2/catch_test_macros.hpp>
#include "AVL.h"
#include <sstream>
#include <vector>
#include <algorithm>
#define CATCH_CONFIG_MAIN
using namespace std;
TEST_CASE("AVL Tree Incorrect Commands", "[error]") {
    AVLTree tree;

    SECTION("Incorrect format for insert command") {
        istringstream input("insert \"A11y\" 45679999");
        tree.parse(input);
        REQUIRE(tree.target == "unsuccessful");
    }

    SECTION("Invalid characters in name during insert") {
        istringstream input("insert \"Alice123\" 456");
        tree.parse(input);
        REQUIRE(tree.target == "unsuccessful");
    }

    SECTION("Remove command with invalid UFID") {
        istringstream input("remove 999");
        tree.parse(input);
        REQUIRE(tree.target == "unsuccessful");
    }

    SECTION("Search command with invalid UFID format") {
        istringstream input("search @Alice");
        tree.parse(input);
        REQUIRE(tree.target == "unsuccessful");
    }

    SECTION("Invalid command that is not recognized") {
        istringstream input("unknown_command");
        tree.parse(input);
        REQUIRE(tree.target == "unsuccessful");
    }
}

TEST_CASE("AVL Tree Edge Cases", "[edge]") {
    AVLTree tree;

    SECTION("Remove a node that doesn't exist") {
        tree.Insert("Alice", 123);
        tree.Remove(999);
        REQUIRE(tree.target == "unsuccessful");
    }

    SECTION("Search for a UFID that doesn't exist") {
        tree.Insert("Alice", 123);
        tree.search(999);
        REQUIRE(tree.target == "unsuccessful");
    }

    SECTION("Insert and remove the only node in the tree") {
        tree.Insert("Alice", 123);
        tree.Remove(123);
        vector<string> expectedInorder = {};
        vector<string> actualInorder = tree.printinorder(tree.root);
        REQUIRE(expectedInorder == actualInorder);
    }
}

TEST_CASE("AVL Tree Rotation Cases", "[rotation]") {
    AVLTree tree;

    SECTION("Right Rotation (left-left imbalance)") {
        // Construct an unbalanced tree to test right rotation
        tree.Insert("Alice", 123);
        tree.Insert("Bob", 456);
        tree.Insert("Charlie", 789);
        // Perform an operation that causes left-left imbalance
        tree.Insert("David", 101);

        // Verify the tree is balanced after rotation
        vector<string> expectedInorder = { "David", "Alice", "Bob", "Charlie" };
        vector<string> actualInorder = tree.printinorder(tree.root);
        REQUIRE(expectedInorder == actualInorder);
    }

    SECTION("Left Rotation (right-right imbalance)") {
        // Construct an unbalanced tree to test left rotation
        tree.Insert("Alice", 123);
        tree.Insert("Charlie", 789);
        tree.Insert("Bob", 456);
        // Perform an operation that causes right-right imbalance
        tree.Insert("David", 101);

        // Verify the tree is balanced after rotation
        vector<string> expectedInorder = { "David", "Alice", "Bob", "Charlie" };
        vector<string> actualInorder = tree.printinorder(tree.root);
        REQUIRE(expectedInorder == actualInorder);
    }

    SECTION("Left-Right Rotation (left-right imbalance)") {
        // Construct an unbalanced tree to test left-right rotation
        tree.Insert("Alice", 123);
        tree.Insert("Charlie", 789);
        tree.Insert("Bob", 456);
        // Perform an operation that causes left-right imbalance
        tree.Insert("David", 501);

        // Verify the tree is balanced after rotation
        vector<string> expectedInorder = { "Alice", "Bob", "David", "Charlie" };
        vector<string> actualInorder = tree.printinorder(tree.root);
        REQUIRE(expectedInorder == actualInorder);
    }

    SECTION("Right-Left Rotation (right-left imbalance)") {
        // Construct an unbalanced tree to test right-left rotation
        tree.Insert("Alice", 123);
        tree.Insert("Bob", 456);
        tree.Insert("Charlie", 789);
        // Perform an operation that causes right-left imbalance
        tree.Insert("David", 101);

        // Verify the tree is balanced after rotation
        vector<string> expectedInorder =  { "David", "Alice", "Bob", "Charlie" };
        vector<string> actualInorder = tree.printinorder(tree.root);
        REQUIRE(expectedInorder == actualInorder);
    }
}

TEST_CASE("AVL Tree Deletion Cases", "[delete]") {
    AVLTree tree;
    
    // Insert nodes into the AVL tree
    tree.Insert("Alice", 123);
    tree.Insert("Bob", 456);
    tree.Insert("Charlie", 789);
    tree.Insert("David", 101);
    tree.Insert("Emma", 202);
    tree.Insert("Frank", 303);
    tree.Insert("Grace", 404);
    tree.Insert("Hannah", 505);

    SECTION("Delete node with no children") {
        tree.Remove(505); // Remove "Hannah"
        REQUIRE(tree.target == "successful");
        std::vector<std::string> expectedInorder = { "David", "Alice", "Emma", "Frank", "Grace", "Bob", "Charlie" };
        std::vector<std::string> actualInorder = tree.printinorder(tree.root);
        REQUIRE(expectedInorder == actualInorder);
    }

    SECTION("Delete node with one child") {
        tree.Remove(404); // Remove "Grace" which has one child "Hannah"
        REQUIRE(tree.target == "successful");
        std::vector<std::string> expectedInorder = { "David", "Alice", "Emma", "Frank", "Bob", "Hannah", "Charlie" };
        std::vector<std::string> actualInorder = tree.printinorder(tree.root);
        REQUIRE(expectedInorder == actualInorder);
    }

    SECTION("Delete node with two children") {
        tree.Remove(456); // Remove "Bob" which has two children
        REQUIRE(tree.target == "successful");
        std::vector<std::string> expectedInorder = { "David", "Alice", "Emma", "Frank", "Grace", "Hannah", "Charlie" };
        std::vector<std::string> actualInorder = tree.printinorder(tree.root);
        REQUIRE(expectedInorder == actualInorder);
    }
}

TEST_CASE("AVL Tree Insert and Remove 100 Nodes", "[insert-remove]") {
    AVLTree tree;

    // Insert 100 nodes
    for (int i = 1; i <= 100; ++i) {
        string name = "Student" + to_string(i);
        tree.Insert(name, i);
    }

    // Verify inorder traversal after 100 insertions
    vector<string> expectedInorderAfterInsert;
    for (int i = 1; i <= 100; ++i) {
        expectedInorderAfterInsert.push_back("Student" + to_string(i));
    }
    vector<string> actualInorderAfterInsert = tree.printinorder(tree.root);
    REQUIRE(expectedInorderAfterInsert == actualInorderAfterInsert);

    // Remove 10 random nodes
    srand(time(NULL)); // Seed for random number generation
    vector<int> removedUFIDs;
    for (int i = 0; i < 10; ++i) {
        int randomUFID = rand() % 100 + 1; // Generate random UFID between 1 and 100
        tree.Remove(randomUFID);
        removedUFIDs.push_back(randomUFID);
    }

    // Verify inorder traversal after removals
    for (int i = 0; i < 10; ++i) {
        auto it = find(expectedInorderAfterInsert.begin(), expectedInorderAfterInsert.end(), "Student" + to_string(removedUFIDs[i]));
        if (it != expectedInorderAfterInsert.end()) {
            expectedInorderAfterInsert.erase(it); // Remove the expected node from vector
        }
    }
    vector<string> actualInorderAfterRemoval = tree.printinorder(tree.root);
    REQUIRE(expectedInorderAfterInsert == actualInorderAfterRemoval);
}
*/