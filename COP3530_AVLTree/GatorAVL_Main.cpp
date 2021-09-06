#include <iostream>
#include <string>
#include <ctype.h>
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
		int FindBF();	// Return an invoking node's balance factor
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
	void Insert(string name, unsigned long gatorID);
	void Remove(unsigned long gatorID);
	void Search(unsigned long gatorID);		// Search for an ID
	void Search(string name);		// Search for a name
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

// GatorNode function definitions:
GatorAVL::GatorNode::GatorNode(string name, unsigned long gatorID, int height, GatorNode* left, GatorNode* right) {
	this->name = name;
	this->gatorID = gatorID;
	this->height = height;
	this->left = left;
	this->right = right;
}

int GatorAVL::GatorNode::FindHeight() {
	int heightLeft;
	int heightRight;
	if (left) {
		heightLeft = left->height;
	}
	else {
		heightLeft = 0;
	}
	if (right) {
		heightRight = right->height;
	}
	else {
		heightRight = 0;
	}
	return 1 + max(heightLeft, heightRight);
}

int GatorAVL::GatorNode::FindBF() {
	int heightLeft;
	int heightRight;
	if (!this->left) {
		heightLeft = 0;
	}
	else {
		heightLeft = this->left->height;
	}
	if (!this->right) {
		heightRight = 0;
	}
	else {
		heightRight = this->right->height;
	}
	return heightLeft - heightRight;
}


// GatorAVL private member function definitions:
GatorAVL::GatorNode* GatorAVL::RotateLeft(GatorNode* root) {	// Referenced code from Lecture 4a
	GatorNode* grandChild = root->right->left;
	GatorNode* newRoot = root->right;
	newRoot->left = root;
	root->right = grandChild;
	root->height = root->FindHeight();	// Recalculate the height of the original root node
	return newRoot;
}

GatorAVL::GatorNode* GatorAVL::RotateRight(GatorNode* root) {
	GatorNode* grandChild = root->left->right;
	GatorNode* newRoot = root->left;
	newRoot->right = root;
	root->left = grandChild;
	root->height = root->FindHeight();	// Recalculate the height of the original root node
	return newRoot;
}

GatorAVL::GatorNode* GatorAVL::RotateLeftRight(GatorNode* root) {
	root->left = RotateLeft(root->left);
	return RotateRight(root);
}

GatorAVL::GatorNode* GatorAVL::RotateRightLeft(GatorNode* root) {
	root->right = RotateRight(root->right);
	return RotateLeft(root);
}

GatorAVL::GatorNode* GatorAVL::BalanceNode(GatorNode* node) {
	int balanceFactor = node->FindBF();
	if (balanceFactor < -1) {
		if (node->right->FindBF() < 0) {	// There is a right-right imbalance at the given node
			return RotateLeft(node);
		}
		else {	// There is a right-left imbalance at the given node
			return RotateRightLeft(node);
		}
	}
	else if (balanceFactor > 1) {
		if (node->left->FindBF() > 0) {	// There is a left-left imbalance at the given node
			return RotateRight(node);
		}
		else {	// There is a left-right imbalance at the given node
			return RotateLeftRight(node);
		}
	}
	return node;
}

GatorAVL::GatorNode* GatorAVL::RecursiveInsert(GatorNode* root, string name, unsigned long gatorID) {	// Referenced pseudocode from Lecture 3a
	// Recursively insert the node at the correct location:
	if (!root) {
		return new GatorNode(name, gatorID);
	}
	else if (root->gatorID == gatorID) {	// Handling the case of duplicate IDs
		throw exception();
	}
	else if (gatorID < root->gatorID) {
		root->left = RecursiveInsert(root->left, name, gatorID);
	}
	else {
		root->right = RecursiveInsert(root->right, name, gatorID);
	}
	// Balance the tree along the search path:
	root = BalanceNode(root);
	root->height = root->FindHeight();

	return root;
}

GatorAVL::GatorNode* GatorAVL::RecursiveRemove(GatorNode* root, unsigned long gatorID) {	// Referenced pseudocode from Lecture 3b
	if (!root) {	// Handling the case of a non-existent search parameter
		throw exception();
	}
	if (gatorID < root->gatorID) {
		root->left = RecursiveRemove(root->left, gatorID);
	}
	else if (gatorID > root->gatorID) {
		root->right = RecursiveRemove(root->right, gatorID);
	}
	else {
		if (!root->left && !root->right) {	// The desired node has no children
			delete root;
			return nullptr;
		}
		else if (root->left && !root->right) {	// The desired node only has a left child
			GatorNode* temp = root->left;
			delete root;
			return temp;
		}
		else if (root->right && !root->left) {	// The desired node only has a right child
			GatorNode* temp = root->right;
			delete root;
			return temp;
		}
		else {	// The desired node has a right and left child -- find the inorder successor
			if (!root->right->left) {
				GatorNode* temp = root->right;
				temp->left = root->left;
				delete root;
				return temp;
			}
			else {
				GatorNode* currNode = root->right;	// Used to traverse to the parent of the inorder successor
				while (currNode->left->left) {
					currNode = currNode->left;
				}
				GatorNode* temp = currNode->left;
				currNode->left = temp->right;
				temp->left = root->left;
				temp->right = root->right;
				delete root;
				return temp;
			}
		}
	}
	// Balance the tree along the search path:
	root = BalanceNode(root);
	root->height = root->FindHeight();

	return root;
}

void GatorAVL::RecursiveRemoveInorder(GatorNode* root, int& count) {
	if (!root) {
		return;
	}
	else {
		RecursiveRemoveInorder(root->left, count);
		if (count != 0) {
			count--;
		}
		else {
			Remove(root->gatorID);
			throw exception();	// Break out of the recursion hierarchy after the desired node has been deleted -- prevents read-access violations
		}
		RecursiveRemoveInorder(root->right, count);
	}
}

void GatorAVL::RecursiveSearch(GatorNode* root, unsigned long gatorID) {	// Referenced pseudocode from Lecture 3a
	if (!root) {
		cout << "unsuccessful" << endl;
	}
	else if (root->gatorID == gatorID) {
		cout << root->name << endl;
	}
	else if (gatorID < root->gatorID) {
		RecursiveSearch(root->left, gatorID);
	}
	else {
		RecursiveSearch(root->right, gatorID);
	}
}

void GatorAVL::RecursiveSearch(GatorNode* root, string name, bool& found) {
	if (!root) {
		return;
	}
	else {
		if (root->name == name) {
			cout << root->gatorID << endl;
			found = true;
		}
		RecursiveSearch(root->left, name, found);
		RecursiveSearch(root->right, name, found);
	}
}

void GatorAVL::RecursiveInorder(GatorNode* root, vector<string>& result) {	// Referenced code from Lecture 3b
	if (!root) {
		return;
	}
	else {
		RecursiveInorder(root->left, result);
		result.push_back(root->name);
		RecursiveInorder(root->right, result);
	}
}

void GatorAVL::RecursivePreorder(GatorNode* root, vector<string>& result) {
	if (!root) {
		return;
	}
	else {
		result.push_back(root->name);
		RecursivePreorder(root->left, result);
		RecursivePreorder(root->right, result);
	}
}

void GatorAVL::RecursivePostorder(GatorNode* root, vector<string>& result) {
	if (!root) {
		return;
	}
	else {
		RecursivePostorder(root->left, result);
		RecursivePostorder(root->right, result);
		result.push_back(root->name);
	}
}

void GatorAVL::ClearTree(GatorNode* root) {
	if (!root) {
		return;
	}
	else {
		ClearTree(root->left);
		ClearTree(root->right);
		delete root;
	}
}


// GatorAVL public member function definitions:
GatorAVL::GatorAVL() {
	size = 0;
	root = nullptr;
}

GatorAVL::~GatorAVL() {
	ClearTree(root);
}

void GatorAVL::Insert(string name, unsigned long gatorID) {
	if (to_string(gatorID).length() != 8) {
		cout << "unsuccessful" << endl;
		return;
	}
	for (int i = 0; i < name.length(); i++) {
		if (!isalpha(name[i])) {
			cout << "unsuccessful" << endl;
			return;
		}
	}
	try {
		root = RecursiveInsert(root, name, gatorID);
	}
	catch (exception) {	// Will execute in the case of duplicate IDs
		cout << "unsuccessful" << endl;
		return;
	}
	size++;
	cout << "successful" << endl;
}

void GatorAVL::Remove(unsigned long gatorID) {
	if (to_string(gatorID).length() != 8) {
		cout << "unsuccessful" << endl;
		return;
	}
	try {
		root = RecursiveRemove(root, gatorID);
	}
	catch (exception) {		// Will execute in the case that the input gatorID is not found in the tree
		cout << "unsuccessful" << endl;
		return;
	}
	size--;
	cout << "successful" << endl;
}

void GatorAVL::Search(unsigned long gatorID) {
	if (to_string(gatorID).length() != 8) {
		cout << "unsuccessful";
		return;
	}
	RecursiveSearch(root, gatorID);
}

void GatorAVL::Search(string name) {
	for (int i = 0; i < name.length(); i++) {
		if (!isalpha(name[i])) {
			cout << "unsuccessful" << endl;
			return;
		}
	}
	bool found = false;		// Passed as a reference to keep track of matches across the recursive hierarchy
	RecursiveSearch(root, name, found);
	if (!found) {
		cout << "unsuccessful" << endl;
	}
}

void GatorAVL::Inorder() {
	vector<string> result;	// Vector to store the ordered names
	RecursiveInorder(root, result);
	for (int i = 0; i < result.size(); i++) {
		if (i == result.size() - 1) {
			cout << result[i] << endl;
		}
		else {
			cout << result[i] << ", ";
		}
	}
}

void GatorAVL::Preorder() {
	vector<string> result;	// Vector to store the ordered names
	RecursivePreorder(root, result);
	for (int i = 0; i < result.size(); i++) {
		if (i == result.size() - 1) {
			cout << result[i] << endl;
		}
		else {
			cout << result[i] << ", ";
		}
	}
}

void GatorAVL::Postorder() {
	vector<string> result;	// Vector to store the ordered names
	RecursivePostorder(root, result);
	for (int i = 0; i < result.size(); i++) {
		if (i == result.size() - 1) {
			cout << result[i] << endl;
		}
		else {
			cout << result[i] << ", ";
		}
	}
}

void GatorAVL::PrintLevelCount() {
	if (!root) {	// The tree is empty
		cout << "0" << endl;
	}
	else {
		cout << root->height << endl;
	}
}

void GatorAVL::RemoveInorder(int index) {
	if (index > size - 1 || index < 0) {	// Index is invalid
		cout << "unsuccessful" << endl;
		return;
	}
	try {
		RecursiveRemoveInorder(root, index);
	}
	catch (exception) {		// An exception is thrown if the value at index is removed successfully
		return;
	}
	cout << "unsuccessful" << endl;
}

GatorAVL::GatorNode* GatorAVL::GetRoot() {
	return root;
}

int GatorAVL::GetSize() {
	return size;
}

int GatorAVL::GetLevelCount() {
	if (!root) {
		return 0;
	}
	else {
		return root->height;
	}
}


//int main() {
//	GatorAVL avlTree;	// Initialize the class object
//
//	int numCommands;
//	cin >> numCommands;		// Read the number of commands to be parsed
//
//	string parseString;		// Used to load commands and parameters
//
//	for (int i = 0; i < numCommands; i++) {
//		cin >> parseString;
//		
//		if (parseString == "search") {
//			cin >> parseString;
//			if (isdigit(parseString[0])) {
//				avlTree.Search(stol(parseString));
//			}
//			else {
//				avlTree.Search(parseString.substr(1, parseString.length() - 2));	// Remove the surrounding quotation marks
//			}
//		}
//		else if (parseString == "insert") {
//			cin >> parseString;
//			string name = parseString.substr(1, parseString.length() - 2);
//			cin >> parseString;
//			unsigned long gatorID = stol(parseString);
//			avlTree.Insert(name, gatorID);
//		}
//		else if (parseString == "printLevelCount") {
//			avlTree.PrintLevelCount();
//		}
//		else if (parseString == "remove") {
//			cin >> parseString;
//			avlTree.Remove(stol(parseString));
//		}
//		else if (parseString == "removeInorder") {
//			cin >> parseString;
//			avlTree.RemoveInorder(stoi(parseString));
//		}
//		else if (parseString == "printInorder") {
//			avlTree.Inorder();
//		}
//		else if (parseString == "printPreorder") {
//			avlTree.Preorder();
//		}
//		else if (parseString == "printPostorder") {
//			avlTree.Postorder();
//		}
//		else {
//			cout << "unsuccessful" << endl;
//		}
//	}
//
//	return 0;
//}