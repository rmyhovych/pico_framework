//
// Created by rmyho on 6/6/2020.
//

#ifndef PICOFRAMEWORK_BUDDY_NODE_H
#define PICOFRAMEWORK_BUDDY_NODE_H

#include <cstdint>

class BuddyNode
{
public:
	BuddyNode(uint32_t size, uint32_t offset, bool taken = false);

	BuddyNode(BuddyNode* left);

	~BuddyNode();


	BuddyNode* getLeft();

	BuddyNode* setLeft(bool taken);


	BuddyNode* getRight();

	BuddyNode* setRight(bool taken);


	bool hasChildren() const;

	bool isTaken() const;


	uint32_t getSize() const;

	uint32_t getOffset() const;


	bool deleteDescendant(BuddyNode* descendant);

private:
	bool taken_;

	BuddyNode* left_;
	BuddyNode* right_;

	uint32_t size_;
	uint32_t offset_;
};


#endif //PICOFRAMEWORK_BUDDY_NODE_H
