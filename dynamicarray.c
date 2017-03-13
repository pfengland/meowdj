#include "dynamicarray.h"
#include <stdlib.h>

dynamicarray* dynamicarray_create(void) {
     dynamicarray *a = malloc(sizeof(*a));
     a->count = 0;
     a->items = NULL;
     a->size = dynamicarray_initsize;
     return a;
}

void dynamicarray_free(dynamicarray *a) {
     free(a);
}
