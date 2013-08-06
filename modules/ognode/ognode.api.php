<?php

/**
 * @file
 * Open Graph Node API documentation.
 */

/**
 * Add or alter known node type list.
 *
 * Node type is the value of the "og:type" top level mandatory property for
 * Open Graph nodes.
 *
 * @param array $types
 */
function hook_ognode_node_type_list_alter(array &$list) {
  $list['book'] = t("Book");
}

/**
 * Add or alter known property list.
 *
 * Node type is the value of the "og:type" top level mandatory property for
 * Open Graph nodes.
 *
 * @param array $types
 */
function hook_ognode_property_list_alter(array &$list) {
  $list['og:author'] = t("Author");
}

/**
 * Alter metadata that will be used for populating the head section
 * of the HTML page.
 *
 * @param array &$ret
 *   Meta information, please consult ognode_entity_find_meta() documentation.
 * @param string $type
 *   Entity type.
 * @param object $entity
 *   Entity object.
 * @param array $properties
 *   Configuration, please consult ognode_entity_find_meta() documentation.
 *
 * @see ognode_entity_find_meta()
 */
function hook_og_meta_entity_alter(array &$meta, $type, $entity, array $properties) {

  $meta['og:profile'][] = array(
    'url'        => "http://john-doe.com",
    'first_name' => "John",
    'last_name'  => "Doh",
  );
  $meta['og:profile'][] = array(
    'url'        => "http://www.thesimpsons.com/#/characters",
    'first_name' => "Homer",
  );

  $meta['og:image'][] = array(
    'url'    => "http://example.com/example.png",
    'width'  => 100,
    'height' => 100,
  );
}

/**
 * Alter metadata that will be used for populating the head section
 * of the HTML page.
 *
 * @param array &$ret
 *   Meta information, please consult ognode_entity_find_meta() documentation.
 * @param object $entity
 *   Entity object.
 * @param array $properties
 *   Configuration, please consult ognode_entity_find_meta() documentation.
 *
 * @see ognode_entity_find_meta()
 */
function hook_og_meta_ENTITY_TYPE_alter(array &$meta, $entity, array $properties) {
  if (!isset($meta['og:title'])) {
    $meta['og:title'] = check_plain($entity->title);
  }
}

/**
 * Alter the supported field type list.
 *
 * @param string[] $list
 *   Keys are field types and values are callback. Each callback must accept
 *   three arguments:
 *     - $item : Field item
 *     - $type : Field type
 *     - $name : Open graph property name
 */
function hook_og_meta_og_meta_field_type_alter(array &$list) {
  $list['phone'] = 'my_module_og_phone';
  $list['email'] = function ($item, $type, $name) {
    return check_plain($item['value']);
  };
}
