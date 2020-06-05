//
// Created by rmyho on 5/31/2020.
//

#ifndef PICOFRAMEWORK_BUDDY_TREE_H
#define PICOFRAMEWORK_BUDDY_TREE_H

#include <vector>

class BuddyTree
{
public:
	struct Node
	{
		uint32_t offset;
		bool isTaken;

		Node* left = nullptr;
		Node* right = nullptr;

		Node();

		~Node();

		bool deleteChild(Node* child);
	};

	BuddyTree(uint32_t pageSize, uint32_t startingOrder);

	~BuddyTree();

	void addOrder();

	Node* findFreeNode(uint32_t size);

	Node* findTakenNode(uint32_t offset);

	void removeNode(Node* node);

	uint32_t size() const;

private:
	uint32_t getNodeSize(uint32_t nodeOrder) const;

	Node* findFreeNode(Node* subroot, uint32_t rootOrder, uint32_t neededOrder);

	Node* findTakenNode(Node* subroot, uint32_t offset);

private:
	const uint32_t PAGE_SIZE_;
	uint32_t rootOrder_;

	Node* root_;
};


#endif //PICOFRAMEWORK_BUDDY_TREE_H
