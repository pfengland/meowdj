#ifndef H_DYNAMICARRAY
#define H_DYNAMICARRAY

static const int dynamicarray_initsize = 100;

typedef struct dynamicarray_s dynamicarray;
struct dynamicarray_s {
     int count;
     int size;
     void *items;
};

dynamicarray* dynamicarray_create(void);
void dynamicarray_free(dynamicarray *a);

#endif
