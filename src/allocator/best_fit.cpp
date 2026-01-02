#include "allocator.h"

void* BestFit::malloc(size_t size) {
    Block* best = nullptr;
    Block* curr = head;

    while(curr) {
        if(curr->is_free && curr->size >= size) {
            if(!best || curr->size < best->size) {
                best = curr;
            }
        }
        curr = curr->next;
    }

    if(best) {
        if(best->size > size) {
            Block* newBlock = new Block(-1, best->size - size, best->start_addr + size, true, best->next);
            best->size = size;
            best->next = newBlock;
        }
        best->is_free = false;
        best->id = next_id++;
        std::cout << "Allocated ID " << best->id << " at " << best->start_addr << "\n";
        return (void*)best->start_addr;
    }
    std::cout << "Allocation failed\n";
    return nullptr;
}