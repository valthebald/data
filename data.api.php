<?php
// $Id$

/**
 * @file
 * Documentation of hooks.
 *
 * @todo
 *   Add default hook documentation.
 */

/**
 * Invoked after a data record has been inserted.
 */
function hook_data_insert($record) {
}

/**
 * Invoked after a data record has been updated.
 */
function hook_data_update($record) {
}

/**
 * Invoked before a delete. Add to/remove from a delete query before deleting.
 */
function hook_data_delete_query_alter(&$query) {
}
