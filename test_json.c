#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "json.h"

void debug_array(struct array *array, int depth);

void debug_element(struct element element, int depth) {
    int i;
    for (i = 0; i < depth; ++i) {
        printf(" ");
    }
    if (element.type == TYPE_STRING) {
        printf("%s\n", element.u.s);
    }
    else {
        debug_array(element.u.array, depth);
    }
}

void debug_array(struct array *array, int depth) {
    int i;

    printf("\n");
    for (i = 0; i < array->i; ++i) {
        debug_element(array->elements[i], depth+1);
    }
    printf("\n");
}

void debug(struct result *result) {
    int i;

    printf("Strings:\n");
    for (i = 0; i < result->i_string; ++i) {
        printf("%s\n", result->strings[i]);
    }

    printf("---\n\n");
    for (i = 0; i < result->i_array; ++i) {
        printf("\narray %d\n", i);
        debug_array(result->arrays[i], 0);
        printf("\n");
    }
}

int main(int argc, char **argv) {
    struct result result;

    init_result(&result);
    int rc = parse("     [\"hello\", [\"a\", \"b\"],    \"you\"]\n", &result);
    printf("rc = %d\n", rc);
    debug(&result);
    printf("allocations = %d\n", result.allocations);
    release_result(&result);
    printf("after releasing, net allocations = %d\n", result.allocations);
}

