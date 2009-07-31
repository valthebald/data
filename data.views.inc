<?php
// $Id$
/**
 * @file
 * Views hooks and utility functions.
 */

/**
 * Implementation of hook_views_data().
 * 
 * Dynamically create views integration for any table Data manages.
 */
function data_views_data() {
  $data = array();

  $tables = data_get_all_tables();

  foreach ($tables as $table) {

    // Get schema and check wether there are field definitions.
    $schema = $table->get('table_schema');
    $meta = $table->get('meta');
    if (!isset($schema['fields'])) {
      continue;
    }

    $table_data = array();
    $table_data['table'] = array(
      'group' => $table->get('title'),
      );
    foreach ($schema['fields'] as $field_name => $field) {
      // If there is no label, generate one from field name.
      $title = empty($meta['fields'][$field_name]['label']) ? data_natural_name($field_name) : $meta['fields'][$field_name]['label'];

      $table_data[$field_name] = array(
        'title' => $title,
        'help' => $title,
        'field' => array(
          'handler' => data_get_views_field_handler($table, $field_name),
          'help' => $title,
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => data_get_views_filter_handler($table, $field_name),
          'allow empty' => TRUE,
          'help' => t('Filter on %field', array('%field' => $title)),
        ),
        'argument' => array(
          'handler' => data_get_views_field_argument($table, $field_name),
          'help' => $title,
        ),
        'sort' => array(
          'handler' => data_get_views_field_sort($table, $field_name),
          'help' => t('Sort by %field', array('%field' => $title)),
        ),
      );
    }

    // Tables with a primary key are base tables.
    if (isset($schema['primary key']) && count($schema['primary key']) >= 1) {
      $table_data['table']['base'] = array(
        'field' => current($schema['primary key']),
        'title' => $table->get('title'),
        'help' => t('Data table'),
        'weight' => 10, 
      );
    }

    // Add join information.
    if (isset($meta['join']) && is_array($meta['join'])) {
      $table_data['table']['join'] = array();
      foreach ($meta['join'] as $left_table => $join) {
        // @todo: See if left table has other tables it is linked to and link
        // all the way to the leftmost table.
        $table_data['table']['join'][$left_table] = array(
          'left_field' => $join['left_field'],
          'field' => $join['field'],
          'type' => $join['inner_join'] ? 'INNER' : 'LEFT',
        );
        // Add an explicit relationship for every join added.
        $table_data[$join['field']]['relationship'] = array(
          'label' => "{$table->get('name')}.{$join['field']} -> {$left_table}.{$join['left_field']}",
          'base' => $left_table,
          'base field' => $join['left_field'],
        );
      }
    }

    $data[$table->get('name')] = $table_data;
  }

  return $data;
}

/**
 * Return a views handler string for a given field type.
 *
 * @param $field_type
 *   Schema API field type.
 * @return
 *   Views handler string.
 */
function data_get_views_filter_handler($table, $field_name) {
  $schema = $table->get('table_schema');
  switch ($schema['fields'][$field_name]['type']) {
    case 'float':
    case 'numeric':
      return 'views_handler_filter_float';
    case 'int':
    case 'serial':
      return 'views_handler_filter_numeric';
    case 'datetime':
      // @TODO: make this conditional on whether the time is stored as a timestamp/datetime
      if (module_exists('date_api')) {
        return 'date_api_filter_handler';
      }
      return 'views_handler_filter_date';
  }
  return 'views_handler_filter_string';
}

/**
 * Return all available field handlers.
 */
function data_get_views_field_handlers() {
  return array(
    'views_handler_field' => 'views_handler_field',
    'views_handler_field_numeric' => 'views_handler_field_numeric',
    'views_handler_field_date' => 'views_handler_field_date',
  );
}

/**
 * Return a field handler string for a given field.
 * 
 * @param $table
 *   DataTable object.
 * @param $field_name
 *   String that defines the field.
 * @param 
 *   If TRUE, default value will be returned, if FALSE the user-defined value
 *   will be returned, if available.
 * 
 * @return 
 *   A string that defines the views handler to use.
 */
function data_get_views_field_handler($table, $field_name, $default = FALSE) {
  if (!$default) {
    $meta = $table->get('meta');
    if (isset($meta['fields'][$field_name]['views_field_handler'])) {
      return $meta['fields'][$field_name]['views_field_handler'];
    }
  }
  $schema = $table->get('table_schema');
  switch ($schema['fields'][$field_name]['type']) {
    case 'int':
    case 'float':
    case 'serial':
    case 'numeric':
      return 'views_handler_field_numeric';
    case 'datetime':
      return 'views_handler_field_date';
  }
  return 'views_handler_field';
}

// Return a argument handler string for a given field type.
function data_get_views_field_argument($table, $field_name) {
  $schema = $table->get('table_schema');
  switch ($schema['fields'][$field_name]['type']) {
    case 'int':
    case 'float':
    case 'serial':
    case 'numeric':
      return 'views_handler_argument_numeric';
    case 'datetime':
      return 'views_handler_argument_date';
    case 'varchar':
     return 'views_handler_argument_string';
  }
  return 'views_handler_argument';
}

// Return a sort handler string for a given field type.
function data_get_views_field_sort($table, $field_name) {
  $schema = $table->get('table_schema');
  switch ($schema['fields'][$field_name]['type']) {
    case 'datetime':
      return 'views_handler_sort_date';
  }
  return 'views_handler_sort';
}
