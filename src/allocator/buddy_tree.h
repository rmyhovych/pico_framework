//
// Created by rmyho on 5/31/2020.
//

#ifndef PICOFRAMEWORK_BUDDY_TREE_H
#define PICOFRAMEWORK_BUDDY_TREE_H

#include <vector>

struct BuddyNode
{
	bool isTaken = false;
	int rowPosition = 0;

	BuddyNode* left = nullptr;
	BuddyNode* right = nullptr;
};


class BuddyTree
{
public:
	BuddyTree(int pageSize, int startingOrder);

	~BuddyTree();

	void addOrder();

	int findFreeOffset(int size);

	int size() const;

private:
	int getNodeSize(int nodeOrder) const;

	BuddyNode* getFreeNode(BuddyNode* subroot, int rootOrder, int neededOrder);

	BuddyNode* addNewNode();

private:
	const int PAGE_SIZE_;
	int rootOrder_;

	BuddyNode* root_;

	std::vector<BuddyNode*> nodes_;
};


#endif //PICOFRAMEWORK_BUDDY_TREE_H
