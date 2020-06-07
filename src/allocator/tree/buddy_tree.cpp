//
// Created by rmyho on 5/31/2020.
//

#include "buddy_tree.h"
#include <functional>

BuddyTree::BuddyTree(uint32_t pageSize, uint32_t startingOrder) :
		PAGE_SIZE_(pageSize),
		rootOrder_(startingOrder),

		root_(new BuddyNode(getNodeSize(startingOrder), 0, false))
{
}

BuddyTree::~BuddyTree()
{
	delete root_;
}

void BuddyTree::addOrder()
{
	++rootOrder_;

	BuddyNode* oldRoot = root_;
	if (oldRoot->getLeft() != nullptr || oldRoot->getRight() != nullptr || oldRoot->isTaken())
	{
		root_ = new BuddyNode(oldRoot);
	}
	else
	{
		delete oldRoot;
		root_ = new BuddyNode(getNodeSize(rootOrder_), 0);
	}
}

BuddyNode* BuddyTree::fillFreeNode(uint32_t size)
{
	uint32_t neededOrder = size / PAGE_SIZE_;
	if (neededOrder > rootOrder_)
	{
		return nullptr;
	}

	return fillFreeNode(root_, rootOrder_, neededOrder);
}

BuddyNode* BuddyTree::findTakenNode(uint32_t offset)
{
	return nullptr;
}

void BuddyTree::removeNode(BuddyNode* node)
{
	root_->deleteDescendant(node);
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

BuddyNode* BuddyTree::fillFreeNode(BuddyNode* subroot, uint32_t rootOrder, uint32_t neededOrder)
{
	if (subroot->isTaken())
	{
		return nullptr;
	}


	bool rightFirst = subroot->getLeft() == nullptr && subroot->getRight() != nullptr;
	std::pair<std::function<BuddyNode*(void)>, std::function<void(bool)>> rightFinder(
			[&]() -> BuddyNode*
			{ return subroot->getRight(); },
			[&](bool taken) -> void
			{ subroot->setRight(taken); }
	);

	std::pair<std::function<BuddyNode*(void)>, std::function<void(bool)>> leftFinder(
			[&]() -> BuddyNode*
			{ return subroot->getLeft(); },
			[&](bool taken) -> void
			{ subroot->setLeft(taken); }
	);

	std::vector<std::pair<std::function<BuddyNode*(void)>, std::function<void(bool)>>*> nodeFinders(0);
	if (rightFirst)
	{
		nodeFinders.push_back(&rightFinder);
		nodeFinders.push_back(&leftFinder);
	}
	else
	{
		nodeFinders.push_back(&leftFinder);
		nodeFinders.push_back(&rightFinder);
	}

	uint32_t lowerOrder = rootOrder - 1;
	BuddyNode* nodeFound = nullptr;

	bool isNextOrder = lowerOrder == neededOrder;
	for (auto finder : nodeFinders)
	{
		if (finder->first() == nullptr)
		{
			finder->second(isNextOrder);
			if (isNextOrder)
				return finder->first();
		}

		nodeFound = fillFreeNode(finder->first(), lowerOrder, neededOrder);
		if (nodeFound != nullptr)
			break;
	}

	return nodeFound;
}

BuddyNode* BuddyTree::findTakenNode(BuddyNode* subroot, uint32_t offset)
{
	if (subroot->getOffset() == offset && subroot->isTaken())
		return subroot;

	BuddyNode* newSubroot = offset < subroot->getRight()->getOffset() ? subroot->getLeft() : subroot->getRight();
	if (newSubroot != nullptr)
		return findTakenNode(newSubroot, offset);

	return nullptr;
}
