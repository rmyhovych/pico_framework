//
// Created by rmyho on 6/4/2020.
//

#include "buddy_tree.h"


BuddyTree::Node::Node() :
		offset(0),
		isTaken(false)
{
}


BuddyTree::Node::~Node()
{
	delete left;
	delete right;
}


bool BuddyTree::Node::deleteChild(BuddyTree::Node* child)
{
	if (left == child)
	{
		delete left;
		left = nullptr;
	}
	else if (right == child)
	{
		delete right;
		right = nullptr;
	}
	else if (left != nullptr && left->deleteChild(child))
	{
		if (left->left == nullptr && left->right == nullptr)
		{
			delete left;
			left = nullptr;
		}
	}
	else if (right != nullptr && right->deleteChild(child))
	{
		if (right->left == nullptr && right->right == nullptr)
		{
			delete right;
			right = nullptr;
		}
	}
	else
	{
		return false;
	}

	return true;
}
