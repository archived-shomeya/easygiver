<?php
// $Id$

/**
 * @file
 *
 */


class eg_merchant_handler_field_partner_id extends views_handler_field {
  var $field_alias = 'unknown';
  var $aliases = array();


  function query() {
    $this->ensure_my_table();

    $join = new views_join();
    $join->construct('eg_partner_identifier', $this->table, 'network', 'network_id');
    $join->extra = array(array('field' => 'nid', 'operator' => '=', 'value' => $this->view->argument['nid']->get_value()));
    $this->identifier_table = $this->query->ensure_table('eg_partner_identifier', $this->relationship, $join);
    $this->field_alias = $this->query->add_field($this->identifier_table, 'id');
  }
}
