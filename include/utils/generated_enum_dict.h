#define typename(x)                                              \
    _Generic((x),                                                \
        int: "int",                                              \
        float: "float",                                          \
        double: "double",                                        \
        char: "char",                                            \
        short: "short",                                          \
        long: "long",                                            \
        long long: "long long",                                  \
        enum trace_level: "trace_level",                         \
        enum parser_status: "parser_status",                     \
        enum LEXICAL_ANALSYS_STATAUS: "LEXICAL_ANALSYS_STATAUS", \
        enum token_type: "token_type",                           \
        enum number_type: "number_type",                         \
        enum node_type: "node_type",                             \
        default: "unknown")
typedef char* (*getter_func_t)(void*);
extern char type_val_string[1000];

extern char* get_trace_level_name(void* p_type);
extern char* trace_level_dict[4];
extern char* get_parser_status_name(void* p_type);
extern char* parser_status_dict[2];
extern char* get_LEXICAL_ANALSYS_STATAUS_name(void* p_type);
extern char* LEXICAL_ANALSYS_STATAUS_dict[2];
extern char* get_token_type_name(void* p_type);
extern char* token_type_dict[9];
extern char* get_number_type_name(void* p_type);
extern char* number_type_dict[4];
extern char* get_node_type_name(void* p_type);
extern char* node_type_dict[37];
extern getter_func_t g_a_getters[];
int typename_to_idx(const char* type_name);
