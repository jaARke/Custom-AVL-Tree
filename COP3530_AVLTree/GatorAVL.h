#include <string>
#include <sstream>
#include <iostream>
#include <ctype.h>
#include <iomanip>
#include <vector>

using namespace std;

class GatorAVL {
	struct GatorNode {
		string name;
		unsigned long gatorID;
		int height;
		GatorNode* left;
		GatorNode* right;

		GatorNode(string name = "default_name", unsigned long gatorID = 0, int height = 1, GatorNode* left = nullptr, GatorNode* right = nullptr);
		int FindHeight();	// Return an invoking node's height value (used to update the height member variable)
		int FindBF(); // Return an invoking node's balance factor
	};

private:
	// Private member variables:
	unsigned int size;
	GatorNode* root;

	GatorNode* RotateLeft(GatorNode* root);
	GatorNode* RotateRight(GatorNode* root);
	GatorNode* RotateLeftRight(GatorNode* root);
	GatorNode* RotateRightLeft(GatorNode* root);

	GatorNode* BalanceNode(GatorNode* node);	// Check if the invoking node is balanced -- if not, make necessary rotations and return the node it was replaced by
	GatorNode* RecursiveInsert(GatorNode* root, string name, unsigned long gatorID);	// Helper function for Insert()
	GatorNode* RecursiveRemove(GatorNode* root, unsigned long gatorID);		// Helper function for Remove()
	void RecursiveRemoveInorder(GatorNode* root, int& count);
	// Helper functions for Search():
	void RecursiveSearch(GatorNode* root, unsigned long gatorID);
	void RecursiveSearch(GatorNode* root, string name, bool& found);
	// Helper functions for traversals:
	void RecursiveInorder(GatorNode* root, vector<string>& result);
	void RecursivePreorder(GatorNode* root, vector<string>& result);
	void RecursivePostorder(GatorNode* root, vector<string>& result);

	void ClearTree(GatorNode* root);	// Delete each node in the tree

public:
	GatorAVL();
	~GatorAVL();
	void Insert(string name, string gatorID);
	void Remove(string gatorID);
	void Search(string term);		// Search for a name or gatorID
	void Inorder();
	void Preorder();
	void Postorder();
	void PrintLevelCount();
	void RemoveInorder(int index);
	// Accessor functions to aid with testing:
	GatorNode* GetRoot();
	int GetSize();
	int GetLevelCount();
};