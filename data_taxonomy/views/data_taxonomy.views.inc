<?php
// $Id$
/**
 * @file
 * Views hooks.
 */

/**
 * Implementation of hook_views_data().
 */
function data_taxonomy_views_data() {
  $data = array();

  $data['data_taxonomy']['table']['group']  = t('Data taxonomy');
  $data['data_taxonomy']['table']['title']  = t('Taxonomy');
  $data['data_taxonomy']['table']['join'] = array(
    'term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );
  // Add an explicit relationship to the node table.
  $data['data_taxonomy']['tid']['relationship'] = array(
    'help' => t('Relate data_taxonomy to node table.'),
    'label' => t('Relate data_taxonomy to node table'),
    'base' => 'term_data',
    'base field' => 'tid',
  );
  $data['data_taxonomy']['item_count'] = array(
    'title' => t('Item count'),
    'help' => t('A count of the number of data items related to this node.'),
    'field' => array(
      'handler' => 'data_taxonomy_views_handler_field_item_count',
    ),
  );

  $tables = data_get_all_tables();
  foreach ($tables as $table) {
    $meta = $table->get('meta');

    if (!empty($meta['data_taxonomy']['vocabularies'])) {
      $data['data_taxonomy']['table']['join'][$table->get('name')] = array(
        'left_field' => $meta['data_taxonomy']['id'],
        'field' => 'id',
      );
      $data[$table->get('name')]['data_taxonomy_form'] = array(
        'field' => array(
          'title' => t('Taxonomy form'),
          'help' => t('A taxonomy form for relating table records to taxonomy terms.'),
          'handler' => 'data_taxonomy_views_handler_field_form',
        ),
      );
    }
  }
  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function data_taxonomy_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'data_taxonomy') .'/views',
    ),
    'handlers' => array(
      'data_taxonomy_views_handler_field_form' => array(
        'parent' => 'views_handler_field',
      ),
      'data_node_views_handler_field_node_list' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}