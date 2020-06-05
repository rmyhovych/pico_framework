//
// Created by rmyho on 5/31/2020.
//

#include "buddy_tree.h"


BuddyTree::BuddyTree(uint32_t pageSize, uint32_t startingOrder) :
		PAGE_SIZE_(pageSize),
		rootOrder_(startingOrder),

		root_(new Node)
{
}

BuddyTree::~BuddyTree()
{
	delete root_;
}

void BuddyTree::addOrder()
{
	Node* oldRoot = root_;
	root_ = new Node;

	if (oldRoot->left != nullptr || oldRoot->right != nullptr || oldRoot->isTaken)
	{
		root_->left = oldRoot;
	}
	else
	{
		delete oldRoot;
	}

	++rootOrder_;
}

BuddyTree::Node* BuddyTree::findFreeNode(uint32_t size)
{
	uint32_t neededOrder = size / PAGE_SIZE_;
	if (neededOrder > rootOrder_)
	{
		return nullptr;
	}

	Node* freeNode = findFreeNode(root_, rootOrder_, neededOrder);
	return freeNode;
}

BuddyTree::Node* BuddyTree::findTakenNode(uint32_t offset)
{
	return nullptr;
}

void BuddyTree::removeNode(BuddyTree::Node* node)
{
	root_->deleteChild(node);
}


uint32_t BuddyTree::size() const
{
	return getNodeSize(rootOrder_);
}

/**------------------------------------------------------------------------------------------------------------*/

uint32_t BuddyTree::getNodeSize(uint32_t nodeOrder) const
{
	return (nodeOrder + 1) * PAGE_SIZE_;
}

BuddyTree::Node* BuddyTree::findFreeNode(BuddyTree::Node* subroot, uint32_t rootOrder, uint32_t neededOrder)
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
		subroot->left = new Node;
		subroot->left->offset = subroot->offset;
	}

	uint32_t lowerOrder = rootOrder - 1;

	Node* nodeFound = findFreeNode(subroot->left, lowerOrder, neededOrder);
	if (nodeFound == nullptr)
	{
		if (subroot->right == nullptr)
		{
			subroot->right = new Node;
			subroot->right->offset = subroot->offset + getNodeSize(neededOrder);
		}

		nodeFound = findFreeNode(subroot->right, lowerOrder, neededOrder);
	}

	return nodeFound;
}

BuddyTree::Node* BuddyTree::findTakenNode(BuddyTree::Node* subroot, uint32_t offset)
{
	if (subroot->offset == offset && subroot->isTaken)
		return subroot;

	Node* newSubroot = offset < subroot->right->offset ? subroot->left : subroot->right;
	if (newSubroot != nullptr)
		return findTakenNode(newSubroot, offset);

	return nullptr;
}
