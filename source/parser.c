#include "compiler/compiler.h"
#include "trace.h"
#include "utils/vector.h"

static compile_process_t *g_p_process = NULL;
static token_t *g_p_parser_last_token = NULL;

STATIC_FUNCTION void parser_ignore_nl_or_comment(token_t *p_token) {
    FW_LOG_ENTERED_FUNCTION();
    while (p_token && token_is_nl_or_comment_or_nl_seperator(p_token)) {
        // Skip the token
        vector_peek(g_p_process->p_s_token_vec);
        p_token = vector_peek_no_increment(g_p_process->p_s_token_vec);
    }
}

STATIC_FUNCTION token_t *token_next() {
    FW_LOG_ENTERED_FUNCTION();
    token_t *p_next_token = (token_t *)vector_peek_no_increment(g_p_process->p_s_token_vec);
    parser_ignore_nl_or_comment(p_next_token);
    g_p_process->s_pos = p_next_token->s_pos;
    g_p_parser_last_token = p_next_token;
    return vector_peek(g_p_process->p_s_token_vec);
}

STATIC_FUNCTION token_t *token_peek_next() {
    FW_LOG_ENTERED_FUNCTION();
    token_t *p_next_token = (token_t *)vector_peek_no_increment(g_p_process->p_s_token_vec);
    parser_ignore_nl_or_comment(p_next_token);
    return vector_peek_no_increment(g_p_process->p_s_token_vec);
}

void parse_single_token_to_node() {
    FW_LOG_ENTERED_FUNCTION();
    token_t *p_token = token_next();
    node_t *p_node = NULL;
    if (!p_token) {
        FW_LOG_ERROR("No token to parse\n");
        return;
    }

    node_t node = {0};
    switch (p_token->type) {
        case TOKEN_TYPE_NUMBER:
            node.type = NODE_TYPE_NUMBER;
            node.llval = p_token->llval;
            break;
        case TOKEN_TYPE_IDENTIFIER:
            node.type = NODE_TYPE_IDENTIFIER;
            node.sval = p_token->sval;
            break;
        case TOKEN_TYPE_STRING:
            node.type = NODE_TYPE_STRING;
            node.sval = p_token->sval;
            break;
        default:
            FW_LOG_ERROR("Token type not supported\n");
            compiler_error(g_p_process,
                           "This is not a single token that can be parsed into a node\n");
            return;
    }
    p_node = node_create(&node);
}
int parse_next() {
    FW_LOG_ENTERED_FUNCTION();
    token_t *p_token = token_peek_next();
    if (!p_token) {
        return -1;
    }

    int res = 0;
    switch (p_token->type) {
        case TOKEN_TYPE_NUMBER:
        case TOKEN_TYPE_IDENTIFIER:
        case TOKEN_TYPE_STRING:
            parse_single_token_to_node();
        default:
            break;
    }

    return 0;
}

int parse(compile_process_t *p_process) {
    FW_LOG_ENTERED_FUNCTION();

    // Assign the global process pointer - allows easier coding but it is not the correct way to do
    // stuff.
    g_p_process = p_process;
    g_p_parser_last_token = NULL;

    node_set_vector(p_process->p_node_vec, p_process->p_node_tree_vec);

    node_t *p_node = NULL;
    vector_set_peek_pointer(p_process->p_node_vec, 0);

    while (parse_next() == 0) {
        FW_LOG_DEBUG("Parsing next node\n");
        p_node = node_peek();
        vector_push(p_process->p_node_tree_vec, &p_node);
    }
    return PARSE_ALL_OK;
}