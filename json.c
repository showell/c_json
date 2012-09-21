#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING_SIZE 128
#define MAX_NUM_STRINGS 50
#define MAX_NUM_ARRAYS 50
#define MAX_ARRAY_SIZE 15

#define TYPE_STRING 0
#define TYPE_ARRAY 1

struct element {
  int type;
  union {
    char *s;
  } u;
};

struct array {
  struct element elements[MAX_ARRAY_SIZE];
  int i;
};


struct result {
  int num_tokens;
  char *strings[MAX_NUM_STRINGS+1];
  int i_string;
  struct array *arrays[MAX_NUM_ARRAYS+1];
  int i_array;
  struct element element;
};

void debug(struct result *result) {
  int i;

  for (i = 0; i < result->i_string; ++i) {
    printf("%s\n", result->strings[i]);
  }
  for (i = 0; i < result->i_array; ++i) {
    printf("YO: %s\n", result->arrays[i]->elements[0].u.s);
  }
}

void release_result(struct result *result) {
  int i;

  for (i = 0; i < result->i_string; ++i) {
    free(result->strings[i]);
  }
  for (i = 0; i < result->i_array; ++i) {
    free(result->arrays[i]);
  }
}

struct array *visit_array_start(struct result *result) {
  struct array *array;
  if (result->i_array >= MAX_NUM_ARRAYS) {
    return NULL;
  }
  array = malloc(sizeof(struct array));
  array->i = 0;
  result->arrays[result->i_array] = array;
  ++result->i_array;

  return array;
}

int visit_array_add(struct result *result, struct array *array) {
  if (array->i >= MAX_ARRAY_SIZE) {
    return 0;
  }
  array->elements[array->i] = result->element;
  ++array->i;

  printf("visit_array_add %s\n", result->element.u.s);
  return 1;
}

int visit_array_end(struct result *result) {
  printf("visit_array_end\n");
}

int visit_string(struct result *result, char *start, char *end) {
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
  ++result->i_string;
  result->element.type = TYPE_STRING;
  result->element.u.s = s;

  printf("string: %s\n", buf);

  return 1;
}

void eat_whitespace(char **s) {
  while (**s && !strchr("\"[],", **s)) {
    ++*s;
  }
}

int _parse_array(char **s, struct result *result) {

  int rc;
  struct array *array = visit_array_start(result);
  while (1) {
    rc = _parse(s, result);
    if (!rc) return rc;
    visit_array_add(result, array);
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
  visit_array_end(result);
  return 1;
}

int _parse_string(char **s, struct result *result) {
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

int _parse(char **s, struct result *result) {
  eat_whitespace(s);
  if (**s == '[') {
    printf("array\n");
    ++*s;
    return _parse_array(s, result);
  }
  else if (**s == '"') {
    ++*s;
    _parse_string(s, result);
    return 1;
  }
  else {
    return 0;
  }
}

int parse(char *s, struct result *result) {
  int rc = _parse(&s, result);
  printf("%s", s);
  return rc;
}


void init_result(struct result *result) {
  result->i_string = 0;
  result->i_array = 0;
}

int main(int argc, char **argv) {
  struct result result;

  init_result(&result);
  int rc = parse("   [\"hello\", \"you\"]\n", &result);
  printf("rc = %d\n", rc);
  debug(&result);
  release_result(&result);
}
