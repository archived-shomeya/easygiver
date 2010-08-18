<?php

function package_default_info() {
  $items['default'] = array(
    'profile' => 'packgr',
    'name' => st('Default'),
    'description' => st('Provides the same functionality included with the default Drupal install profile. %recommended', array('%recommended' => 'Recommended.')),
    'configure' => TRUE,
    'modules' => array('color', 'comment', 'help', 'menu', 'taxonomy', 'dblog'),
    'weight' => -1000,    
  );
  return $items;
}

function package_default_tasks() {
  $items[] = array(
    'taskname' => 'default_module_enable',
    'type' => 'enable',
    'callback' => 'package_default_module_enable',
    'title' => 'Install default modules',
  );
  $items[] = array(
    'taskname' => 'default_configure_form',
    'type' => 'configure',
    'callback' => 'package_default_configure_form',
    'title' => 'Select default content types',
  );
  $items[] = array(
    'taskname' => 'default_enable',
    'type' => 'enable',
    'callback' => 'package_default_enable',
    'title' => 'Install default package',
  );
  return $items;
}

function package_default_enable() {
  // Insert default user-defined node types into the database. For a complete
  // list of available node type attributes, refer to the node type API
  // documentation at: http://api.drupal.org/api/HEAD/function/hook_node_info.
  $types = array();
  
  if (variable_get('package_default_page_enable', FALSE)) {
    $types[] = array(
      'type' => 'page',
      'name' => st('Page'),
      'module' => 'node',
      'description' => st("A <em>page</em>, similar in form to a <em>story</em>, is a simple method for creating and displaying information that rarely changes, such as an \"About us\" section of a website. By default, a <em>page</em> entry does not allow visitor comments and is not featured on the site's initial home page."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    );
  }
  
  if (variable_get('package_default_story_enable', FALSE)) {
    $types[] = array(
      'type' => 'story',
      'name' => st('Story'),
      'module' => 'node',
      'description' => st("A <em>story</em>, similar in form to a <em>page</em>, is ideal for creating and displaying content that informs or engages website visitors. Press releases, site announcements, and informal blog-like entries may all be created with a <em>story</em> entry. By default, a <em>story</em> entry is automatically featured on the site's initial home page, and provides the ability to post comments."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    );
  }

  foreach ($types as $type) {
    $type = (object) _node_type_set_defaults($type);
    node_type_save($type);
  }

  // Default page to not be promoted and have comments disabled.
  variable_set('node_options_page', array('status'));
  variable_set('comment_page', COMMENT_NODE_DISABLED);

  // Don't display date and author information for page nodes by default.
  $theme_settings = variable_get('theme_settings', array());
  $theme_settings['toggle_node_info_page'] = FALSE;
  variable_set('theme_settings', $theme_settings);

  // Update the menu router information.
  menu_rebuild();
  
  // Clean up our variables
  variable_del('package_default_story_enable');
  variable_del('package_default_page_enable');
  packgr_mark_task_completed();
}

function package_default_module_enable() {
  $info = packgr_load_package_hook('info', 'default');
  drupal_install_modules($info['default']['modules']);
  packgr_mark_task_completed();
}

function package_default_configure_form(&$form_state, $url) {
  $form['intro'] = array(
    '#value' => st('Please select the content types to enable for this site.'),
    '#weight' => -10,
  );
  
  $form['types']['page'] = array(
    '#type' => 'checkbox',
    '#title' => st('Page'),
    '#description' => st("A <em>page</em>, similar in form to a <em>story</em>, is a simple method for creating and displaying information that rarely changes, such as an \"About us\" section of a website. By default, a <em>page</em> entry does not allow visitor comments and is not featured on the site's initial home page."),
  );
  $form['types']['story'] = array(
    '#type' => 'checkbox',
    '#title' => st('Story'),
    '#description' => st("A <em>story</em>, similar in form to a <em>page</em>, is ideal for creating and displaying content that informs or engages website visitors. Press releases, site announcements, and informal blog-like entries may all be created with a <em>story</em> entry. By default, a <em>story</em> entry is automatically featured on the site's initial home page, and provides the ability to post comments."),
  );
  

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => st('Save and continue'),
    '#weight' => 15,
  );
  $form['#action'] = $url;
  $form['#redirect'] = FALSE;

  // Allow the profile to alter this form. $form_state isn't available
  // here, but to conform to the hook_form_alter() signature, we pass
  // an empty array.
  $hook_form_alter = $_GET['profile'] .'_form_alter';
  if (function_exists($hook_form_alter)) {
    $hook_form_alter($form, array(), 'package_default_configure');
  }
  return $form;
}

function package_default_configure_form_submit($form, &$form_state) {
  
  
  if ($form_state['values']['page'] == 1) {
    variable_set('package_default_page_enable', TRUE);
  }
    if ($form_state['values']['story'] == 1) {
    variable_set('package_default_story_enable', TRUE);
  }
  
  global $redirect_url;
  packgr_mark_task_completed();
  drupal_goto($redirect_url);
}