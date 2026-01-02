#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <vector>
#include <iomanip>

struct CacheLine {
    bool valid;
    int tag;
    int last_access;
};

class Cache {
protected:
    std::vector<CacheLine> lines;
    int size;      
    int access_counter; 
    std::string name;

public:
    int hits;
    int misses;

    Cache(std::string _name, int _size) 
        : size(_size), access_counter(0), name(_name), hits(0), misses(0) {
        lines.resize(size, {false, -1, 0});
    }

    virtual ~Cache() {}

    bool access(int addr) {
        access_counter++;
        int index = addr % size; 
        int tag = addr;          

        if (lines[index].valid && lines[index].tag == tag) {
           
            lines[index].last_access = access_counter;
            hits++;
            return true; 
        } else {
            
            lines[index].valid = true;
            lines[index].tag = tag;
            lines[index].last_access = access_counter;
            misses++;
            return false; 
        }
    }

    void print_stats() {
        std::cout << name << " Stats: Hits=" << hits 
                  << " Misses=" << misses 
                  << " Ratio=" << (hits + misses > 0 ? (float)hits/(hits+misses) : 0.0) << "\n";
    }
};

class L1Cache : public Cache {
public:
    L1Cache() : Cache("L1", 16) {} 
};

class L2Cache : public Cache {
public:
    L2Cache() : Cache("L2", 64) {} 
};

#endif