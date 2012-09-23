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
    struct array *array;
  } u;
};

struct array {
  struct element *elements;
  int i;
};


struct result {
  int num_tokens;
  int allocations;
  char *strings[MAX_NUM_STRINGS+1];
  int i_string;
  struct array *arrays[MAX_NUM_ARRAYS+1];
  int i_array;
  struct element element;
};

void init_result(struct result *result);
void release_result(struct result *result);
int parse(char *s, struct result *result);
