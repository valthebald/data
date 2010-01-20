<?php
// $Id$
/**
 * @file
 * Admin UI functionality.
 */

/**
 * Form callback for relating a data table to a node.
 */
function data_taxonomy_settings_form(&$form_state, $table) {
  drupal_set_title($table->get('title'));

  $form = $vocabularies = array();
  foreach (taxonomy_get_vocabularies() as $v) {
    error_log(print_r($v, true));
    if ($v->tags) {
      $vocabularies[$v->vid] = $v->name;
    }
  }
  $meta = $table->get('meta');
  $form['#table'] = $table;
  $form['vocabularies'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Vocabularies'),
    '#options' => $vocabularies,
    '#default_value' => $meta['data_taxonomy']['vocabularies'],
    '#description' => t('Select the vocabulary to whose terms this data table\'s entries can be related to. Note: only free tagging vocabularies are supported.'),
  );
  $schema = $table->get('table_schema');
  $fields = drupal_map_assoc(array_keys($schema['fields']));
  $form['id'] = array(
    '#type' => 'select',
    '#title' => t('Identifier'),
    '#options' => $fields,
    '#default_value' => $meta['data_taxonomy']['id'],
    '#description' => t('Select the identifier of the data table that should be related to terms.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler for data_node_settings_form().
 */
function data_taxonomy_settings_form_submit($form, &$form_state) {
  $meta = $form['#table']->get('meta');
  $meta['data_taxonomy']['vocabularies'] = array_filter($form_state['values']['vocabularies']);
  $meta['data_taxonomy']['id'] = $form_state['values']['id'];
  $form['#table']->update(array('meta' => $meta));
  drupal_set_message(t('Settings saved.'));
}