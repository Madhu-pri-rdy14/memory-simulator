#include "allocator.h"

void* FirstFit::malloc(size_t size) {
    Block* curr = head;
    while(curr) {
        if(curr->is_free && curr->size >= size) {
            if(curr->size > size) {
                Block* newBlock = new Block(-1, curr->size - size, curr->start_addr + size, true, curr->next);
                curr->size = size;
                curr->next = newBlock;
            }
            curr->is_free = false;
            curr->id = next_id++;
            std::cout << "Allocated ID " << curr->id << " at " << curr->start_addr << "\n";
            return (void*)curr->start_addr;
        }
        curr = curr->next;
    }
    std::cout << "Allocation failed\n";
    return nullptr;
}