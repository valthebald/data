<?php
// $Id$

/**
 * @todo
 */
class data_taxonomy_views_handler_field_form extends views_handler_field {
  protected $id;

  /**
   * Suppress options.
   */
  function options_form(&$form, &$form_state) {
    return array();
  }

  /**
   * Add field on which we join.
   */
  function query() {
    if (user_access('edit data taxonomy relations')) {
      $this->ensure_my_table();

      // Add the id field for this table to the query.
      $table = data_get_table($this->table);
      $meta = $table->get('meta');
      if (!empty($meta['data_taxonomy']['vocabularies']) && isset($meta['data_taxonomy']['id'])) {
        $this->id = $this->query->add_field($this->table_alias, $meta['data_taxonomy']['id']);
      }
      $this->add_additional_fields();
    }
  }

  /**
   * Render form.
   */
  function render($values) {
    $output = '';
    if (user_access('edit data taxonomy relations')) {
      $table = data_get_table($this->table);
      $meta = $table->get('meta');
      if (!empty($meta['data_taxonomy']['vocabularies'])) {
        foreach ($meta['data_taxonomy']['vocabularies'] as $vid) {
          $output .= drupal_get_form('data_taxonomy_tagging_form', $vid, $values->{$this->id}, $this->table);
        }
      }
    }
    return $output;
  }
}