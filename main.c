#include <stdio.h>

void *my_malloc(size_t);

typedef long Align; // long is the most restrictive type. We want to make sure we are always aligning on memory, therefore, we use the most restrictive type
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

void my_free(void *ap);

int main() {

}


void *my_malloc(size_t n) {
    
}

void my_free(void *ap) {

}
