//
// Created by rmyho on 5/31/2020.
//

#include "buddy_tree.h"


BuddyNode::BuddyNode(int offset) :
		rowPosition_(offset),
		taken_(false),
		children_(0)
{
}

BuddyNode::~BuddyNode()
{
	for (BuddyNode* pChild : children_)
	{
		delete pChild;
	}

	children_.resize(0);
}

BuddyNode* BuddyNode::addChild()
{
	int childRowOffset = rowPosition_ * 2 + children_.size();
	children_.push_back(new BuddyNode(childRowOffset));
	return children_.back();
}

BuddyNode* BuddyNode::addChild(BuddyNode* child)
{
	children_.push_back(child);
	return child;
}

bool BuddyNode::isTaken() const
{
	return taken_;
}

void BuddyNode::setTaken(bool taken)
{
	taken_ = taken;
}


int BuddyNode::getRowPosition() const
{
	return rowPosition_;
}

int BuddyNode::size() const
{
	return children_.size();
}

BuddyNode* BuddyNode::operator[](int index)
{
	return children_[index];
}


/**---------------------------------------------------------------------------------------------------------------------*/

BuddyTree::BuddyTree(int startingOrder, int pageSize) :
		root_(new BuddyNode(0)),
		rootOrder_(startingOrder),
		PAGE_SIZE_(pageSize)
{
}

void BuddyTree::addOrder()
{
	BuddyNode* oldRoot = root_;
	root_ = new BuddyNode(0);
	root_->addChild(oldRoot);
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
	freeNode->setTaken(true);

	return freeNode->getRowPosition() * getNodeSize(neededOrder);
}

int BuddyTree::getNodeSize(int nodeOrder)
{
	return nodeOrder * PAGE_SIZE_;
}

BuddyNode* BuddyTree::getFreeNode(BuddyNode* subroot, int rootOrder, int neededOrder)
{
	if (subroot->isTaken())
	{
		return nullptr;
	}

	if (rootOrder == neededOrder)
	{
		if (subroot->size() == 0)
		{
			return subroot;
		}

		return nullptr;
	}

	if (subroot->size() == 0)
	{
		subroot->addChild();
	}

	int subrootSize = subroot->size();
	for (int i = 0; i < subrootSize; ++i)
	{
		BuddyNode* node = getFreeNode((*subroot)[i], rootOrder - 1, neededOrder);
		if (node != nullptr)
		{
			return node;
		}
	}

	return nullptr;
}
