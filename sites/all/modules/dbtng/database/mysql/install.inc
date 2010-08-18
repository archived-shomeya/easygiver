<?php
// $Id: install.inc,v 1.1.2.1 2009/08/12 01:15:42 mikeyp Exp $

/**
 * @file
 * Installation code for MySQL embedded database engine.
 */


// MySQL specific install functions

class DatabaseTasks_mysql extends DatabaseTasks {
  protected $pdoDriver = 'mysql';
  public function name() {
    return 'MySQL';
  }
}

