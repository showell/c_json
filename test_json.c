#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "json.h"

void debug_array(struct json_array *array, int depth);

void debug_json_element(struct json_element json_element, int depth) {
    int i;
    for (i = 0; i < depth; ++i) {
        printf(" ");
    }
    if (json_element.type == JSON_TYPE_STRING) {
        printf("%s\n", json_element.u.s);
    }
    else {
        debug_array(json_element.u.array, depth);
    }
}

void debug_array(struct json_array *array, int depth) {
    int i;

    printf("\n");
    for (i = 0; i < array->i; ++i) {
        debug_json_element(array->json_elements[i], depth+1);
    }
    printf("\n");
}

void debug(struct json_result *result) {
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

void fix_quotes(char *s) {
    while (*s) {
        if (*s == '\'') {
            *s = '"';
        }
        ++s;
    }
}

int main(int argc, char **argv) {
    struct json_result result;

    json_init_result(&result);
    char *s = strdup("     ['hello', ['a', 'b', ['c', 'd', 'e'], 'you']");
    fix_quotes(s);
    printf("%s\n", s);
    int rc = json_parse(s, &result);
    printf("rc = %d\n", rc);
    debug(&result);
    printf("allocations = %d\n", result.allocations);
    json_release_result(&result);
    printf("after releasing, net allocations = %d\n", result.allocations);
    free(s);
}

