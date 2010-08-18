<?php
// $Id$

function eg_network_views_data() {
  $data = array();

  $data['eg_network'] = array(
    'table' => array(
      'title' => t('Affiliate networks'),
      'group' => t('Network'),
      'help' => t('Network related data.'),
      'join' => array(
        'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
        ),
      ),
    ),

    'identifier' => array(
      'field' => array(
        'title' => t('Network identifier'),
        'help' => t('An identifier for a network'),
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
       ),
    ),
    'enabled' => array(
      'title' => t('Enabled'),
      'help' => t('Enabled status of the network.'),
      'field' => array(
        'handler' => 'views_handler_field_boolean',
       ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator',
        'label' => t('Published'),
        'type' => 'yes-no',
      )
    ),
  );

  return $data;
}
