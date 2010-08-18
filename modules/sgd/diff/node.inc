<?php
// $Id: node.inc,v 1.2.2.1.2.1 2008/04/08 14:01:57 weitzman Exp $

/**
 * Implementation of hook_diff() for node.module (body and title).
 */
function node_diff(&$old_node, &$new_node) {
  
  $result = array();
  $type = node_get_types('type', $new_node);
  $result['title'] = array(
    '#name' => $type->title_label,
    '#old' => _node_diff_filter($old_node->title),
    '#new' => _node_diff_filter($new_node->title),
    '#weight' => -5,
    '#format' => array(
      'show_header' => FALSE,
    )
  );
  if ($type->has_body) {
    $result['body'] = array(
      '#name' => $type->body_label,
      '#old' => _node_diff_filter($old_node->body),
      '#new' => _node_diff_filter($new_node->body),
    );
  }
  return $result;
}

function _node_diff_filter($text) {
  $text = preg_replace( '#(</?(?:h\d|p|li|div|th|pre|xmp|td|ul|ol|br|blockquote)>)+#i', "\n", $text );
  $text = preg_replace( '/<.*?>/', ' ',  $text );
  $text = preg_replace( '/  */',   ' ',  $text );
  $text = preg_replace( '/\n\n*/', "\n", $text );
  $text = preg_replace( '/&nbsp;/', ' ', $text );

  return preg_split('/\n/', $text);
}