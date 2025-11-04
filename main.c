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

int main() {

}


void *my_malloc(size_t n) {
    
}
