#include <stdio.h>
#include <stdlib.h>
#include <core/interfaces.h>
#include <data/array.h>

void* my_alloc(void* ptr, size_t size) {
    if (size == 0) { free(ptr); return NULL; }
    return realloc(ptr, size);
}
fn_allocator* default_allocator = my_alloc;
int logger_depth = 0;
void logger_pad() {}
void log_error(const char* fmt, ...) {}

int main() {
    array* arr = NULL;
    array_construct(&arr, 10);
    array_add(arr, (void*)1);
    void* discard;
    int res = array_remove(arr, -1, &discard);
    printf("res=%d size=%zu\n", res, array_size(arr));
    return 0;
}
