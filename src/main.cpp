#include <iostream>
#include <string>
#include "allocator.h"
#include "cache.h"
#include "page_table.h" 

int main() {
    Allocator* allocator = nullptr;
    L1Cache l1;
    L2Cache l2;
    PageTable mmu; 

    std::string cmd;
    size_t val;

    std::cout << "=== Memory Simulator ===\n";
    std::cout << "Commands: \n";
    std::cout << "  init <size>      : Initialize physical memory\n";
    std::cout << "  strategy <name>  : Set strategy (first/best/worst/buddy)\n";
    std::cout << "  malloc <size>    : Allocate physical memory\n";
    std::cout << "  map <virt> <phys>: Map a virtual address to physical (create page entry)\n";
    std::cout << "  access <virt>    : CPU accesses a Virtual Address\n";
    std::cout << "  stats            : Show Cache & Memory stats\n";

    while(true) {
        std::cout << "> ";
        std::cin >> cmd;

        if(cmd == "exit") break;
        
        if(cmd == "strategy") {
            std::string strat;
            std::cin >> strat;
            if(allocator) delete allocator;
            
            if(strat == "first") allocator = new FirstFit();
            else if(strat == "best") allocator = new BestFit();
            else if(strat == "worst") allocator = new WorstFit();
            else if(strat == "buddy") allocator = new BuddyAllocator();
            else std::cout << "Unknown strategy\n";
        }
        else if(cmd == "init") {
            std::cin >> val;
            if(allocator) allocator->init(val);
            else std::cout << "Set strategy first!\n";
        }
        else if(cmd == "malloc") {
            std::cin >> val;
            if(allocator) allocator->malloc(val);
        }
        else if(cmd == "free") {
            int id;
            std::cin >> id;
            if(allocator) allocator->free(id);
        }
        else if(cmd == "map") {
            int v_addr, p_addr;
            std::cin >> v_addr >> p_addr;
            mmu.map(v_addr, p_addr);
        }
        else if(cmd == "access") {
            int v_addr;
            std::cin >> v_addr;
            std::cout << "CPU Requesting Virtual Addr: " << v_addr << "\n";
            
            int p_addr = mmu.translate(v_addr);
            
            if (p_addr == -1) {
                std::cout << "-> PAGE FAULT! (Page not mapped)\n";
            } else {
                std::cout << "-> MMU Translated to Physical Addr: " << p_addr << "\n";
                
                if (l1.access(p_addr)) {
                    std::cout << "-> L1 Cache HIT\n";
                } else {
                    std::cout << "-> L1 Miss -> Checking L2...\n";
                    if (l2.access(p_addr)) {
                        std::cout << "-> L2 Cache HIT\n";
                    } else {
                        std::cout << "-> L2 Miss -> Fetching from Main Memory\n";
                    }
                }
            }
        }
        else if(cmd == "stats") {
            l1.print_stats();
            l2.print_stats();
            if(allocator) allocator->print_stats();
        }
        else if(cmd == "dump") {
             if(allocator) allocator->dump();
        }
    }
    return 0;
}