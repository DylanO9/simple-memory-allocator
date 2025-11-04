// Design: First-fit allocation
#include <stdio.h>

#define NALLOC  1024 /* minimum #units to request */

void *my_malloc(size_t);

typedef long Align; // long is the most restrictive type. We want to make sure we are always aligning on memory, therefore, we use the most restrictive type
union header {
    struct {
        union header *ptr; // Pointer to the next free block in the list
        unsigned size; 
    } s;
    Align x;
};
typedef union header Header;
static Header base; // Start with an empty list
static Header *freep = NULL; // Start of the free list

void my_free(void *ap);

int main() {

}

void *my_malloc(size_t n) {
    Header *p, *prevp;
    unsigned nunits;
    
    // Calculate how much memory is needed/the minumum size that we are looking for
        // What variables do we need to calculate or store this? nunits -> unsigned integer -> Why? -> memory shouldn't be 0 or a negative number
    nunits = (n + sizeof(Header) - 1) / sizeof(Header) + 1;

    // What if freep hasn't been allocated yet? initialize base, and assign it it's own address to itself
    if ((prevp = freep) == NULL) {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
            
    // How are we going to find the free block, and what information, state, objects do we need?
        // Once we find the free block, we want to remove it, therefore, we need a way to know what our previous block was -> use a prevp
        // We need an iterator p to move through the blocks
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        // As we are iterating what are we looking for? We want to check if our block has a size greater than or equal to units
        if (p->s.size >= nunits) {
            if (p->s.size == nunits) {
                prevp->s.ptr = p->s.ptr;
            }
            else { // Remove the tail portion that is not needed
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp; // Future searches will begin from where we left off increasing spatial locality and avoid repeated scans from the start
            return (void *)(p + 1); // (p + 1), so we are pointing to the non-header based memory
        } 
        if (p == freep)
            if ((p = morecore(nunits)) == NULL)
                return NULL;
    } 
}

void my_free(void *ap) {

}

static Header *morecore(unsigned nu) {

}
