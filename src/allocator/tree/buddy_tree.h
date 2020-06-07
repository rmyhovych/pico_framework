//
// Created by rmyho on 5/31/2020.
//

#ifndef PICOFRAMEWORK_BUDDY_TREE_H
#define PICOFRAMEWORK_BUDDY_TREE_H

#include <vector>
#include "buddy_node.h"

class BuddyTree
{
public:

	BuddyTree(uint32_t pageSize, uint32_t startingOrder);

	~BuddyTree();

	void addOrder();

	BuddyNode* fillFreeNode(uint32_t size);

	BuddyNode* findTakenNode(uint32_t offset);

	void removeNode(BuddyNode* node);

	uint32_t size() const;

private:
	uint32_t getNodeSize(uint32_t nodeOrder) const;

	BuddyNode* fillFreeNode(BuddyNode* subroot, uint32_t rootOrder, uint32_t neededOrder);

	BuddyNode* findTakenNode(BuddyNode* subroot, uint32_t offset);

private:
	const uint32_t PAGE_SIZE_;
	uint32_t rootOrder_;

	BuddyNode* root_;
};


#endif //PICOFRAMEWORK_BUDDY_TREE_H
