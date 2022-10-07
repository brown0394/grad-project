#include "AddressMapElement.h"

AddressMapElement::AddressMapElement():numberOfBlocks(0){}

AddressMapElement::~AddressMapElement() {
	clear();
}

void AddressMapElement::addBlock(Block* block, int offset, int page) {//LBA can be written on multiple blocks
	this->blocks.push_back(block);
	this->offsets.push_back(offset);
	this->pages.push_back(page);
}

void AddressMapElement::clear() {
	this->blocks.clear();
	this->offsets.clear();
	this->pages.clear();
	this->numberOfBlocks = 0;
}

std::map<int, int> AddressMapElement::markInvalid() {
	std::map<int, int> lastValidPages;//for to update fullBlock list
	for (int i = 0; i < this->blocks.size(); ++i) {
		if (blocks[i]->isFull()) {//if two block ptrs are pointing same block, recording number of invalid once is enough.
			auto found = lastValidPages.find(blocks[i]->getBlockID());
			if (found == lastValidPages.end()) {
				lastValidPages.insert({ blocks[i]->getBlockID(), blocks[i]->getNumberOfValidPages()});
			}
		}
		this->blocks[i]->markBlock(this->offsets[i], this->pages[i], INVALID);
	}
	return lastValidPages;
}

void AddressMapElement::setNumberOfBlocks(unsigned int number) {
	this->numberOfBlocks = number;
}

unsigned int AddressMapElement::getNumberOfBlocks() {
	return this->numberOfBlocks;
}

void AddressMapElement::unlinkBlock(Block* block) {
	for (int i = 0; i < this->blocks.size(); ++i) {//when a block is erased, should be linked to new block.
		if (blocks[i] == block) {
			this->blocks.erase(this->blocks.begin() + i);
			this->pages.erase(this->pages.begin() + i);
			this->offsets.erase(this->offsets.begin() + i);
			--i;
		}
	}
}