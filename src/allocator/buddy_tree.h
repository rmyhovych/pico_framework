//
// Created by rmyho on 5/31/2020.
//

#ifndef PICOFRAMEWORK_BUDDY_TREE_H
#define PICOFRAMEWORK_BUDDY_TREE_H

#include <vector>

class BuddyNode
{
public:
	explicit BuddyNode(int offset);

	~BuddyNode();

	BuddyNode* addChild();

	BuddyNode* addChild(BuddyNode* child);

	bool isTaken() const;

	void setTaken(bool taken);

	int getRowPosition() const ;

	int size() const;

	BuddyNode* operator[](int index);

private:
	bool taken_;
	int rowPosition_;

	std::vector<BuddyNode*> children_;
};


/**---------------------------------------------------------------------------------------------------------------------*/

class BuddyTree
{
public:
	BuddyTree(int startingOrder, int pageSize);

	void addOrder();

	int findFreeOffset(int size);

private:
	int getNodeSize(int nodeOrder);

	BuddyNode* getFreeNode(BuddyNode* subroot, int rootOrder, int neededOrder);

private:
	BuddyNode* root_;
	int rootOrder_;

	const int PAGE_SIZE_;
};


#endif //PICOFRAMEWORK_BUDDY_TREE_H
