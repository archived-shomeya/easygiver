<?php
// $Id$

/**
 * @file
 * 
 */

function eg_merchant_views_data() {
  $data = array();

  $data['eg_merchant'] = array(
    'table' => array(
      'title' => t('Merchant data'),
      'group' => t('Merchant'),
      'help' => t('Merchant data.'),
      'join' => array(
        'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
        ),
        'eg_network' => array(
          'left_field' => 'nid',
          'field' => 'network',
        ),
      ),
    ),
    'nid' => array(
      'argument' => array(
        'title' => t('Merchant node'),
        'help' => t('Display merhants associated with a specified network.'),
        'handler' => 'views_handler_argument_node_nid',
        'parent' => 'views_handler_argument_numeric', // make sure parent is included
        'click sortable' => TRUE,
        'numeric' => TRUE,
        'field' => 'network',
      ),
    ),
    'link' => array(
      'field' => array(
        'title' => t('Merchant link'),
        'help' => t('An affiliate link to the merchant, through the network specified in the Merchant network field.'),
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
       ),
    ),
    'network' => array(
      'relationship' => array(
        'title' => t('Network'),
        'help' => t('Provides a relationship from a marchant to the network it is assigned to.'),
        'handler' => 'views_handler_relationship',
        'base' => 'node',
        'base field' => 'nid',
        'label' => t('network'),
      ),
    ),
    'enabled' => array(
      'title' => t('Enabled'),
      'help' => t('Enabled status of the merchant.'),

      'field' => array(
        'handler' => 'views_handler_field_boolean',
        'click sortable' => TRUE,
       ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator',
        'type' => 'yes-no',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'partner_id' => array(
      'title' => t('Partner ID'),
      'help' => t('The partner ID coresponding to the network a assigned to a merchant.'),
      'field' => array(
        'handler' => 'eg_merchant_handler_field_partner_id',
      ),
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function eg_merchant_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'eg_merchant') . '/includes',
    ),
    'handlers' => array(
      'eg_merchant_handler_field_partner_id' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}