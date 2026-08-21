#include "emoji.h"

#include <parser.h>
#include <render.h>
#include <html.h>

#include "ext_scanners.h"

cmark_node_type CMARK_NODE_EMOJI;

static cmark_node *match(cmark_syntax_extension *self,
                         cmark_parser *parser,
                         cmark_node *parent,
                         unsigned char character,
                         cmark_inline_parser *inline_parser) {
  if (character != ':')
    return NULL;

  int pos = cmark_inline_parser_get_offset(inline_parser);
  struct cmark_chunk *chunk = cmark_inline_parser_get_chunk(inline_parser);
  unsigned char *input = chunk->data + pos;
  int len = chunk->len - pos;
  bufsize_t matched = 0;

  if (len <= 2)
    return NULL;

  if ((matched = scan_emoji(chunk->data, chunk->len, pos))) {
    cmark_node *node =
        cmark_node_new_with_mem_and_ext(CMARK_NODE_EMOJI, parser->mem, self);

    node->start_line = node->end_line = cmark_inline_parser_get_line(inline_parser);
    node->start_column = cmark_inline_parser_get_column(inline_parser);
    node->end_column = node->start_column + matched;

    cmark_strbuf_init(parser->mem, &node->content, matched);
    cmark_strbuf_put(&node->content, input, matched);

    cmark_strbuf buf;
    cmark_strbuf_init(parser->mem, &buf, matched);
    cmark_strbuf_put(&buf, input + 2, matched - 4);

    node->as.code.fenced = true;
    node->as.code.fence_char = ':';
    node->as.code.fence_length = (matched > 255) ? 255 : matched;
    node->as.code.fence_offset = parser->first_nonspace - parser->offset;
    node->as.code.info = cmark_chunk_literal("emoji");
    node->as.code.literal = cmark_chunk_buf_detach(&buf);

    cmark_inline_parser_set_offset(inline_parser, pos + matched);
    return node;
  }

  return NULL;
}

static const char *get_type_string(cmark_syntax_extension *extension,
                                   cmark_node *node) {
  return node->type == CMARK_NODE_EMOJI ? "emoji" : "<unknown>";
}

static int can_contain(cmark_syntax_extension *extension, cmark_node *node,
                       cmark_node_type child_type) {
  return false;
}

static void commonmark_render(cmark_syntax_extension *extension,
                              cmark_renderer *renderer, cmark_node *node,
                              cmark_event_type ev_type, int options) {
  renderer->out(renderer, node, ":", false, LITERAL);
}

static void latex_render(cmark_syntax_extension *extension,
                         cmark_renderer *renderer, cmark_node *node,
                         cmark_event_type ev_type, int options) {
  renderer->out(renderer, node, "", false, LITERAL);
}

static void html_render(cmark_syntax_extension *extension,
                        cmark_html_renderer *renderer, cmark_node *node,
                        cmark_event_type ev_type, int options) {
  if (ev_type == CMARK_EVENT_ENTER) {
    cmark_strbuf_puts(renderer->html, "<emoji");
    cmark_html_render_sourcepos(node, renderer->html, options);
    cmark_strbuf_putc(renderer->html, '>');
  } else {
    cmark_strbuf_puts(renderer->html, "</emoji>");
  }
}

cmark_syntax_extension *create_emoji_extension(void) {
  cmark_syntax_extension *ext = cmark_syntax_extension_new("emoji");

  cmark_syntax_extension_set_get_type_string_func(ext, get_type_string);
  cmark_syntax_extension_set_can_contain_func(ext, can_contain);
  cmark_syntax_extension_set_commonmark_render_func(ext, commonmark_render);
  cmark_syntax_extension_set_latex_render_func(ext, latex_render);
  cmark_syntax_extension_set_html_render_func(ext, html_render);
  cmark_syntax_extension_set_plaintext_render_func(ext, commonmark_render);

  CMARK_NODE_EMOJI = cmark_syntax_extension_add_node(0);
  cmark_syntax_extension_set_match_inline_func(ext, match);

  cmark_llist *special_chars = NULL;
  cmark_mem *mem = cmark_get_default_mem_allocator();
  special_chars = cmark_llist_append(mem, special_chars, (void *)':');
  cmark_syntax_extension_set_special_inline_chars(ext, special_chars);
  cmark_syntax_extension_set_emphasis(ext, 0);

  return ext;
}
