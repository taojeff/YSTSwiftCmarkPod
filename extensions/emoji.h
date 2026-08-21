#ifndef CMARK_EMOJI_H
#define CMARK_EMOJI_H

#include "cmark-gfm-core-extensions.h"

#ifdef __cplusplus
extern "C" {
#endif

extern cmark_node_type CMARK_NODE_EMOJI;

CMARK_GFM_EXPORT
cmark_syntax_extension *create_emoji_extension(void);

#ifdef __cplusplus
}
#endif

#endif
