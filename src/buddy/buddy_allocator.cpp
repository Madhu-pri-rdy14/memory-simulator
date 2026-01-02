#include "allocator.h"
#include <cmath>
#include <iostream>

BuddyAllocator::BuddyAllocator() : max_order(0) {}

void BuddyAllocator::init(size_t size) {

    int order = get_order(size);
    size_t actual_size = 1 << order;
    
    this->total_size = actual_size;
    this->max_order = order;

    free_lists.resize(max_order + 1);
    
    Block* initial_block = new Block(-1, actual_size, 0, true, nullptr);
    free_lists[max_order].push_back(initial_block);
    
    head = initial_block; 
    
    std::cout << "Buddy System initialized with " << actual_size << " bytes (Order " << max_order << ")\n";
}

void* BuddyAllocator::malloc(size_t size) {
 
    int k = get_order(size);
    if (k > max_order) {
        std::cout << "Request too large.\n";
        return nullptr;
    }

    int i = k;
    while (i <= max_order && free_lists[i].empty()) {
        i++;
    }

    if (i > max_order) {
        std::cout << "Out of memory.\n";
        return nullptr;
    }

    while (i > k) {
        Block* block = free_lists[i].back();
        free_lists[i].pop_back();

        int new_order = i - 1;
        size_t half_size = 1 << new_order;
        
        Block* buddy1 = block; 
        buddy1->size = half_size;
        
        Block* buddy2 = new Block(-1, half_size, buddy1->start_addr + half_size, true, nullptr);
   
        free_lists[new_order].push_back(buddy2);
        free_lists[new_order].push_back(buddy1);
        
        i--;
    }

    Block* allocated = free_lists[k].back();
    free_lists[k].pop_back();
    
    allocated->is_free = false;
    allocated->id = next_id++;
    
    std::cout << "Buddy Allocated ID " << allocated->id << " at " << allocated->start_addr 
              << " (Order " << k << ")\n";
    
    return (void*)allocated->start_addr;
}

void BuddyAllocator::free(int id) {
    
    std::cout << "Buddy free not fully implemented in this snippet.\n";
}