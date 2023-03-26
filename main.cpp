#include <cstddef>
#include <vector>
#include <iostream>
#include <array>
#include <limits>

template <class T, size_t ChunkSize, size_t NumPools>
class PoolAllocator {
public:
    using pointer = T*;
    using size_type = size_t;

    PoolAllocator() noexcept {}

    template <class U>
    PoolAllocator(const PoolAllocator<U, ChunkSize, NumPools>& other) noexcept {}

    pointer allocate(size_type n) {
        if (n > 1) {
            throw std::bad_alloc();
        }
        for (size_t i = 0; i < NumPools; ++i) {
            if (chunkSize[i] >= sizeof(T) && !pool[i].empty()) {
                pointer block = pool[i].back();
                pool[i].pop_back();
                return block;
            }
        }
        throw std::bad_alloc();
    }

    void deallocate(pointer p, size_type n) noexcept {
        if (n > 1) {
            return;
        }
        size_t i;
        for (i = 0; i < NumPools; ++i) {
            if (p >= start[i] && p < start[i] + poolSize[i]) {
                break;
            }
        }
        if (i == NumPools) {
            return;
        }
        pool[i].push_back(p);
    }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

private:
    // Array of pools
    std::array<std::vector<pointer>, NumPools> pool;
    // Array of pointers to the start of each pool
    std::array<pointer, NumPools> start;
    // Array of sizes of each pool
    std::array<size_type, NumPools> poolSize;
    // Array of chunk sizes of each pool
    std::array<size_type, NumPools> chunkSize;

};

int main() {
    return 0;
}
