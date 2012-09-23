#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "json.h"

void debug_array(struct array *array);

void debug_element(struct element element) {
  if (element.type == TYPE_STRING) {
    printf("YO: %s\n", element.u.s);
  }
  else {
    debug_array(element.u.array);
  }
}

void debug_array(struct array *array) {
  int i;

  printf("starting to show array\n");
  for (i = 0; i < array->i; ++i) {
    debug_element(array->elements[i]);
  }
  printf("done\n");
}

void debug(struct result *result) {
  int i;

  for (i = 0; i < result->i_string; ++i) {
    printf("%s\n", result->strings[i]);
  }
  for (i = 0; i < result->i_array; ++i) {
    printf("\narray %d\n", i);
    debug_array(result->arrays[i]);
    printf("\n");
  }
}

int main(int argc, char **argv) {
  struct result result;

  init_result(&result);
  int rc = parse("   [\"hello\", [\"a\", \"b\"],  \"you\"]\n", &result);
  printf("rc = %d\n", rc);
  debug(&result);
  printf("allocations = %d\n", result.allocations);
  release_result(&result);
  printf("after releasing, net allocations = %d\n", result.allocations);
}

