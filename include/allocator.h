#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

struct Block {
    int id;
    size_t size;
    size_t start_addr;
    bool is_free;
    Block* next;

    Block(int _id, size_t _size, size_t _addr, bool _free, Block* _next = nullptr)
        : id(_id), size(_size), start_addr(_addr), is_free(_free), next(_next) {}
};

class Allocator {
protected:
    size_t total_size;
    Block* head;
    int next_id;

public:
    Allocator() : total_size(0), head(nullptr), next_id(1) {}
    virtual ~Allocator() {
        Block* curr = head;
        while(curr) { Block* tmp = curr; curr = curr->next; delete tmp; }
    }

    virtual void init(size_t size) {
        total_size = size;
        head = new Block(-1, size, 0, true, nullptr);
        std::cout << "Memory initialized: " << size << " bytes\n";
    }

    virtual void* malloc(size_t size) = 0;

    virtual void free(int id) {
        Block* curr = head;
        bool found = false;
        while(curr) {
            if(!curr->is_free && curr->id == id) {
                curr->is_free = true;
                curr->id = -1;
                found = true;
                std::cout << "Block " << id << " freed.\n";
                break;
            }
            curr = curr->next;
        }
        if(found) coalesce();
        else std::cout << "Block " << id << " not found.\n";
    }

    void coalesce() {
        Block* curr = head;
        while(curr && curr->next) {
            if(curr->is_free && curr->next->is_free) {
                Block* temp = curr->next;
                curr->size += temp->size;
                curr->next = temp->next;
                delete temp;
            } else {
                curr = curr->next;
            }
        }
    }

    virtual void dump() {
        std::cout << "Memory Map:\n";
        Block* curr = head;
        while(curr) {
            std::cout << "[" << curr->start_addr << "-" << (curr->start_addr + curr->size -1) << "] "
                      << (curr->is_free ? "FREE" : "USED") << " Size: " << curr->size << "\n";
            curr = curr->next;
        }
    }

    virtual void print_stats() {
        size_t used_mem = 0;
        size_t free_mem = 0;
        size_t largest_free_block = 0;
        int free_blocks_count = 0;

        Block* curr = head;
        while(curr) {
            if(curr->is_free) {
                free_mem += curr->size;
                free_blocks_count++;
                if(curr->size > largest_free_block) largest_free_block = curr->size;
            } else {
                used_mem += curr->size;
            }
            curr = curr->next;
        }

        double utilization = (total_size > 0) ? ((double)used_mem / total_size) * 100.0 : 0.0;
        double fragmentation = (free_mem > 0) ? (1.0 - ((double)largest_free_block / free_mem)) * 100.0 : 0.0;

        std::cout << "------ MEMORY STATS ------\n";
        std::cout << "Total memory: " << total_size << "\n";
        std::cout << "Used memory : " << used_mem << "\n";
        std::cout << "Free memory : " << free_mem << "\n";
        std::cout << "Memory utilization: " << std::fixed << std::setprecision(4) << utilization << "%\n";
        std::cout << "External fragmentation: " << std::fixed << std::setprecision(4) << fragmentation << "%\n";
        std::cout << "--------------------------\n";
    }
};

class FirstFit : public Allocator { public: void* malloc(size_t size) override; };
class BestFit  : public Allocator { public: void* malloc(size_t size) override; };
class WorstFit : public Allocator { public: void* malloc(size_t size) override; };

class BuddyAllocator : public Allocator {
private:
    std::vector<std::vector<Block*>> free_lists; 
    int max_order; 

    int get_order(size_t size) {
        int order = 0;
        size_t s = 1;
        while (s < size) { s <<= 1; order++; }
        return order;
    }

public:
    BuddyAllocator();
    void init(size_t size) override;
    void* malloc(size_t size) override;
    void free(int id) override;
    
    void print_stats() override {
        size_t free_mem = 0;
        for(const auto& list : free_lists) {
            for(auto* block : list) {
                free_mem += block->size;
            }
        }
        std::cout << "------ BUDDY STATS ------\n";
        std::cout << "Total: " << total_size << " | Used: " << (total_size - free_mem) << " | Free: " << free_mem << "\n";
        std::cout << "-------------------------\n";
    }
};

#endif