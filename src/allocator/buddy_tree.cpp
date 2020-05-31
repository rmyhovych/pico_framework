//
// Created by rmyho on 5/31/2020.
//

#include "buddy_tree.h"


BuddyTree::BuddyTree(int pageSize, int startingOrder) :
		PAGE_SIZE_(pageSize),
		rootOrder_(startingOrder),

		root_(new BuddyNode),

		nodes_(0)
{
	nodes_.push_back(root_);
}

BuddyTree::~BuddyTree()
{
	for (BuddyNode* n : nodes_)
	{
		delete n;
	}

	nodes_.resize(0);
}

void BuddyTree::addOrder()
{
	BuddyNode* oldRoot = root_;
	root_ = new BuddyNode;
	root_->left = oldRoot;
	++rootOrder_;
}

int BuddyTree::findFreeOffset(int size)
{
	int neededOrder = size / PAGE_SIZE_;
	if (neededOrder > rootOrder_)
	{
		return -1;
	}

	BuddyNode* freeNode = getFreeNode(root_, rootOrder_, neededOrder);
	if (freeNode == nullptr)
	{
		return -1;
	}

	freeNode->isTaken = true;
	return freeNode->rowPosition * getNodeSize(neededOrder);
}

int BuddyTree::getNodeSize(int nodeOrder)
{
	return (nodeOrder + 1) * PAGE_SIZE_;
}

BuddyNode* BuddyTree::getFreeNode(BuddyNode* subroot, int rootOrder, int neededOrder)
{
	if (subroot->isTaken)
	{
		return nullptr;
	}

	if (rootOrder == neededOrder)
	{
		if (subroot->left == nullptr)
		{
			return subroot;
		}

		return nullptr;
	}

	if (subroot->left == nullptr)
	{
		subroot->left = addNewNode();
		subroot->left->rowPosition = subroot->rowPosition * 2;
	}

	int lowerOrder = rootOrder - 1;

	BuddyNode* nodeFound = getFreeNode(subroot->left, lowerOrder, neededOrder);
	if (nodeFound == nullptr)
	{
		if (subroot->right == nullptr)
		{
			subroot->right = addNewNode();
			subroot->right->rowPosition = subroot->rowPosition * 2 + 1;
		}

		nodeFound = getFreeNode(subroot->right, lowerOrder, neededOrder);
	}

	return nodeFound;
}

BuddyNode* BuddyTree::addNewNode()
{
	BuddyNode* newNode = new BuddyNode;
	nodes_.push_back(newNode);
	return newNode;
}
