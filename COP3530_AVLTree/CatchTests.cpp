#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "GatorAVL.h"

TEST_CASE("Big Tree") {
	GatorAVL avlTree;

	for (int i = 0; i < 500; i++) {
		avlTree.Insert("testname", to_string((99999999 - i)));
	}
	REQUIRE(avlTree.GetSize() == 500);
	REQUIRE(avlTree.GetRoot()->gatorID == 99999744);	// A median element should be at the root of the tree

	avlTree.Remove("99999744");	// Remove the root and check that the tree changed properly
	REQUIRE(avlTree.GetSize() == 499);
	REQUIRE(avlTree.GetRoot()->gatorID == 99999745);

	for (int i = 0; i < 499; i++) {		// Remove every value except the lowest
		avlTree.Remove(to_string(99999999 - i));
	}
	REQUIRE(avlTree.GetRoot()->gatorID == 99999500);	// Check to see the lowest value is now the root of the tree
	avlTree.Remove("99999500");
	REQUIRE(avlTree.GetSize() == 0);	// Check to see the tree is now empty
}

TEST_CASE("Inorder Removals") {
	GatorAVL avlTree;

	avlTree.RemoveInorder(0);	// Nothing should happen -- there is nothing in the tree
	REQUIRE(avlTree.GetRoot() == nullptr);

	avlTree.Insert("Jacob", "99999999");
	avlTree.Insert("Jack", "99999998");
	avlTree.Insert("Jake", "99999997");
	avlTree.Insert("Lauren", "99999996");
	avlTree.Insert("Dustin", "99999995");
	avlTree.Insert("Gabriel", "99999994");
	avlTree.Insert("Jerold", "99999993");
	REQUIRE(avlTree.GetRoot()->name == "Lauren");

	avlTree.RemoveInorder(3);	// Removing the median index removes the root node of the tree
	REQUIRE(avlTree.GetRoot()->name != "Lauren");

	avlTree.RemoveInorder(0);
	avlTree.RemoveInorder(0);
	avlTree.RemoveInorder(0);
	REQUIRE(avlTree.GetRoot()->name == "Jack");	// The new root should be the median of the remaining three elements

	avlTree.RemoveInorder(9);	// Trying to remove from an out-of-bounds index
	REQUIRE(avlTree.GetSize() == 3);	// Check that the size has not changed
}

TEST_CASE("Duplicate GatorIDs") {
	GatorAVL avlTree;

	avlTree.Insert("John", "99999999");
	avlTree.Insert("Joe", "99999998");
	avlTree.Insert("Makayla", "99999997");
	avlTree.Insert("Mick", "99999996");
	avlTree.Insert("Carlos", "99999995");
	avlTree.Insert("Jane", "99999994");
	avlTree.Insert("Cassidy", "99999993");

	avlTree.Insert("Zoey", "99999993");	// Attempt to insert duplicate gatorIDs
	avlTree.Insert("Lucas", "99999994");
	avlTree.Insert("Chloe", "99999995");
	REQUIRE(avlTree.GetSize() == 7);	// Check that the size has not changed
	
	avlTree.Remove("99999993");
	avlTree.Remove("99999993");	// Attempt to remove an already-removed value
	REQUIRE(avlTree.GetSize() == 6);	// Check that the size has only decreased by 1
}

TEST_CASE("Level Correctness") {
	GatorAVL avlTree;

	avlTree.Insert("Jacob", "99999999");
	avlTree.Insert("Jack", "99999998");
	avlTree.Insert("Jake", "99999997");
	avlTree.Insert("Lauren", "99999996");
	avlTree.Insert("Dustin", "99999995");
	avlTree.Insert("Gabriel", "99999994");
	avlTree.Insert("Jerold", "99999993");
	avlTree.Insert("John", "99999992");
	avlTree.Insert("Joe", "99999991");
	avlTree.Insert("Makayla", "99999990");
	avlTree.Insert("Mick", "99999989");
	avlTree.Insert("Carlos", "99999988");
	avlTree.Insert("Jane", "99999987");
	avlTree.Insert("Cassidy", "99999986");

	REQUIRE(avlTree.GetLevelCount() == 4);

	for (int i = 0; i < 7; i++) {	// Remove 7 nodes from the tree (the number in the last level)
		avlTree.RemoveInorder(0);
	}
	REQUIRE(avlTree.GetLevelCount() == 3);	// Check that there is one less level in the tree
}

TEST_CASE("Height Correctness") {
	GatorAVL avlTree;

	avlTree.Insert("Jacob", "99999999");
	avlTree.Insert("Jack", "99999998");
	avlTree.Insert("Jake", "99999997");
	avlTree.Insert("Lauren", "99999996");
	avlTree.Insert("Dustin", "99999995");
	avlTree.Insert("Gabriel", "99999994");
	avlTree.Insert("Jerold", "99999993");
	avlTree.Insert("John", "99999992");
	avlTree.Insert("Joe", "99999991");
	avlTree.Insert("Makayla", "99999990");
	avlTree.Insert("Mick", "99999989");
	avlTree.Insert("Carlos", "99999988");
	avlTree.Insert("Jane", "99999987");
	avlTree.Insert("Cassidy", "99999986");

	REQUIRE(avlTree.GetRoot()->height == 4);

	for (int i = 0; i < 7; i++) {	// Remove 7 nodes from the tree (the number in the last level)
		avlTree.RemoveInorder(0);
	}
	REQUIRE(avlTree.GetRoot()->height == 3); // Check that the root node's height has decreased by one

	REQUIRE(avlTree.GetRoot()->left->height == 2);	// Check that the heights of both children nodes of the root are one less than the root's height
	REQUIRE(avlTree.GetRoot()->right->height == 2);
	
	for (int i = 0; i < 4; i++) {	// Strip the tree to only a root and two leaf nodes
		avlTree.RemoveInorder(0);
	}
	REQUIRE(avlTree.GetRoot()->left->height == 1);	// Check that both the leaf nodes have a height of 1
	REQUIRE(avlTree.GetRoot()->right->height == 1);
}


