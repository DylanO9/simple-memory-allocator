// Design: First-fit allocation
#include <stdio.h>
#include <string.h>

#define NALLOC  1024 /* minimum #units to request */

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

void *my_malloc(size_t);
static Header *morecore(unsigned);
void my_free(void *ap);

int main() {
    char *new_name = my_malloc(10);
    new_name[0] = '\0';
    strcat(new_name, "Hi");
    printf("new name: %s\n", new_name);
    return 0;
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
    // Okay, so we are given the pointer to free
    // This pointer is the memory that we gave them, so one sizeof(Header) backwards means that is where our first header lies

    // When we free what are we doing? We are adding the block back to the free list that we have
    // The memory inside of this is potentially already been messed up with garbage inside
    
    // Our goal of this is to take the memory, potentially clear it, then place the memory back into our list
    Header *ptr = (Header *)ap - 1; // Cast ap to a Header, so subtracting one subtracts the size of one Header element    
    
    // We want to place the block, so that the list stays in an order of growing addresses. We can compare addresses by just using the value of the pointers
    Header *p, *prevp;
    for (p = freep; !(p < ptr && ptr < ptr->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (p > ptr || ptr > p->s.ptr ))
            break;
    // Now we have the spot to insert
    // Coalesce if the addresses are adjacent
    if (ptr + ptr->s.size == p->s.ptr) {
        ptr->s.size += p->s.ptr->s.size;
        ptr->s.ptr = p->s.ptr->s.ptr; 
    } else
        ptr->s.ptr = p->s.ptr;
    if (p + p->s.size == ptr) {
        p->s.size += ptr->s.size;
        p->s.ptr = ptr->s.ptr;
    } else
        p->s.ptr = ptr; 
    freep = p;        
}

static Header *morecore(unsigned nu) {
   
}
