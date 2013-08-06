<?php

/**
 * @file
 * OgConsumer AJAX callbacks.
 */

/**
 * Fetch URL previews.
 */
function ogconsumer_ajax_fetch() {

  if (empty($_GET['u'])) {
    return MENU_NOT_FOUND;
  }

  if (is_array($_GET['u'])) {
    $input = $_GET['u'];
  } else {
    $input = array($_GET['u']);
  }

  foreach ($input as $url) {
    if (!valid_url($url, true)) {
      // This is not a webservice, offuscate return when dealing with
      // wrong stuff.
      return MENU_NOT_FOUND;
    }
  }

  drupal_json_output(
    ogconsumer_preview_fetch($input));
}
