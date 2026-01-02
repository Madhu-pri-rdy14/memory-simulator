#include "allocator.h"

void* WorstFit::malloc(size_t size) {
    Block* worst = nullptr;
    Block* curr = head;

    while(curr) {
        if(curr->is_free && curr->size >= size) {
            if(!worst || curr->size > worst->size) {
                worst = curr;
            }
        }
        curr = curr->next;
    }

    if(worst) {
        if(worst->size > size) {
            Block* newBlock = new Block(-1, worst->size - size, worst->start_addr + size, true, worst->next);
            worst->size = size;
            worst->next = newBlock;
        }
        worst->is_free = false;
        worst->id = next_id++;
        std::cout << "Allocated ID " << worst->id << " at " << worst->start_addr << "\n";
        return (void*)worst->start_addr;
    }
    std::cout << "Allocation failed\n";
    return nullptr;
}