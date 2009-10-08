<?php
// $Id$
/**
 * @file
 * Definition of DataHandler class.
 */

/**
 * Simple access methods to table data. Can be used on any table, not just Data
 * managed tables.
 */
class DataHandler {

  // Holds the name of the table that this handler is responsible for.
  public $table;

  /**
   * Constructor, call indirectly through DataHandler::instance();
   */
  protected function __construct($table) {
    $this->table = $table;
  }

  /**
   * Instantiate a DataHandler object.
   *
   * @param $table
   *   The name of the table to access with this DataHandler object.
   */
  public static function instance($table) {
    static $handlers;
    if (!isset($handlers[$table])) {
      $handlers[$table] = new DataHandler($table);
    }
    return $handlers[$table];
  }

  /**
   * Load a record.
   */
  public function load($keys) {
    $where = $values = array();
    $schema = drupal_get_schema($this->table);
    $fields = $schema['fields'];
    foreach ($keys as $key => $value) {
      // Return if a key does not exist.
      if (!isset($fields[$key]['type'])) {
        return FALSE;
      }
      $where[] = db_escape_string($key) ." = ". db_type_placeholder($fields[$key]['type']);
      $values[] = $value;
    }

    if (!empty($where)) {
      $result = db_query('SELECT * FROM {'. db_escape_table($this->table) .'} WHERE '. implode(' AND ', $where), $values);
      $results = array();
      while ($row = db_fetch_array($result)) {
        $results[] = $row;
      }
      return count($results) ? $results : FALSE;
    }
    return FALSE;
  }

  /**
   * Save one or more records to the table.
   *
   * If $update is given, DataHandler will try to load an item for the update
   * and update it if present, otherwise it will insert data.
   *
   * @todo: revisit insert-if-update, this doesn't scale and has some data
   * integrity problems.
   */
  public function save(&$record, $update = array()) {
    if (count($update)) {
      $keys = array();
      foreach ($update as $key) {
        $keys[$key] = $record[$key];
      }
      // Update if available.
      if ($this->load($keys)) {
        return drupal_write_record($this->table, $record, $update);
      }
    }

    // Otherwise insert.
    return drupal_write_record($this->table, $record);
  }

  /**
   * Delete one or more records from the table.
   */
  public function delete($keys) {
    $where = $values = array();
    $schema = drupal_get_schema($this->table);
    $fields = $schema['fields'];
    foreach ($keys as $key => $value) {
      $where[] = db_escape_string($key) ." = ". db_type_placeholder($fields[$key]['type']);
      $values[] = $value;
    }
    if (!empty($where)) {
      db_query('DELETE FROM {'. db_escape_table($this->table) .'} WHERE '. implode(' AND ', $where), $values);
    }
  }

  /**
   * Empty data table.
   */
  public function truncate() {
    db_query('TRUNCATE TABLE {'. db_escape_table($this->table) .'}');
  }
}
