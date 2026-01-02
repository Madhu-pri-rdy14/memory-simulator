#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <iostream>
#include <vector>
#include <map>

struct PageTableEntry {
    int frame_number; 
    bool valid;       
};

class PageTable {
private:
    std::map<int, PageTableEntry> table; 
    int page_size;

public:
    PageTable() : page_size(64) {} 

    void map(int v_addr, int p_addr) {
        int vpn = v_addr / page_size;
        int pfn = p_addr / page_size;
        table[vpn] = {pfn, true};
        std::cout << "MMU: Mapped Virtual Page " << vpn << " to Physical Frame " << pfn << "\n";
    }

    int translate(int v_addr) {
        int vpn = v_addr / page_size;
        int offset = v_addr % page_size;

        if (table.find(vpn) != table.end() && table[vpn].valid) {
            int pfn = table[vpn].frame_number;
            return (pfn * page_size) + offset;
        }
        return -1; 
    }
};

#endif