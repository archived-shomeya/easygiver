<?php
// $Id: packgr.profile,v 1.1 2008/10/12 02:32:40 mikeyp Exp $

/**
 * Return an array of the modules to be enabled when this profile is installed.
 *
 * @return
 *   An array of modules to enable.
 */
function packgr_profile_modules() {
  return array();
}

/**
 * Return a description of the profile for the initial installation screen.
 *
 * @return
 *   An array with keys 'name' and 'description' describing this profile,
 *   and optional 'language' to override the language selection for
 *   language-specific profiles.
 */
function packgr_profile_details() {
  return array(
  'name' => 'packgr',
  'description' => 'Select this profile to choose packages and enable advanced functionality.'
  );
}

/**
 * Return a list of tasks that this profile supports.
 *
 * @return
 *   A keyed array of tasks the profile will perform during
 *   the final stage. The keys of the array will be used internally,
 *   while the values will be displayed to the user in the installer
 *   task list.
 */
function packgr_profile_task_list() {
  $tasks = array();
  $tasks['select_packages'] = st('Select packages');
  if (variable_get('packgr_selected_packages', FALSE)) {
    $package_tasks = packgr_format_task_list();
    $tasks += $package_tasks;
  }
  return $tasks;
}

/**
 * Build a task list formatted for hook_profile_task_list
 *
 * @param  An array of packages to fetch tasks from
 * @return An array of tasks in the form of taskname => title
 */
function packgr_format_task_list() {
  $tasks = array();
  $task_list = packgr_build_task_list();
  if ($task_list) {
    foreach ($task_list as $value) {
      $tasks[$value['taskname']] = $value['title'];
    }
  }
  
  return $tasks;
}

/**
 * Build the complete list of tasks for the submit handler, and task advancer
 *
 * @param an array  $packages
 * @return unknown
 */
function packgr_build_task_list() {
  if (($packages = variable_get('packgr_selected_packages', '')) == 'NONE') {
    return;
  }
  
  
  $task_list = variable_get('packgr_task_list', FALSE);
  if (!$task_list) {
    
  
    $task_list = array();
    foreach ($packages as $key => $value) {
      $result = packgr_load_package_hook('tasks', $key);
      if (isset($result) && is_array($result)) {
        $task_list = array_merge($task_list, $result);
      }
    }
    variable_set('packgr_task_list', $task_list);
  }
  

  return $task_list;
}

/**
 * Perform any final installation tasks for this profile.
 *
 * The installer goes through the profile-select -> locale-select
 * -> requirements -> database -> profile-install-batch
 * -> locale-initial-batch -> configure -> locale-remaining-batch
 * -> finished -> done tasks, in this order, if you don't implement
 * this function in your profile.
 *
 * If this function is implemented, you can have any number of
 * custom tasks to perform after 'configure', implementing a state
 * machine here to walk the user through those tasks. First time,
 * this function gets called with $task set to 'profile', and you
 * can advance to further tasks by setting $task to your tasks'
 * identifiers, used as array keys in the hook_profile_task_list()
 * above. You must avoid the reserved tasks listed in
 * install_reserved_tasks(). If you implement your custom tasks,
 * this function will get called in every HTTP request (for form
 * processing, printing your information screens and so on) until
 * you advance to the 'profile-finished' task, with which you
 * hand control back to the installer. Each custom page you
 * return needs to provide a way to continue, such as a form
 * submission or a link. You should also set custom page titles.
 *
 * You should define the list of custom tasks you implement by
 * returning an array of them in hook_profile_task_list(), as these
 * show up in the list of tasks on the installer user interface.
 *
 * Remember that the user will be able to reload the pages multiple
 * times, so you might want to use variable_set() and variable_get()
 * to remember your data and control further processing, if $task
 * is insufficient. Should a profile want to display a form here,
 * it can; the form should set '#redirect' to FALSE, and rely on
 * an action in the submit handler, such as variable_set(), to
 * detect submission and proceed to further tasks. See the configuration
 * form handling code in install_tasks() for an example.
 *
 * Important: Any temporary variables should be removed using
 * variable_del() before advancing to the 'profile-finished' phase.
 *
 * @param $task
 *   The current $task of the install system. When hook_profile_tasks()
 *   is first called, this is 'profile'.
 * @param $url
 *   Complete URL to be used for a link or form action on a custom page,
 *   if providing any, to allow the user to proceed with the installation.
 *
 * @return
 *   An optional HTML string to display to the user. Only used if you
 *   modify the $task, otherwise discarded.
 */
function packgr_profile_tasks(&$task, $url) {

  $selected_packages = variable_get('packgr_selected_packages', '');
  
  if ($selected_packages == 'NONE') {
    packgr_advance_task($task);
    return;
  }
  
  global $redirect_url;
  $redirect_url = $url;

  // includes all packages files
  // @TODO Replace this with a better include method, with only needed packages
  packgr_load_callback('info');

  // Check to see if this is our first time and return build first task manually
  if ($task == 'profile') {
    packgr_advance_task($task); //will set task for us
    drupal_set_title(st('Select packages'));
    return drupal_get_form('packgr_select_packages_form', $url);
  }
  
  // Second time called
  if ($task == 'select_packages') {
    if (!$selected_packages) {
      drupal_set_title(st('Select packages'));
      return drupal_get_form('packgr_select_packages_form', $url);
    }
    packgr_advance_task($task);
    
  }
  
  // Get the current task
  if (!isset($current_task)) {
    $current_task = variable_get('packgr_current_task', '');
  }
  
  
  // if the current task has been marked completed, get next task
  if ($current_task['completed'] == TRUE) {
    $current_task = packgr_advance_task($task);
  }
  
  // make sure the task we are calling matches the installer task 
  if ($task == $current_task['taskname']) {
    return packgr_do_task($current_task);
  }
}

/**
 * Implementation of hook_form_alter().
 *
 * Allows the profile to alter the site-configuration form. This is
 * called through custom invocation, so $form_state is not populated.
 */
function packgr_form_alter(&$form, $form_state, $form_id) {
  switch ($form_id) {
    case 'install_configure' :
      // Set default for site name field.
      $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
      break;
    case 'packgr_select_packages' :
      // set the default to true since its recommended
      $form['packages']['default']['#default_value'] = TRUE;
      break;
  }
}

/**
 * Load a callback from all available packages, such as info hook. 
 *
 * @param name of the callback to load, in the form of package_PACKAGENAME_CALLBACK
 * @return unknown
 */
function packgr_load_callback($callback) {
  $output = array();
  $path = drupal_get_path('profile', 'packgr') . '/packages';
  $files = drupal_system_listing('.inc$', $path, 'name', 0);

  foreach($files as $file) {
    require_once('./' . $file->filename);
  }
  foreach ($files as $file) {
    $function = 'package_' . $file->name . '_' . $callback;
    if (function_exists($function)) {
      $result = $function();
      if (isset($result) && is_array($result)) {
        $output = array_merge($output, $result);
      }
    }
  }
  return $output;
}

/**
 * Wrapper function for sorting packes based on their specified weight in package_hook_
 *
 * @return unknown
 */
function packgr_load_package_info() {
  $package = packgr_load_callback('info');
  // Sort the packages based on their specified weight
  uasort($package, 'packgr_package_sort');
  return $package;
}



function packgr_package_sort ($a, $b) {
  if (!isset($a['weight'])) {
    $a['weight'] = 0;
  }
  if (!isset($b['weight'])) {
    $b['weight'] = 0;
  }
  if ($a['weight'] == $b['weight']) {
    return 0;
  }
  return ($a['weight'] < $b['weight']) ? -1 : 1;
}

/**
 * A version of module_invoke for packages
 *
 * @param unknown_type the hook to invoke
 * @param unknown_type the name of the package
 * @return unknown
 */
function packgr_load_package_hook($hook, $package) {
  $items = array();
  $function = 'package_'. $package .'_'. $hook;
  if (function_exists($function)) {
    $result = $function();
  }
  if (isset($result) && is_array($result)) {
    $items = array_merge($items, $result);
  }
  return $items;
}

/**
 * Form API array for the package selection form.
 *
 * @param unknown_type $form_state
 * @param unknown_type $url
 * @return The package selection form. 
 */
function packgr_select_packages_form(&$form_state, $url) {
  $packages = packgr_load_package_info();
  $form['intro'] = array(
  '#value' => st('Please select packages you would like to install. Packages makred with a "*" require additional configuration.'),
  '#weight' => -10,
  );

  foreach ($packages as $key => $value) {
    $description = $value['description'];
    if ($value['modules']) {
      $description .= '<br />This package uses the following modules: '. implode(', ', $value['modules']);
    }
    $name = $value['name'];
    if ($value['configure']) {
      $name .= ' *';
    }
    $form['packages'][$key] = array(
    '#type' => 'checkbox',
    '#title' => $name,
    '#description' => $description,
    );
  }
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
    $hook_form_alter($form, array(), 'packgr_select_packages');
  }
  return $form;
}

/**
 * Form API submit for the package selection form..
 */
function packgr_select_packages_form_submit($form, &$form_state) {
  global $redirect_url;

  $info = packgr_load_package_info();
  foreach ($info as $key => $value) {
    if ($form_state['values'][$key] == 1) {
      $packages[$key] = 1;
    }
  }

  // Stupid error handling for forms processing last
  if ($packages == '') {
    $packages = 'NONE';
    variable_set('packgr_selected_packages', $packages);
    drupal_goto($redirect_url);
  }
  
  // set the selected packages, since hook_profile_tasks already executed
  variable_set('packgr_selected_packages', $packages);
  $task_list = packgr_build_task_list($packages);
  variable_set('packgr_remaining_tasks', $task_list);
  drupal_goto($redirect_url);
}


/**
 * Advance the Task to the next available task
 * 
 * We pass $task by reference, so that it will be returned to packgr_profile_tasks, and back 
 * to install.php, as a simple task, to keep track of our position in the task list.
 * The current task is returned in order to provide all the task parameters to the calling function.
 *
 * @param unknown_type $task
 */
function packgr_advance_task(&$task) {
  // set a fallback task for fetching the variable_get

  $tasks = variable_get('packgr_remaining_tasks', '');
  $current_task = variable_get('packgr_current_task', '');
  
  
  if ($task == 'profile') {
    $task = 'select_packages';
    return;
  }
  else {
    // Are we there yet?
    if (!$tasks) {
      // @TODO should provide finalize step here
      $task = 'profile-finished';
      packgr_cleanup();
      return;
    }
    $current_task = array_shift($tasks);
    variable_set('packgr_remaining_tasks', $tasks);
    variable_set('packgr_current_task', $current_task);
    $task = $current_task['taskname'];
  }
  
  variable_set('install_task', $task);
  return $current_task;
}


/**
 * Mark the current task as completed
 * 
 * Usually called from within a task callback, or a form submit handler
 *
 */
function packgr_mark_task_completed() {
  $current_task = variable_get('packgr_current_task', array());
  $current_task['completed'] = TRUE;
  variable_set('packgr_current_task', $current_task);
}


/**
 * Execute the actual task, depending on what it is
 *
 * @param unknown_type A single task array
 * @return A form, corresponding to the task
 */
function packgr_do_task($task) {
  global $redirect_url;
  switch ($task['type']) {
    case 'configure' :
      drupal_set_title($task['title']);
      return drupal_get_form($task['callback'], $redirect_url);
      break;
    case 'enable' :
      drupal_set_title($task['title']);
      call_user_func($task['callback']);
      drupal_goto($redirect_url);
      break;
    case 'settings' :
      drupal_set_title($task['title']);
      return drupal_get_form($task['callback']);
      break;
  }
}

function packgr_cleanup() {
  variable_del('packgr_remaining_tasks');
  variable_del('packgr_current_task');
  variable_del('packgr_task_list');
}