<?php
// $Id$

/**
 * @file
 * 
 */

/**
 * Implementation of hook_views_default_views().
 */
function eg_partner_views_default_views() {
  // Search the "default_views" directory for files ending in .view.php.
  $files = file_scan_directory(drupal_get_path('module', 'eg_partner'). '/includes/default_views', 'view.php');
  foreach ($files as $absolute => $file) {
    require_once $absolute;
    if (isset($view)) {
      // $file->name has the ".php" stripped off, but still has the ".view".
      $view_name = substr($file->name, 0, strrpos($file->name, '.'));
      $views[$view_name] = $view;
    }
  }
  return $views;
}