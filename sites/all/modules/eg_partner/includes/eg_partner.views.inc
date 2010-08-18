<?php
// $Id$

/**
 * @file
 * 
 */

function eg_partner_views_data() {
  $data = array();

  $data['eg_partner_merchants'] = array(
    'table' => array(
      'title' => t('Partner merchants'),
      'group' => t('Partners'),
      'help' => t('Relate merchants to partners.'),
      'join' => array(
        'node' => array(
          'left_field' => 'nid',
          'field' => 'merchant_id',
        ),
      ),
    ),
    'nid' => array(
      'argument' => array(
        'help' => t('Display merhants associated with a specified partner.'),
        'handler' => 'views_handler_argument_node_nid',
        'parent' => 'views_handler_argument_numeric', // make sure parent is included
        'click sortable' => TRUE,
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    
  );

  return $data;
}
