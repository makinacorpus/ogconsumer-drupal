<?php

/**
 * @file
 * Timeline administration pages.
 */

/**
 * We need processing helpers for admin section.
 */
require_once __DIR__ . '/ognode.process.inc';

/**
 * Get Open Graph node known types.
 *
 * Do not ever use this function outside of admin pages.
 *
 * @return string[]
 *   Key value pairs, keys are exposed type via "og:type" and values are
 *   human readable descriptions.
 */
function ognode_node_type_list() {
  // This does not need cache because it's admin only.
  if (module_exists('ogconsumer') && false) {
    // @todo Use OgConsumer API
  } else {
    $ret = array(
      'actor'   => t("Actor"),
      'article' => t("Article"),
      'audio'   => t("Audio"),
      'blog'    => t("Blog"),
      'image'   => t("Image"),
      'website' => t("Video"),
      'website' => t("Website"),
    );
  }
  drupal_alter('ognode_node_type_list', $ret);
  asort($ret);
  return $ret;
}

/**
 * Get Open Graph node known property names.
 *
 * Do not ever use this function outside of admin pages.
 *
 * @return string[]
 *   Key value pairs, keys are exposed type via "og:type" and values are
 *   human readable descriptions.
 */
function ognode_property_list() {
  // This does not need cache because it's admin only.
  $ret = array(
    // Common values.
    'og:type'           => t("Default: Type"),
    'og:title'          => t("Default: Title"),
    'og:description'    => t("Default: Description"),
    'og:determiner'     => t("Default: Determiner"),
    'og:url'            => t("Default: URL"),
    // Contact information.
    'og:email'          => t("Contact: E-Mail"),
    'og:phone_number'   => t("Contact: Phone number"),
    'og:fax_number'     => t("Contact: Fax number"),
    // Medias.
    'og:audio'          => t("Media: Audio"),
    'og:image'          => t("Media: Image"),
    'og:video'          => t("Media: Video"),
    // Various.
    'og:author'         => t("Misc: Author"),
    // Localisation.
    'og:latitude'       => t("Localisation: Latitude"),
    'og:longitude'      => t("Localisation: Longitude"),
    // Address.
    'og:street_address' => t("Localisation address: Street"),
    'og:locality'       => t("Localisation address: Locality"),
    'og:region'         => t("Localisation address: Region"),
    'og:postal-code'    => t("Localisation address: Postal code"),
    'og:country-name'   => t("Localisation address: Country"),
  );
  drupal_alter('ognode_property_list', $ret);
  asort($ret);
  return $ret;
}

/**
 * Site global information form.
 */
function ognode_admin_info($form, &$form_state) {

  $form['#tree'] = true;

  $meta = variable_get(OGNODE_VAR_SITE_INFO, array());
  $form[OGNODE_VAR_SITE_INFO] = array(
    '#type'          => 'fieldset',
    '#title'         => t("Default values"),
  );
  $form[OGNODE_VAR_SITE_INFO]['og:type'] = array(
    '#title'         => "og:type",
    '#type'          => 'select',
    '#options'       => array(null => '-- '. t("None") . ' --') + ognode_node_type_list(),
    '#description'   => t("Default is 'website'."),
    '#default_value' => isset($meta['og:type']) ? $meta['og:type'] : null,
  );
  $form[OGNODE_VAR_SITE_INFO]['og:title'] = array(
    '#title'         => "og:title",
    '#type'          => 'textfield',
    '#description'   => t("Default is site name."),
    '#default_value' => isset($meta['og:title']) ? $meta['og:title'] : null,
  );
  // @todo Image upload for default image.
  $form[OGNODE_VAR_SITE_INFO]['og:image'] = array(
    '#title'         => "og:image",
    '#type'          => 'textfield',
    '#description'   => t("Default is site logo if any set."),
    '#default_value' => isset($meta['og:image']) ? $meta['og:image'] : null,
  );
  $form[OGNODE_VAR_SITE_INFO]['og:description'] = array(
    '#title'         => "og:description",
    '#type'          => 'textarea',
    '#rows'          => 2,
    '#description'   => t("Default is site slogan if any set."),
    '#default_value' => isset($meta['og:description']) ? $meta['og:description'] : null,
  );

  return system_settings_form($form);
}

/**
 * Site global information form submit.
 */
function ognode_admin_info_submit($form, &$form_state) {
  // @todo
}

/**
 * Various settings form.
 */
function ognode_admin_settings($form, &$form_state) {

  // @todo

  return system_settings_form($form);
}

/**
 * Bundle selection form.
 */
function ognode_admin_bundle($form, &$form_state) {

  $entity_info = entity_get_info();

  $options = array();
  foreach ($entity_info as $type => $info) {
    foreach ($info['bundles'] as $bundle => $bundle_info) {
      // Open Graph data can exist only when the bundle can be exposed
      // via an uri callback.
      if (isset($info['uri callback']) || isset($bundle_info['uri callback'])) {
        $options[$type][$type . ':' . $bundle] = $bundle_info['label'];
      }
    }
  }

  $default = variable_get(OGNODE_VAR_BUNDLES);
  $form['bundles'] = array(
    '#type'          => 'fieldset',
    '#title'         => format_plural(count($default), "@count enabled bundle", "@count enabled bundles"),
    '#collapsible'   => true,
    '#collapsed'     => !empty($default),
  );
  $form['bundles'][OGNODE_VAR_BUNDLES] = array(
    '#title'         => "Expose Open Graph data on this bundles",
    '#type'          => 'select',
    '#options'       => $options,
    '#multiple'      => true,
    '#size'          => count($options, COUNT_RECURSIVE),
    '#default_value' => $default,
  );
  $form['bundles']['actions'] = array(
    '#type'  => 'actions',
    'submit' => array(
      '#type'   => 'submit',
      '#value'  => t("Save"),
      '#submit' => array('ognode_admin_bundle_submit_field'),
    ),
  );

  if (!empty($default)) {

    $rows = array();
    foreach ($default as $key) {
      list($type, $bundle) = explode(':', $key);

      $options = array('query' => array('destination' => current_path()));
      $links = array(
        l(t("customize"), 'admin/config/services/ognode/bundles/' . $key, $options)
      );

      $rows[] = array(
        $entity_info[$type]['label'],
        $entity_info[$type]['bundles'][$bundle]['label'],
        implode(' / ', $links),
      );
    }

    $form['table'] = array(
      '#theme'  => 'table',
      '#rows'   => $rows,
      '#header' => array(
         t("Entity type"),
         t("Bundle"),
         '',
      ),
    );
  }

  return $form;
}

/**
 * Bundle selection form submit.
 */
function ognode_admin_bundle_submit_field($form, &$form_state) {

  $enabled  = $form_state['values'][OGNODE_VAR_BUNDLES];
  $old      = variable_get(OGNODE_VAR_BUNDLES, array());
  $add      = array_diff($enabled, $old);
  $del      = array_diff($old, $enabled);

  variable_set(OGNODE_VAR_BUNDLES, array_filter($enabled));

  // Add the global field if needed.
  $field = field_info_field(OGNODE_FIELD);
  if (empty($field)) {
    $field = array(
      'field_name' => OGNODE_FIELD,
      'type'       => OGNODE_FIELD,
    );
    field_create_field($field);
  }

  foreach (array_keys($add) as $id) {
    list($type, $bundle) = explode(':', $id);
    if (!$instance = field_info_instance($type, OGNODE_FIELD, $bundle)) {
      $instance = array(
        'field_name'  => OGNODE_FIELD,
        'locked'      => TRUE,
        'entity_type' => $type,
        'bundle'      => $bundle,
        'label'       => t("Open Graph data"),
        'display'     => array(),
      );
      field_create_instance($instance);
      drupal_set_message(t('Added Open Graph data field on bundle @bundle.', array('@bundle' => $bundle)));
    }
  }

  foreach (array_keys($del) as $id) {
    list($type, $bundle) = explode(':', $id);
    if ($instance = field_info_instance($type, OGNODE_FIELD, $bundle)) {
      field_delete_instance($instance);
      drupal_set_message(t('Removed Added Open Graph data field from bundle @bundle.', array('@bundle' => $bundle)));
    }
  }
}

/**
 * Single bundle customization page.
 */
function ognode_admin_bundle_customize_page($key) {

  list($type, $bundle) = explode(':', $key);

  if (!$instance = field_info_instance($type, OGNODE_FIELD, $bundle)) {
    return MENU_NOT_FOUND;
  }

  return drupal_get_form('ognode_admin_bundle_customize', $type, $bundle, $instance);
}

/**
 * Single bundle customization form.
 */
function ognode_admin_bundle_customize($form, &$form_state, $type, $bundle) {

  $form['#tree'] = true;
  $form['#theme'] = 'ognode_admin_bundle_customize';

  $count    = 0;
  $instance = field_info_instance($type, OGNODE_FIELD, $bundle);
  $methods  = array(
    null           => t("Value"),
    'property'     => t("Property"),
    'field'        => t("Field"),
    'callback'     => t("Callback"),
  );

  $form['type'] = array(
    '#type'  => 'value',
    '#value' => $type,
  );
  $form['bundle'] = array(
    '#type'  => 'value',
    '#value' => $bundle,
  );

  // Lookup for available fields on the entity.
  $fields = array();
  $supported_fields = ognode_field_supported_list();

  foreach (field_info_instances($type, $bundle) as $field_name => $instance) {
    if (($field = field_info_field_by_id($instance['field_id'])) && isset($supported_fields[$field['type']])) {
      // Allow only supported fields in select list.
      $fields[$field_name] = $instance['label'];
    }
  }

  $settings = $instance['settings']['properties'];
  for ($i = 0; $i < 4; ++$i) {
    $settings[] = '';
  }

  $properties = ognode_property_list();
  $properties_options = array('remove' => '-- ' . t("Select") . ' --') + $properties + array(null => t("Custom"));

  foreach ($settings as $name => $values) {

    // Values can be either arrays or single values.
    if (!is_array($values)) {
      $values = array($values);
    }

    foreach ($values as $value) {

      if (strpos($value, ':')) {
        list($method, $target) = explode(':', $value, 2);
      } else {
        $method = null;
        $target = $value;
      }

      if (is_numeric($name)) {
        $name = '';
      }

      $property = array();

      $property['property'] = array(
        '#title'         => t("Property"),
        '#type'          => 'select',
        '#options'       => $properties_options,
        '#default_value' => empty($name) ? 'remove' : (isset($properties[$name]) ? $name : null),
        '#id'            => 'prop-' . $count,
      );
      $property['name'] = array(
        '#title'         => t("Custom name"),
        '#type'          => 'textfield',
        '#size'          => 20,
        '#default_value' => $name,
        '#states'        => array(
          'visible'     => array(
            '#prop-' . $count => array('value' => ''),
          ),
        ),
      );
      $property['method'] = array(
        '#title'         => t("Selection method"),
        '#type'          => 'select',
        '#options'       => $methods,
        '#default_value' => $method,
        '#id'            => 'prop-method-' . $count,
        '#states'        => array(
          'visible'      => array(
            '#prop-' . $count => array('!value' => 'remove'),
          ),
        ),
      );

      foreach (array_keys($methods) as $key) {
        $property['target_' . $key] = array(
          '#title'         => t("Data target"),
          '#default_value' => $method == $key ? $target : null,
          '#states'        => array(
            'visible'      => array(
              '#prop-method-' . $count => array('value' => $key),
            ),
          ),
        );
        switch ($key) {

          case 'field':
            $property['target_' . $key]['#type'] = 'select';
            $property['target_' . $key]['#options'] = $fields;
            $property['target_' . $key]['#default_value'] = isset($fields[$target]) ? $target : null;
            break;

          default:
            $property['target_' . $key]['#type'] = 'textfield';
            $property['target_' . $key]['#size'] = 20;
        }
      }

      $property['weight'] = array(
        '#title'         => t("Order"),
        '#type'          => 'weight',
        '#title'         => t("Weight"),
        '#default_value' => $count++,
      );

      $form['properties'][] = $property;
    }
  }

  $form['actions'] = array(
    '#type'  => 'actions',
    'submit' => array(
      '#type'  => 'submit',
      '#value' => t("Save"),
    ),
    'cancel' => array(
      '#markup' => l(t("Cancel"), isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/services/ognode/bundles')
    ),
  );

  return $form;
}

/**
 * Single bundle customization form validate.
 */
function ognode_admin_bundle_customize_validate($form, &$form_state) {
  // @todo There's probably stuff to validate.
  // Yes! At least callback should be callable.
}

/**
 * Single bundle customization form submit.
 */
function ognode_admin_bundle_customize_submit($form, &$form_state) {

  $properties = array();
  $type       = $form_state['values']['type'];
  $bundle     = $form_state['values']['bundle'];

  if (!$instance = field_info_instance($type, OGNODE_FIELD, $bundle)) {
    drupal_set_message(t("An illegal choice has been detected. Please contact the site administrator."), 'error');
    $form_state['redirect'] = 'admin/config/services/ognode/bundles';

    return;
  }

  foreach ($form_state['values']['properties'] as $index => $data) {

    if ('remove' === $data['name']) {
      continue; // User did not select anything.
    }
    if (empty($data['target_' . $data['method']])) {
      continue; // Value is empty.
    }
    if (empty($data['method'])) {
      $value = $data['target_' . $data['method']];
    } else {
      $value = $data['method'] . ':' . $data['target_' . $data['method']];
    }

    if (empty($data['property'])) {
      $name = $data['name'];
    } else {
      $name = $data['property'];
    }

    $properties[$name][] = $value;
  }

  $instance['settings']['properties'] = $properties;
  field_update_instance($instance);

  drupal_set_message(t("Changes have been saved."));

  $form_state['redirect'] = 'admin/config/services/ognode/bundles';
}

/**
 * Single bundle customization theme.
 */
function theme_ognode_admin_bundle_customize(&$variables) {

  $form   = &$variables['form'];
  $output = '';

  $rows   = array();
  $header = array(
    t("Property"),
    t("Name"),
    t("Selection method"),
    t("Data target"),
    t("Order"),
  );

  $methods = array(null, 'callback', 'field', 'property');

  foreach (element_children($form['properties']) as $index) {

    // Remove element titles since we're adding them into the table heaer.
    unset(
      $form['properties'][$index]['property']['#title'],
      $form['properties'][$index]['name']['#title'],
      $form['properties'][$index]['method']['#title'],
      $form['properties'][$index]['weight']['#title']
    );
    foreach ($methods as $method) {
      unset($form['properties'][$index]['target_' . $method]['#title']);
    }

    $form['properties'][$index]['weight']['#attributes']['class'][] = 'og-meta-order';

    $row = array(
      'data' => array(),
      'class' => array('draggable'),
    );
    $row['data'][] = drupal_render($form['properties'][$index]['property']);
    $row['data'][] = drupal_render($form['properties'][$index]['name']);
    $row['data'][] = drupal_render($form['properties'][$index]['method']);
    $target_output = '';
    foreach ($methods as $method) {
      $target_output .= drupal_render($form['properties'][$index]['target_' . $method]);
    }
    $row['data'][] = $target_output;
    $row['data'][] = drupal_render($form['properties'][$index]['weight']);
    $rows[] = $row;
  }

  drupal_add_tabledrag('og-meta-order', 'order', 'sibling', 'og-meta-order');
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'og-meta-order')));

  return $output . drupal_render_children($form);
}

