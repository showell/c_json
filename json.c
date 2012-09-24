#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "json.h"

void release_result(struct json_result *result) {
    int i;

    for (i = 0; i < result->i_string; ++i) {
        free(result->strings[i]);
        result->allocations -= 1;
    }
    for (i = 0; i < result->i_array; ++i) {
        free(result->arrays[i]->json_elements);
        free(result->arrays[i]);
        result->allocations -= 2;
    }
}

struct json_array *visit_array_start(struct json_result *result) {
    struct json_array *array;
    if (result->i_array >= MAX_NUM_ARRAYS) {
        return NULL;
    }
    array = malloc(sizeof(struct json_array));
    if (!array) return 0;
    array->json_elements = malloc(MAX_ARRAY_SIZE * sizeof(struct json_element));
    if (!array->json_elements) return 0;
    result->allocations += 2;
    array->i = 0;
    result->arrays[result->i_array] = array;
    ++result->i_array;

    return array;
}

int visit_array_add(struct json_result *result, struct json_array *array) {
    if (array->i >= MAX_ARRAY_SIZE) {
        return 0;
    }
    array->json_elements[array->i] = result->json_element;
    ++array->i;

    return 1;
}

int visit_array_end(struct json_result *result, struct json_array * array) {
    result->json_element.type = TYPE_ARRAY;
    result->json_element.u.array = array;
}

int visit_string(struct json_result *result, char *start, char *end) {
    char buf[MAX_STRING_SIZE+1];
    char *s = buf;

    if (result->i_string >= MAX_NUM_STRINGS) {
        return 0;
    }

    if (end - start > MAX_STRING_SIZE) {
        return 0;
    }

    while (start < end) {
        *s++ = *start++;
    }
    *s = '\0';
    s = result->strings[result->i_string] = strdup(buf);
    if (!s) return 0;
    result->allocations += 1;
    ++result->i_string;
    result->json_element.type = TYPE_STRING;
    result->json_element.u.s = s;

    return 1;
}

void eat_whitespace(char **s) {
    while (**s && !strchr("\"[],", **s)) {
        ++*s;
    }
}

int _parse_array(char **s, struct json_result *result) {

    int rc;
    struct json_array *array = visit_array_start(result);
    while (1) {
        rc = _parse(s, result);
        if (!rc) return 0;
        rc = visit_array_add(result, array);
        if (!rc) return 0;
        eat_whitespace(s);
        if (**s == ']') {
            ++*s;
            break;
        }
        else if (**s == ',') {
            ++*s;
            eat_whitespace(s);
        }
        else {
            return 0;
        }
    }
    visit_array_end(result, array);
    return 1;
}

int _parse_string(char **s, struct json_result *result) {
    char *start = *s;

    while (**s && **s != '"') {
        ++*s;
    }
    if (**s == '"') {
        int rc = visit_string(result, start, *s);
        if (!rc) return 0;
        ++*s;
        return 1;
    }
    return 0;
}

int _parse(char **s, struct json_result *result) {
    eat_whitespace(s);
    if (**s == '[') {
        ++*s;
        return _parse_array(s, result);
    }
    else if (**s == '"') {
        ++*s;
        return _parse_string(s, result);
    }
    else {
        return 0;
    }
}

int parse(char *s, struct json_result *result) {
    int rc = _parse(&s, result);
    return rc;
}


void init_result(struct json_result *result) {
    result->i_string = 0;
    result->i_array = 0;
    result->allocations = 0;
}

