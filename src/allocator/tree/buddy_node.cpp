//
// Created by rmyho on 6/6/2020.
//

#include "buddy_node.h"

BuddyNode::BuddyNode(uint32_t size, uint32_t offset, bool taken) :
		taken_(taken),

		left_(nullptr),
		right_(nullptr),

		size_(size),
		offset_(offset)
{
}

BuddyNode::BuddyNode(BuddyNode* left) :
		taken_(false),

		left_(left),
		right_(nullptr),

		size_(left->size_ * 2),
		offset_(left->offset_)
{

}


BuddyNode::~BuddyNode()
{
	delete left_;
	delete right_;
}


BuddyNode* BuddyNode::getLeft()
{
	return left_;
}

BuddyNode* BuddyNode::setLeft(bool taken)
{
	delete left_;

	uint32_t childSize = size_ / 2;
	left_ = new BuddyNode(childSize, offset_);

	return left_;
}


BuddyNode* BuddyNode::getRight()
{
	return right_;
}

BuddyNode* BuddyNode::setRight(bool taken)
{
	delete right_;

	uint32_t childSize = size_ / 2;
	right_ = new BuddyNode(childSize, offset_ + childSize);

	return right_;
}


bool BuddyNode::hasChildren() const
{
	return left_ != nullptr || right_ != nullptr;
}

bool BuddyNode::isTaken() const
{
	return taken_;
}

uint32_t BuddyNode::getSize() const
{
	return size_;
}

uint32_t BuddyNode::getOffset() const
{
	return offset_;
}


bool BuddyNode::deleteDescendant(BuddyNode* descendant)
{
	if (left_ == descendant)
	{
		delete left_;
		left_ = nullptr;
	}
	else if (right_ == descendant)
	{
		delete right_;
		right_ = nullptr;
	}
	else if (left_ != nullptr && left_->deleteDescendant(descendant))
	{
		if (left_->left_ == nullptr && left_->right_ == nullptr)
		{
			delete left_;
			left_ = nullptr;
		}
	}
	else if (right_ != nullptr && right_->deleteDescendant(descendant))
	{
		if (right_->left_ == nullptr && right_->right_ == nullptr)
		{
			delete right_;
			right_ = nullptr;
		}
	}
	else
	{
		return false;
	}

	return true;
}
