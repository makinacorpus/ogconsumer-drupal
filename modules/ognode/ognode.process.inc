<?php

/**
 * @file
 * Process data.
 */

/**
 * Get list of supported field types
 *
 * @return string[]
 *   Keys are field types and values are callback. Each callback must accept
 *   three arguments:
 *     - $item : Field item
 *     - $type : Field type
 *     - $name : Open graph property name
 */
function ognode_field_supported_list() {
  // This needs caching because it might called multiple time on the
  // same request.
  $ret = &drupal_static(__FUNCTION__);
  if (null === $ret) {
    $ret = array(
      'datetime'          => 'ognode_field_process_date',
      'date'              => 'ognode_field_process_date',
      'datestamp'         => 'ognode_field_process_date',
      'file'              => 'ognode_field_process_file',
      'image'             => 'ognode_field_process_image',
      'list_boolean'      => 'ognode_field_process_boolean',
      'list_float'        => 'ognode_field_process_float',
      'list_integer'      => 'ognode_field_process_integer',
      'list_text'         => 'ognode_field_process_text',
      'text'              => 'ognode_field_process_text',
      'text_long'         => 'ognode_field_process_text',
      'text_with_summary' => 'ognode_field_process_text',
    );
    drupal_alter('og_meta_field_type', $ret);
  }
  return $ret;
}

/**
 * Process field value.
 *
 * God I hate Drupal Field API.
 *
 * @param string $type
 *   Entity type.
 * @param object $entity
 *   Entity object.
 * @param string $target
 *   Field name.
 * @param string $name
 *   Property name.
 *
 * @return array
 */
function ognode_field_process($type, $entity, $target, $name) {
  $ret = array();

  // WTF!
  $supported = ognode_field_supported_list();
  list(,, $bundle) = entity_extract_ids($type, $entity);
  $instances = field_info_instances($type, $bundle);

  // Double WTF!
  if (!isset($instances[$target]) ||
      !($field = field_info_field_by_id($instances[$target]['field_id'])) ||
      !isset($supported[$field['type']]) ||
      !is_callable($supported[$field['type']]))
  {
    return; // Unsupported field type.
  }

  $callback = $supported[$field['type']];

  // Triple WTF!
  $items = field_get_items($type, $entity, $target);
  if (!empty($items)) {
    foreach ($items as $item) {
      if ($value = call_user_func($callback, $item, $field['type'], $name)) {
        $ret[] = $value;
      }
    }
  }

  return $ret;
}

/**
 * Proxy to entity_uri() returning an absolute canonical URL.
 */
function ognode_entity_uri($type, $entity) {
  if ($uri = entity_uri($type, $entity)) {
    return url($uri['path'], array('absolute' => true) + $uri['options']);
  }
}

/**
 * Field process callback.
 */
function ognode_field_process_date($item, $type, $name) {
  // @todo
}

/**
 * Field process callback.
 */
function ognode_field_process_file($item, $type, $name) {
  if (!empty($item['uri'])) {
    $ret = array();
    $ret['content'] = file_create_url($item['uri']);
    if (!empty($item['filemime'])) {
      $ret['type'] = (string)$item['filemime'];
    }
    return $ret;
  }
}

/**
 * Field process callback.
 */
function ognode_field_process_image($item, $type, $name) {
  if (!empty($item['uri'])) {
    $ret = array();
    $ret['content'] = file_create_url($item['uri']);
    if (!empty($item['width'])) {
      $ret['width'] = (int)$item['width'];
    }
    if (!empty($item['height'])) {
      $ret['height'] = (int)$item['height'];
    }
    if (!empty($item['filemime'])) {
      $ret['type'] = (string)$item['filemime'];
    }
    return $ret;
  }
}

/**
 * Field process callback.
 */
function ognode_field_process_boolean($item, $type, $name) {
  if (!empty($item['value'])) {
    return $item['value'] ? true : false;
  }
}

/**
 * Field process callback.
 */
function ognode_field_process_float($item, $type, $name) {
  if (!empty($item['value'])) {
    return (float)$item['value'];
  }
}

/**
 * Field process callback.
 */
function ognode_field_process_integer($item, $type, $name) {
  if (!empty($item['value'])) {
    return (int)$item['value'];
  }
}

/**
 * Field process callback.
 */
function ognode_field_process_text($item, $type, $name) {
  if (!empty($item['value'])) {
    return text_summary($item['value'], null, 300);
  }
}
