//
// Created by rmyho on 5/31/2020.
//

#include "buddy_tree.h"


BuddyTree::BuddyTree(int pageSize, int startingOrder) :
		PAGE_SIZE_(pageSize),
		rootOrder_(startingOrder),

		root_(new BuddyNode)
{
}

BuddyTree::~BuddyTree()
{
	deleteNode(root_);
}

void BuddyTree::addOrder()
{
	BuddyNode* oldRoot = root_;
	root_ = new BuddyNode;
	root_->left = oldRoot;
	++rootOrder_;
}

BuddyNode* BuddyTree::findFreeNode(int size)
{
	int neededOrder = size / PAGE_SIZE_;
	if (neededOrder > rootOrder_)
	{
		return nullptr;
	}

	BuddyNode* freeNode = findFreeNode(root_, rootOrder_, neededOrder);
	return freeNode;
}

BuddyNode* BuddyTree::findTakenNode(int offset)
{
	return nullptr;
}

int BuddyTree::size() const
{
	return getNodeSize(rootOrder_);
}


int BuddyTree::getNodeSize(int nodeOrder) const
{
	return (nodeOrder + 1) * PAGE_SIZE_;
}

BuddyNode* BuddyTree::findFreeNode(BuddyNode* subroot, int rootOrder, int neededOrder)
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
		subroot->left = new BuddyNode;
		subroot->left->offset = subroot->offset;
	}

	int lowerOrder = rootOrder - 1;

	BuddyNode* nodeFound = findFreeNode(subroot->left, lowerOrder, neededOrder);
	if (nodeFound == nullptr)
	{
		if (subroot->right == nullptr)
		{
			subroot->right = new BuddyNode;
			subroot->right->offset = subroot->offset + getNodeSize(neededOrder);
		}

		nodeFound = findFreeNode(subroot->right, lowerOrder, neededOrder);
	}

	return nodeFound;
}

BuddyNode* BuddyTree::findTakenNode(BuddyNode* subroot, int offset)
{
	if (subroot->offset == offset && subroot->isTaken)
		return subroot;

	BuddyNode* newSubroot = offset < subroot->right->offset ? subroot->left : subroot->right;
	if (newSubroot != nullptr)
		return findTakenNode(newSubroot, offset);

	return nullptr;
}


void BuddyTree::deleteNode(BuddyNode* node)
{
	if (node == nullptr)
		return;

	deleteNode(node->left);
	deleteNode(node->right);

	delete node;
}
