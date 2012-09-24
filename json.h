#define JSON_MAX_STRING_SIZE 128
#define JSON_MAX_NUM_STRINGS 50
#define JSON_MAX_NUM_ARRAYS 50
#define JSON_MAX_ARRAY_SIZE 15

#define JSON_TYPE_STRING 0
#define JSON_TYPE_ARRAY 1

struct json_element {
    int type;
    union {
        char *s;
        struct json_array *array;
    } u;
};

struct json_array {
    struct json_element *json_elements;
    int i;
};


struct json_result {
    int num_tokens;
    int allocations;
    char *strings[JSON_MAX_NUM_STRINGS+1];
    int i_string;
    struct json_array *arrays[JSON_MAX_NUM_ARRAYS+1];
    int i_array;
    struct json_element json_element;
};

void json_init_result(struct json_result *result);
void json_release_result(struct json_result *result);
int json_parse(char *s, struct json_result *result);
