//
// Created by rmyho on 5/31/2020.
//

#ifndef PICOFRAMEWORK_BUDDY_TREE_H
#define PICOFRAMEWORK_BUDDY_TREE_H

#include <vector>

struct BuddyNode
{
	bool isTaken = false;
	int offset = 0;

	BuddyNode* left = nullptr;
	BuddyNode* right = nullptr;
};


class BuddyTree
{
public:
	BuddyTree(int pageSize, int startingOrder);

	~BuddyTree();

	void addOrder();

	BuddyNode* findFreeNode(int size);

	BuddyNode* findTakenNode(int offset);

	int size() const;

private:
	int getNodeSize(int nodeOrder) const;

	BuddyNode* findFreeNode(BuddyNode* subroot, int rootOrder, int neededOrder);

	BuddyNode* findTakenNode(BuddyNode* subroot, int offset);

	void deleteNode(BuddyNode* node);

private:
	const int PAGE_SIZE_;
	int rootOrder_;

	BuddyNode* root_;
};


#endif //PICOFRAMEWORK_BUDDY_TREE_H
