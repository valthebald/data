<?php
/**
 * @file
 * Contains general page callbacks and associated functions.
 */

/**
 * Form builder for viewing and editing a data entity.
 */
function data_entity_entity_edit_form($form, &$form_state, $table, $data_entity = NULL) {
  $form = array();

  // Add in our defaults to the table meta data.
  data_entity_meta_add_defaults($table->meta);

  $form['table'] = array(
    '#type' => 'value',
    '#value' => $table,
  );

  $form['data'] = array(
    '#tree' => TRUE,
  );
  foreach ($table->table_schema['fields'] as $field_name => $field) {
    // For some reason these are lower case as entity keys.
    $field_id_safe = strtolower($field_name);
    $label = !empty($table->meta['fields'][$field_name]['label']) ? $table->meta['fields'][$field_name]['label'] : $field_name;
    $description = '';

    $id_field = data_entity_get_id_field($table);
    if ($id_field == $field_name) {
      $disabled = TRUE;
      $description .= t('The id field can not be edited.');
    }
    else {
      $disabled = $table->meta['fields'][$field_name]['locked'];
    }

    $form['data'][$field_name] = array(
      '#type' => 'textfield',
      '#title' => $label,
      '#description' => $description,
      // We need this check because after adding a field this gives errors.
      '#default_value' => isset($data_entity->$field_id_safe) ? $data_entity->$field_id_safe : NULL,
      '#disabled' => $disabled,
      '#required' => $table->meta['fields'][$field_name]['required'],
    );
  }

  $form['buttons']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submit handler for saving a data entity.
 */
function data_entity_entity_edit_form_submit($form, &$form_state) {
  $table = $form_state['values']['table'];

  $record = $form_state['values']['data'];
  drupal_write_record($table->name, $record, $table->table_schema['primary key']);
}
