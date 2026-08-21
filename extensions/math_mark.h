#ifndef CMARK_MATH_MARK_H
#define CMARK_MATH_MARK_H

#include "cmark-gfm-core-extensions.h"
#include "ext_scanners.h"

#ifdef __cplusplus
extern "C" {
#endif

extern cmark_node_type CMARK_NODE_MATH_INLINE, CMARK_NODE_MATH_BLOCK;

CMARK_GFM_EXPORT
cmark_syntax_extension *create_math_extension(void);

#ifdef __cplusplus
}
#endif

#endif
