<?php

/**
 * @file
 * Open Graph Node field definition.
 */

/**
 * Implements hook_field_info().
 */
function ognode_field_info() {
  return array(
    OGNODE_FIELD => array(
      'label'             => t("Open Graph data"),
      'description'       => t("Stores custom Open Graph data and restore them based upon the current entity data."),
      'settings'          => array(),
      'instance_settings' => array(
        'properties'      => array(
          // Those are sensible mandatory og: default properties.
          // og:title and og:url can be computed using respectively
          // entity_label() and entity_uri().
          // User will be able to add as many properties as he wishes
          // but first he'll need to configure known object structures
          // and type at a site wide level.
          // og:audio and og:video fields will be available for selection
          // as soon as audio or video fields will be defined on the same
          // bundle.
          'og:title'       => 'callback:entity_label',
          'og:url'         => 'callback:ognode_entity_uri',
          'og:type'        => 'article',
          'og:image'       => 'property:picture',
          'og:description' => 'field:body',
        ),
      ),
      'default_widget'    => 'og_meta_hidden',
      'default_formatter' => 'og_meta_hidden',
      'no_ui'             => true,
    ),
  );
}

/**
 * Implements hook_field_formatter_info().
 */
function ognode_field_formatter_info() {
  return array(
    // Open Graph data cannot be realistically formatted for the end user
    // views since it's data that is supposed to be consumed by external
    // web services only.
    'og_meta_hidden' => array(
      'label'       => t("Hidden"),
      'description' => t("Open Graph data has no interest in being displayed."),
      'field types' => array(OGNODE_FIELD),
    ),
  );
}

/**
 * Implements hook_field_formatter_info().
 */
function ognode_field_widget_info() {
  return array(
    // Open Graph data aims to be auto-configured in most cases, we won't
    // leave any choice to the user per default. He should never have to
    // deal with machine driven data that is supposed to exists only for
    // external sites exchange.
    'og_meta_hidden' => array(
      'label'       => t("Hidden"),
      'description' => t("Hide Open Graph data to the user and expose only automatically extracted data from current entity."),
      'field types' => array(OGNODE_FIELD),
    ),
    // For what it worth sites builders might choose to leave the user some
    // level of flexibility on what's provided to the outside world. The
    // simple widget will only expose obvious properties in which you'll
    // find "og:title", "og:type" and "og:description". All other complex
    // properties such as "og:image" should be driven by other fields.
    'og_meta_simple' => array(
      'label'       => array("Simple"),
      'description' => t("Allow the user to override a few mandatory properties, including type, title and description only."),
      'field types' => array(OGNODE_FIELD),
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function ognode_field_is_empty() {
  return false;
}
