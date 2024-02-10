#include <assert.h>
#include <string.h>

#include "compiler/compiler.h"
#include "trace.h"
#include "utils/vector.h"

struct vector *g_p_node_vec = NULL;
struct vector *g_p_node_vec_root = NULL;

void node_set_vector(struct vector *p_node_vec, struct vector *p_node_vec_root) {
    g_p_node_vec = p_node_vec;
    g_p_node_vec_root = p_node_vec_root;
}

void node_push(node_t *p_node) {
    assert(g_p_node_vec);
    vector_push(g_p_node_vec, &p_node);
}

node_t *node_peek_or_null() {
    assert(g_p_node_vec);
    return (node_t *)vector_back_ptr_or_null(g_p_node_vec);
}

node_t *node_peek() {
    assert(g_p_node_vec);
    return *(node_t **)vector_back(g_p_node_vec);
}

/**
 * @brief
 *
 * @return node_t*
 */
node_t *node_pop() {
    assert(g_p_node_vec);
    node_t *p_last_node = vector_back_ptr(g_p_node_vec);
    node_t *p_last_node_root
        = vector_empty(g_p_node_vec_root) ? NULL : vector_back_ptr(g_p_node_vec_root);

    vector_pop(g_p_node_vec);

    if (p_last_node_root == p_last_node) {
        vector_pop(g_p_node_vec_root);
    }

    return p_last_node;
}

void debug_print_node(node_t *p_node) {
    FW_LOG_DEBUG("{\n");
    FW_LOG_DEBUG("  .type = %s\n", ENUM_STRING(p_node->type));
    if (p_node->flags) FW_LOG_DEBUG("  .flags = %d\n", p_node->flags);
    debug_print_pos_struct(&p_node->s_pos);
    if (p_node->binded.p_node) FW_LOG_DEBUG("  .binded.p_node = %p\n", p_node->binded.p_node);
    if (p_node->binded.p_function)
        FW_LOG_DEBUG("  .binded.p_function = %p\n", p_node->binded.p_function);
    switch (p_node->type) {
        // case NODE_TYPE_CHAR:
        //     FW_LOG_DEBUG("  .cval = %c\n", p_node->cval);
        //     break;
        case NODE_TYPE_STRING:
            FW_LOG_DEBUG("  .sval = %s\n", p_node->sval);
            break;
        case NODE_TYPE_NUMBER:
            FW_LOG_DEBUG("  .ival = %d\n", p_node->ival);
            break;
            // case NODE_TYPE_LONG:
            //     FW_LOG_DEBUG("  .lval = %ld\n", p_node->lval);
            //     break;
            // case NODE_TYPE_LONG_LONG:
            //     FW_LOG_DEBUG("  .llval = %lld\n", p_node->llval);
            //     break;
    }
}
node_t *node_create(node_t *p_node) {
    node_t *p_new_node = (node_t *)malloc(sizeof(node_t));
    memcpy(p_new_node, p_node, sizeof(node_t));
#warning "This is a shallow copy, if the node has pointers, they will not be copied"
    node_push(p_new_node);

    // Log node for debugging
    FW_LOG_DEBUG("Node created: \n");
    debug_print_node(p_new_node);
    return p_new_node;
}