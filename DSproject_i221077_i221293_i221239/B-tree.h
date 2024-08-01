#pragma once
#include <iostream>
#include <cmath>
using namespace std;


class FileNode {
public:
	string Hkey;
	string value;

	FileNode(string h = "", string val = "") {
		Hkey = h;
		value = val;
	}
};

class BNode {
public:
	bool isLeaf;
	FileNode** keys;
	BNode** children;
	int keycount;
	int childcount;

	BNode(int order = 5) {
		isLeaf = 0;
		keycount = 0;
		childcount = 0;
		keys = new FileNode * [order - 1];
		for (int i = 0; i < order - 1; i++) {
			keys[i] = new FileNode();
			keys[i]->Hkey = "";
		}
		children = new BNode * [order];
		for (int i = 0; i < order; i++) {
			children[i] = NULL;
		}
	}
	int insertInNode(string val, string rv) {
		int insertloc = keycount;
		for (int i = 0; i < keycount; i++) {
			if ((keys[i]->Hkey > val && (keys[i]->Hkey.length() == val.length())) || (keys[i]->Hkey.length() > val.length())) {
				insertloc = i;
				break;
			}
		}
		for (int i = keycount; i > insertloc; i--) {
			*keys[i] = *keys[i - 1];
		}
		keys[insertloc]->Hkey = val;
		keys[insertloc]->value = rv;
		keycount++;
		for (int i = keycount + 1; i > insertloc; i--) {
			children[i] = children[i - 1];
		}
		return insertloc;
	}
};


class BTree {
public:
	BNode* root;
	int maxkey;
	int minkey;
	int maxchild;
	int minchild;

	BTree(int order1 = 5) { //default parameter of order 5
		root = new BNode(order1);
		root->isLeaf = 1;
		maxkey = order1 - 1;
		minkey = ceil(((order1 * 1.0) / 2)) - 1;
		maxchild = order1;
		minchild = ceil(((order1 * 1.0) / 2));
	}

	void print(BNode* n) {
		if (n != NULL) {
			cout << "Keys: [ ";
			for (int i = 0; i < n->keycount; i++) {
				cout << n->keys[i]->Hkey << "(" << n->keys[i]->value << ")" << " ";
			}
			cout << "]" << endl;
			for (int i = 0; i < n->keycount + 1; i++) {
				print(n->children[i]);
			}
		}
	}

	void searchrecurs(BNode* n, FileNode**& f, int& s, string val) {
		if (n != NULL) {
			for (int i = 0; i < n->keycount; i++) {
				if (n->keys[i]->Hkey == val) {
					if (f == NULL) {
						f = new FileNode * [1];
						s = 1;
						f[0] = n->keys[i];
					}
					else {
						FileNode** temp = new FileNode * [s + 1];
						for (int j = 0; j < s; j++) {
							temp[j] = f[j];
						}
						temp[s] = n->keys[i];
						s++;
						f = temp;
					}
				}
			}

			for (int i = 0; i < n->keycount + 1; i++) {
				searchrecurs(n->children[i], f, s, val);
			}
		}
	}

	void searchrecursless(BNode* n, FileNode**& f, int& s, string val) {
		if (n != NULL) {
			for (int i = 0; i < n->keycount; i++) {
				if ((n->keys[i]->Hkey == val) || (n->keys[i]->Hkey < val && (n->keys[i]->Hkey.length() == val.length())) || (n->keys[i]->Hkey.length() < val.length())) {
					if (f == NULL) {
						f = new FileNode * [1];
						s = 1;
						f[0] = new FileNode(n->keys[i]->Hkey, n->keys[i]->value);
					}
					else {
						FileNode** temp = new FileNode * [s + 1];
						for (int j = 0; j < s; j++) {
							temp[j] = new FileNode(f[j]->Hkey, f[j]->value);
						}
						temp[s] = new FileNode(n->keys[i]->Hkey, n->keys[i]->value);
						s++;
						f = temp;
					}
				}
			}

			for (int i = 0; i < n->keycount + 1; i++) {
				searchrecursless(n->children[i], f, s, val);
			}
		}
	}

//..................................................................................

	void additionmachineB(BTree* prevB, string comparitor) {
		FileNode** f = NULL;
		int sizef = 0;

		searchrecursless(root, f, sizef, comparitor);

		for (int i = 0; i < sizef; i++) {
			cout << f[i]->Hkey << " " << f[i]->value << endl;
		}

		for (int i = 0; i < sizef; i++) {
			cout << "TRANSFER: " << f[i]->Hkey << endl;
			prevB->insert(f[i]->Hkey, f[i]->value);
			this->deleteNode(f[i]->Hkey);

			cout << "new node: " << endl;
			print(prevB->root);
			cout << endl;
			cout << "next node: ";
			print(root);
		}


	}
//..................................................................................

	FileNode** search(string val, int& sizereturn) {
		FileNode** f = NULL;
		int sizef = 0;

		searchrecurs(root, f, sizef, val);

		sizereturn = sizef;
		return f;
	}

	BNode* findparent(BNode* n) {
		if (n == root) {
			return NULL;
		}
		else {
			BNode* temp = root;
			string checkval = n->keys[0]->Hkey;
			while (true) {
				int movindex = 0;
				for (int i = 0; i < temp->keycount; i++) {
					if ((temp->keys[i]->Hkey > checkval && (temp->keys[i]->Hkey.length() == checkval.length())) || (temp->keys[i]->Hkey.length() > checkval.length())) {
						movindex = i;
						break;
					}
				}
				if (temp->children[movindex] == n) {
					return temp;
				}
				temp = temp->children[movindex];
			}
		}
	}

	void insert(string val, string rv) {
		BNode* temp = root;
		while (temp->isLeaf == 0) {
			int nextindex = temp->keycount;
			for (int i = 0; i < temp->keycount; i++) {
				if ((temp->keys[i]->Hkey > val && (temp->keys[i]->Hkey.length() == val.length())) || (temp->keys[i]->Hkey.length() > val.length())) {
					nextindex = i;
					break;
				}
			}
			temp = temp->children[nextindex];
		}
		if (temp->keycount < maxkey) {
			temp->insertInNode(val, rv);
		}
		else {
			FileNode* temps = new FileNode[temp->keycount + 1];
			int insertloc = temp->keycount;
			for (int i = 0; i < temp->keycount; i++) {
				if ((temp->keys[i]->Hkey > val && !(temp->keys[i]->Hkey.length() < val.length())) || (temp->keys[i]->Hkey.length() > val.length())) {
					insertloc = i;
					break;
				}
			}
			for (int i = temp->keycount; i > insertloc; i--) {
				temps[i] = *temp->keys[i - 1];
			}
			temps[insertloc].Hkey = val;
			temps[insertloc].value = rv;
			for (int i = 0; i < insertloc; i++) {
				temps[i] = *temp->keys[i];
			}
			BNode* left = new BNode(maxchild);
			BNode* right = new BNode(maxchild);
			int mid = (temp->keycount) / 2;
			for (int i = 0; i < mid; i++) {
				left->insertInNode(temps[i].Hkey, temps[i].value);
			}
			for (int i = mid + 1; i < temp->keycount + 1; i++) {
				right->insertInNode(temps[i].Hkey, temps[i].value);
			}
			left->isLeaf = 1;
			right->isLeaf = 1;
			FileNode lifter = temps[mid];
			while (true) {
				BNode* parent = findparent(temp);
				//add for null
				if (parent == NULL) {
					parent = root;
					temp = new BNode(maxchild);
					temp->insertInNode(lifter.Hkey, lifter.value);
					temp->children[0] = left;
					temp->children[1] = right;
					temp->childcount = 2;
					root = temp;
					break;
				}
				if (parent->keycount < maxkey) {
					int loc = parent->insertInNode(lifter.Hkey, lifter.value);
					parent->children[loc] = left;
					parent->children[loc + 1] = right;
					break;
				}
				else {
					insertloc = parent->keycount;
					BNode** tempchildren = new BNode * [parent->keycount + 2];
					for (int i = 0; i < parent->keycount + 2; i++) {
						tempchildren[i] = NULL;
					}
					temps = new FileNode[parent->keycount + 1];
					for (int i = 0; i < parent->keycount; i++) {
						if (parent->keys[i]->Hkey > lifter.Hkey && !(parent->keys[i]->Hkey.length() < lifter.Hkey.length()) || (parent->keys[i]->Hkey.length() > lifter.Hkey.length())) {
							insertloc = i;
							break;
						}
					}
					for (int i = parent->keycount; i > insertloc; i--) {
						temps[i] = *parent->keys[i - 1];
					}
					for (int i = parent->keycount + 1; i > insertloc + 1; i--) {
						tempchildren[i] = parent->children[i - 1];
					}
					tempchildren[insertloc] = left;
					tempchildren[insertloc + 1] = right;
					for (int i = 0; i < insertloc; i++) {
						tempchildren[i] = parent->children[i];
					}
					temps[insertloc] = lifter;
					for (int i = 0; i < insertloc; i++) {
						temps[i] = *parent->keys[i];
					}




					BNode* left2 = new BNode(maxchild);
					BNode* right2 = new BNode(maxchild);
					int mid2 = (parent->keycount) / 2;
					for (int i = 0; i < mid2; i++) {
						left2->insertInNode(temps[i].Hkey, temps[i].value);
					}
					for (int i = 0; i <= mid2; i++) {
						left2->children[i] = tempchildren[i];
					}
					left2->childcount = left2->keycount + 1;
					for (int i = mid2 + 1; i < parent->keycount + 1; i++) {
						right2->insertInNode(temps[i].Hkey, temps[i].value);
					}
					for (int i = mid2 + 1; i <= parent->keycount + 1; i++) {
						right2->children[i - mid2 - 1] = tempchildren[i];
					}
					right2->childcount = right2->keycount + 1;
					lifter = temps[mid2];
					left = left2;
					right = right2;
					temp = parent;
				}

			}
		}
	}
	//
	void deleteNode(string val) {
		BNode* temp = root;
		int foundindex = -1;
		while (temp != NULL) {
			int nextindex = temp->keycount;
			for (int i = 0; i < temp->keycount; i++) {
				if (temp->keys[i]->Hkey == val) {
					foundindex = i;
					break;
				}
				if ((temp->keys[i]->Hkey > val && (temp->keys[i]->Hkey.length() == val.length())) || (temp->keys[i]->Hkey.length() > val.length())) {
					nextindex = i;
					break;
				}
			}
			if (foundindex != -1) {
				break;
			}
			temp = temp->children[nextindex];
		}

		if (temp == NULL) {
			cout << val << " Not found" << endl;
		}
		else {
			cout << "Found: " << temp->keys[foundindex]->Hkey << " (" << temp->keys[foundindex]->value << ")" << endl;
			if (temp->isLeaf == 1) {
				for (int i = foundindex; i < temp->keycount - 1; i++) {
					temp->keys[i]->Hkey = temp->keys[i + 1]->Hkey;
				}
				temp->keycount--;
			}
			else {
				BNode* traverse = temp->children[foundindex];
				while (traverse->isLeaf == 0) {
					traverse = traverse->children[traverse->keycount];
				}
				temp->keys[foundindex]->Hkey = traverse->keys[traverse->keycount - 1]->Hkey;
				temp->keys[foundindex]->value = traverse->keys[traverse->keycount - 1]->value;
				traverse->keycount--;
				temp = traverse;
			}

			while (temp->keycount < minkey) {
				BNode* parent = findparent(temp);
				if (parent == NULL) {
					return;
				}
				int parentindex = -1;
				int parentkeyindex = -1;
				for (int i = 0; i < parent->keycount + 1; i++) {
					if (parent->children[i] == temp) {
						parentindex = i;
						break;
					}
				}
				parentkeyindex = parentindex;
				if (parentkeyindex == parent->keycount) {
					parentkeyindex--;
				}
				int pindexleft = parentindex - 1;
				int pindexright = parentindex + 1;
				//sharing

				if (pindexleft >= 0 && parent->children[pindexleft]->keycount > minkey) {
					parentkeyindex = parentindex - 1;
					BNode* tempchild = parent->children[pindexleft]->children[parent->children[pindexleft]->keycount];

					parent->children[parentindex]->keycount++;
					for (int i = parent->children[parentindex]->keycount - 1; i > 0; i--) {
						parent->children[parentindex]->keys[i]->Hkey = parent->children[parentindex]->keys[i - 1]->Hkey;
						parent->children[parentindex]->keys[i]->value = parent->children[parentindex]->keys[i - 1]->value;
					}
					for (int i = parent->children[parentindex]->keycount; i > 0; i--) {
						parent->children[parentindex]->children[i] = parent->children[parentindex]->children[i - 1];
					}
					//what
					parent->children[parentindex]->keys[0]->Hkey = parent->keys[parentkeyindex]->Hkey;
					parent->children[parentindex]->keys[0]->value = parent->keys[parentkeyindex]->value;

					parent->children[parentindex]->children[0] = tempchild;

					parent->keys[parentkeyindex]->Hkey = parent->children[pindexleft]->keys[parent->children[pindexleft]->keycount - 1]->Hkey;
					parent->keys[parentkeyindex]->value = parent->children[pindexleft]->keys[parent->children[pindexleft]->keycount - 1]->value;

					parent->children[pindexleft]->keycount--;
				}
				else if (pindexright <= parent->keycount && parent->children[pindexright]->keycount > minkey) {
					parentkeyindex = parentindex;
					BNode* tempchild = parent->children[pindexright]->children[0];

					parent->children[parentindex]->keycount++;
					parent->children[parentindex]->keys[parent->children[parentindex]->keycount - 1]->Hkey = parent->keys[parentkeyindex]->Hkey;
					parent->children[parentindex]->keys[parent->children[parentindex]->keycount - 1]->value = parent->keys[parentkeyindex]->value;

					parent->children[parentindex]->children[parent->children[parentindex]->keycount] = tempchild;

					parent->keys[parentkeyindex]->Hkey = parent->children[pindexright]->keys[0]->Hkey;
					parent->keys[parentkeyindex]->value = parent->children[pindexright]->keys[0]->value;

					for (int i = 0; i < parent->children[pindexright]->keycount - 1; i++) {
						parent->children[pindexright]->keys[i]->Hkey = parent->children[pindexright]->keys[i + 1]->Hkey;
						parent->children[pindexright]->keys[i]->value = parent->children[pindexright]->keys[i + 1]->value;
						parent->children[pindexright]->children[i] = parent->children[pindexright]->children[i + 1];
					}
					parent->children[pindexright]->children[parent->children[pindexright]->keycount - 1] = parent->children[pindexright]->children[parent->children[pindexright]->keycount];
					parent->children[pindexright]->keycount--;

				}
				else if (pindexleft >= 0) {
					parentkeyindex = parentindex - 1;
					BNode* newadderchild = new BNode(maxchild);
					for (int i = 0; i < parent->children[pindexleft]->keycount; i++) {
						newadderchild->insertInNode(parent->children[pindexleft]->keys[i]->Hkey, parent->children[pindexleft]->keys[i]->value);
					}
					newadderchild->insertInNode(parent->keys[parentkeyindex]->Hkey, parent->keys[parentkeyindex]->value);
					for (int i = 0; i < parent->children[parentindex]->keycount; i++) {
						newadderchild->insertInNode(parent->children[parentindex]->keys[i]->Hkey, parent->children[parentindex]->keys[i]->value);
					}

					for (int i = 0; i < parent->children[pindexleft]->keycount + 1; i++) {
						newadderchild->children[i] = parent->children[pindexleft]->children[i];
					}

					for (int i = parent->children[pindexleft]->keycount + 1; i < parent->children[parentindex]->keycount + parent->children[pindexleft]->keycount + 2; i++) {
						newadderchild->children[i] = parent->children[parentindex]->children[i - (parent->children[pindexleft]->keycount + 1)];
					}

					for (int i = parentkeyindex; i < parent->keycount - 1; i++) {
						parent->keys[i]->Hkey = parent->keys[i + 1]->Hkey;
						parent->keys[i]->value = parent->keys[i + 1]->value;
						parent->children[i] = parent->children[i + 1];
					}
					parent->children[parent->keycount - 1] = parent->children[parent->keycount];
					parent->keycount--;

					parent->children[parentindex - 1] = newadderchild;
					if (newadderchild->children[0] == NULL) {
						newadderchild->isLeaf = 1;
					}
					if (parent == root && root->keycount == 0) {
						root = parent->children[parentindex - 1];
						return;
					}
				}
				else {
					parentkeyindex = parentindex;
					BNode* newadderchild = new BNode(maxchild);
					for (int i = 0; i < parent->children[parentindex]->keycount; i++) {
						newadderchild->insertInNode(parent->children[parentindex]->keys[i]->Hkey, parent->children[parentindex]->keys[i]->value);
					}
					newadderchild->insertInNode(parent->keys[parentkeyindex]->Hkey, parent->keys[parentkeyindex]->value);
					for (int i = 0; i < parent->children[pindexright]->keycount; i++) {
						newadderchild->insertInNode(parent->children[pindexright]->keys[i]->Hkey, parent->children[pindexright]->keys[i]->value);
					}


					for (int i = 0; i < parent->children[parentindex]->keycount + 1; i++) {
						newadderchild->children[i] = parent->children[parentindex]->children[i];
					}

					for (int i = parent->children[parentindex]->keycount + 1; i < parent->children[parentindex]->keycount + parent->children[pindexright]->keycount + 2; i++) {
						newadderchild->children[i] = parent->children[pindexright]->children[i - (parent->children[parentindex]->keycount + 1)];
					}


					for (int i = parentkeyindex; i < parent->keycount - 1; i++) {
						parent->keys[i]->Hkey = parent->keys[i + 1]->Hkey;
						parent->keys[i]->value = parent->keys[i + 1]->value;
						parent->children[i] = parent->children[i + 1];
					}
					parent->children[parent->keycount - 1] = parent->children[parent->keycount];
					parent->keycount--;

					parent->children[parentindex] = newadderchild;
					if (newadderchild->children[0] == NULL) {
						newadderchild->isLeaf = 1;
					}
					if (parent == root && root->keycount == 0) {
						root = parent->children[parentindex];
						return;
					}
				}

				temp = parent;
			}
		}

	}
};
