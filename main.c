#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NALLOC  1024

typedef long Align;
union header {
    struct {
        union header *ptr;
        unsigned size; 
    } s;
    Align x;
};

typedef union header Header;
static Header base; 
static Header *freep = NULL;

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
    
    nunits = (n + sizeof(Header) - 1) / sizeof(Header) + 1;

    if ((prevp = freep) == NULL) {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
            
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {
            if (p->s.size == nunits) {
                prevp->s.ptr = p->s.ptr;
            }
            else { 
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp; 
            return (void *)(p + 1); 
        } 
        if (p == freep)
            if ((p = morecore(nunits)) == NULL)
                return NULL;
    } 
}

void my_free(void *ap) {
    Header *ptr = (Header *)ap - 1; 
    
    Header *p, *prevp;
    for (p = freep; !(p < ptr && ptr < ptr->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (p > ptr || ptr > p->s.ptr ))
            break;

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
    char *cp;
    Header *up;
    
    if (nu < NALLOC)
        nu = NALLOC;
    
    cp = sbrk(nu * sizeof(Header)); 
    if (cp == (char *) -1)
        return NULL;
    up = (Header *) cp;
    up->s.size = nu;
    my_free((void *)(up+1));
    return freep;     
}
