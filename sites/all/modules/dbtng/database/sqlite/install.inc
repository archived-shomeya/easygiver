<?php
// $Id: install.inc,v 1.1.2.1 2009/08/12 01:15:42 mikeyp Exp $

/**
 * @file
 * SQLite specific install functions
 */

class DatabaseTasks_sqlite extends DatabaseTasks {
  protected $pdoDriver = 'sqlite';
  public function name() {
    return 'SQLite';
  }
}

