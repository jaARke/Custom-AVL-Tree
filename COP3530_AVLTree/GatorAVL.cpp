#include "GatorAVL.h"

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
			// Convert the root gatorID to a string padded with 0s
			stringstream idString;
			idString << setfill('0') << setw(8) << root->gatorID;
			Remove(idString.str());
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
			cout << setfill('0') << setw(8) << root->gatorID << endl;
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

void GatorAVL::Insert(string name, string gatorID) {
	// The following validation of the gatorID string was made with help from https://stackoverflow.com/questions/49635616/stdstoi-string-with-non-numeric-characters-getting-parsed-as-an-integer-with
	size_t sizeOfNum = 0;	// Used to compare the parsed number's length to the length of the string (to make sure all characters were able to be parsed -- i.e. they were all digits)
	long gatorIDNum = 0;
	try {
		gatorIDNum = stol(gatorID, &sizeOfNum);
	}
	catch (invalid_argument) {	// The above try block throws an error if the first character of the gatorID string is not a digit
		gatorIDNum = -1;	// This will cause the operation to be unsuccessful in the next comparison
	}
	if (gatorID.length() != 8 || sizeOfNum != gatorID.length() || gatorIDNum < 0) {
		cout << "unsuccessful" << endl;
		return;
	}
	for (int i = 0; i < name.length(); i++) {
		if (!isalpha(name[i]) && name[i] != ' ') {
			cout << "unsuccessful" << endl;
			return;
		}
	}
	try {
		root = RecursiveInsert(root, name, gatorIDNum);
	}
	catch (exception) {	// Will execute in the case of duplicate IDs
		cout << "unsuccessful" << endl;
		return;
	}
	size++;
	cout << "successful" << endl;
}

void GatorAVL::Remove(string gatorID) {
	size_t sizeOfNum = 0;
	long gatorIDNum = 0;
	try {
		gatorIDNum = stol(gatorID, &sizeOfNum);
	}
	catch (invalid_argument) {
		gatorIDNum = -1;
	}
	if (gatorID.length() != 8 || sizeOfNum != gatorID.length() || gatorIDNum < 0) {
		cout << "unsuccessful" << endl;
		return;
	}
	try {
		root = RecursiveRemove(root, gatorIDNum);
	}
	catch (exception) {		// Will execute in the case that the input gatorID is not found in the tree
		cout << "unsuccessful" << endl;
		return;
	}
	size--;
	cout << "successful" << endl;
}

void GatorAVL::Search(string term) {
	if (!isdigit(term[0])) {	// Search for a name
		for (int i = 0; i < term.length(); i++) {
			if (!isalpha(term[i]) && term[i] != ' ') {
				cout << "unsuccessful" << endl;
				return;
			}
		}
		bool found = false;		// Passed as a reference to keep track of matches across the recursive hierarchy
		RecursiveSearch(root, term, found);
		if (!found) {
			cout << "unsuccessful" << endl;
		}
	}
	else {	// Search for a gatorID
		size_t sizeOfNum = 0;
		long gatorIDNum = 0;
		try {
			gatorIDNum = stol(term, &sizeOfNum);
		}
		catch (invalid_argument) {
			gatorIDNum = -1;
		}
		if (term.length() != 8 || sizeOfNum != term.length() || gatorIDNum < 0) {
			cout << "unsuccessful" << endl;
			return;
		}
		RecursiveSearch(root, gatorIDNum);
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