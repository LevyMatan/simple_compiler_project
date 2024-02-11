#include <stdio.h>
#include <string.h>

#include "utils/generated_enum_dict.h"

char type_val_string[1000];

char* get_trace_level_name(void* p_type) {
    int type = *(int*)p_type;
    if (type < 0 || type >= sizeof(trace_level_dict) / sizeof(trace_level_dict[0])) {
        return "Invalid type";
    }
    return trace_level_dict[type];
}

char* get_parser_status_name(void* p_type) {
    int type = *(int*)p_type;
    if (type < 0 || type >= sizeof(parser_status_dict) / sizeof(parser_status_dict[0])) {
        return "Invalid type";
    }
    return parser_status_dict[type];
}

char* get_LEXICAL_ANALSYS_STATAUS_name(void* p_type) {
    int type = *(int*)p_type;
    if (type < 0
        || type >= sizeof(LEXICAL_ANALSYS_STATAUS_dict) / sizeof(LEXICAL_ANALSYS_STATAUS_dict[0])) {
        return "Invalid type";
    }
    return LEXICAL_ANALSYS_STATAUS_dict[type];
}

char* get_token_type_name(void* p_type) {
    int type = *(int*)p_type;
    if (type < 0 || type >= sizeof(token_type_dict) / sizeof(token_type_dict[0])) {
        return "Invalid type";
    }
    return token_type_dict[type];
}

char* get_number_type_name(void* p_type) {
    int type = *(int*)p_type;
    if (type < 0 || type >= sizeof(number_type_dict) / sizeof(number_type_dict[0])) {
        return "Invalid type";
    }
    return number_type_dict[type];
}

char* get_node_type_name(void* p_type) {
    int type = *(int*)p_type;
    if (type < 0 || type >= sizeof(node_type_dict) / sizeof(node_type_dict[0])) {
        return "Invalid type";
    }
    return node_type_dict[type];
}

char* get_int_name(void* p_type) {
    int val = *(int*)p_type;
    sprintf(type_val_string, "%d\n", val);
    return type_val_string;
}

char* get_float_name(void* p_type) {
    float val = *(float*)p_type;
    sprintf(type_val_string, "%f\n", val);
    return type_val_string;
}

char* get_double_name(void* p_type) {
    double val = *(double*)p_type;
    sprintf(type_val_string, "%f\n", val);
    return type_val_string;
}

char* get_char_name(void* p_type) {
    char val = *(char*)p_type;
    sprintf(type_val_string, "%c\n", val);
    return type_val_string;
}

char* get_short_name(void* p_type) {
    short val = *(short*)p_type;
    sprintf(type_val_string, "%d\n", val);
    return type_val_string;
}

char* get_long_name(void* p_type) {
    long val = *(long*)p_type;
    sprintf(type_val_string, "%ld\n", val);
    return type_val_string;
}

char* get_long_long_name(void* p_type) {
    long long val = *(long long*)p_type;
    sprintf(type_val_string, "%lld\n", val);
    return type_val_string;
}

getter_func_t g_a_getters[] = {
    get_trace_level_name, get_parser_status_name, get_LEXICAL_ANALSYS_STATAUS_name,
    get_token_type_name,  get_number_type_name,   get_node_type_name,
    get_int_name,         get_float_name,         get_double_name,
    get_char_name,        get_short_name,         get_long_name,
    get_long_long_name,
};

int typename_to_idx(const char* type_name) {
    if (strcmp(type_name, "trace_level") == 0) return 0;
    if (strcmp(type_name, "parser_status") == 0) return 1;
    if (strcmp(type_name, "LEXICAL_ANALSYS_STATAUS") == 0) return 2;
    if (strcmp(type_name, "token_type") == 0) return 3;
    if (strcmp(type_name, "number_type") == 0) return 4;
    if (strcmp(type_name, "node_type") == 0) return 5;
    if (strcmp(type_name, "int") == 0) return 6;
    if (strcmp(type_name, "float") == 0) return 7;
    if (strcmp(type_name, "double") == 0) return 8;
    if (strcmp(type_name, "char") == 0) return 9;
    if (strcmp(type_name, "short") == 0) return 10;
    if (strcmp(type_name, "long") == 0) return 11;
    if (strcmp(type_name, "long_long") == 0) return 12;
    return -1;  // Return -1 if type_name is not found
}
