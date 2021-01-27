<?php
/**
 * @file
 * Command(s) for working with Backdrop projects.
 */

/**
 * Implements hook_b_command().
 */
function projects_b_command() {
  return array(
    'projects' => array(
      'description' => bt('Display information about available projects (modules, themes, layouts).'),
      'callback' => 'projects_b_callback',
      'arguments' => array(
        'project' => bt('The name of a project to display information for. Leave blank to list information about all projects.'),
      ),
      'optional_arguments' => array('project'),
      'options' => array(
        'type' => array(
          'description' => bt('Restrict list to projects of a given type: `module`, `theme` or `layout`.'),
          'value' => bt('Type'),
        ),
      ),
      'aliases' => array('pml', 'pmi', 'project', 'pm-list', 'pm-info'),
      'bootstrap' => B_BOOTSTRAP_FULL,
      'examples' => array(
        'b projects webform' => bt('Show information about the Webform module.'),
        'b projects --type=layout' => bt('Show information about all available layouts.'),
        'b projects' => bt('Show information about all available projects.'),
      ),
    ),
    'enable' => array(
      'description' => bt('Enable one or more projects (modules, themes, layouts).'),
      'callback' => 'enable_b_callback',
      'arguments' => array(
        'projects' => bt('One or more projects to enable.'),
      ),
      'multiple_argument' => 'projects',
      'options' => array(
        'no-dependency-checking' => array(
          'description' => bt('Disable dependency-checking and enable module(s) regardless. This could cause problems if there are missing dependencies. Use with caution.'),
        ),
      ),
      'aliases' => array('en', 'pm-enable'),
      'bootstrap' => B_BOOTSTRAP_FULL,
      'examples' => array(
        'b enable webform' => bt('Enable the Webform module.'),
        'b enable --no-dependency-checking rules' => bt('Enable the Rules module, regardless of whether or not its dependencies are available.'),
        'b enable simplify thesis bamboo' => bt('Enable the Simplify module, Thesis theme, and Bamboo layout.'),
      ),
    ),
    'disable' => array(
      'description' => bt('Disable one or more projects (modules, themes, layouts).'),
      'callback' => 'disable_b_callback',
      'arguments' => array(
        'projects' => bt('One or more projects to disable.'),
      ),
      'multiple_argument' => 'projects',
      'options' => array(
        'no-dependency-checking' => array(
          'description' => bt('Disable dependency-checking and disable module(s) regardless. This could cause problems if there are other enabled modules that depend on this one. Use with caution.'),
        ),
      ),
      'aliases' => array('dis', 'pm-disable'),
      'bootstrap' => B_BOOTSTRAP_FULL,
      'examples' => array(
        'b disable webform' => bt('Disable the Webform module.'),
        'b disable --no-dependency-checking rules' => bt('Disable the Rules module, regardless of whether or not other modules depend on it.'),
        'b disable simplify thesis bamboo' => bt('Disable the Simplify module, Thesis theme, and Bamboo layout.'),
      ),
    ),
    'uninstall' => array(
      'description' => bt('Uninstall one or more modules.'),
      'callback' => 'uninstall_b_callback',
      'arguments' => array(
        'modules' => bt('One or more modules to uninstall.'),
      ),
      'multiple_argument' => 'modules',
      'options' => array(
        'no-dependency-checking' => array(
          'description' => bt('Disable dependency-checking and uninstall module(s) regardless. This could cause problems if there are other installed modules that depend on this one. Use with caution.'),
        ),
      ),
      'aliases' => array('pmu', 'pm-uninstall'),
      'bootstrap' => B_BOOTSTRAP_FULL,
      'examples' => array(
        'b uninstall webform' => bt('Uninstall the Webform module.'),
        'b uninstall --no-dependency-checking rules' => bt('Uninstall the Rules module, regardless of whether or not other modules depend on it.'),
        'b uninstall simplify thesis bamboo' => bt('Uninstall the Simplify module, Thesis theme, and Bamboo layout.'),
      ),
    ),
    'theme-default' => array(
      'description' => bt('Set the default theme.'),
      'callback' => 'theme_default_b_callback',
      'arguments' => array(
        'theme' => bt('The theme to set as the default theme. It will be enabled, if not already.'),
      ),
      'aliases' => array('default-theme'),
      'bootstrap' => B_BOOTSTRAP_FULL,
      'examples' => array(
        'b theme-default bartik' => bt('Set Bartik as the default theme.'),
      ),
    ),
    'theme-admin' => array(
      'description' => bt('Set the admin theme.'),
      'callback' => 'theme_admin_b_callback',
      'arguments' => array(
        'theme' => bt('The theme to set as the admin theme. It will be enabled, if not already.'),
      ),
      'aliases' => array('admin-theme'),
      'bootstrap' => B_BOOTSTRAP_FULL,
      'examples' => array(
        'b theme-admin basis' => bt('Set Basis as the admin theme.'),
      ),
    ),
  );
}

/**
 * Command callback: Display information about available projects (modules,
 * themes, layouts).
 */
function projects_b_callback($arguments, $options) {
  $rows = array();

  // Show information about a single project.
  if (!empty($arguments['project'])) {
    return projects_b_show_project($arguments['project']);
  }

  // Get module data.
  if ($options['type'] == 'module' || !isset($options['type'])) {
    $modules = system_rebuild_module_data();

    foreach ($modules as $module) {
      if (!empty($module->info['hidden'])) {
        continue;
      }

      $rows[] = array(
        array('value' => $module->name),
        array('value' => $module->info['name']),
        array('value' => $module->type),
        array('value' => $module->status ? bt('Enabled') : bt('Disabled')),
        array('value' => $module->info['version']),
      );
    }
  }

  // Get theme data.
  if ($options['type'] == 'theme' || !isset($options['type'])) {
    $themes = system_rebuild_theme_data();

    foreach ($themes as $theme) {
      if (!empty($theme->info['hidden'])) {
        continue;
      }

      $rows[] = array(
        array('value' => $theme->name),
        array('value' => $theme->info['name']),
        array('value' => $theme->type),
        array('value' => $theme->status ? bt('Enabled') : bt('Disabled')),
        array('value' => $theme->info['version']),
      );
    }
  }

  // Get layout data.
  if ($options['type'] == 'layout' || !isset($options['type'])) {
    $layouts = layout_get_layout_template_info();
    $disabled_layouts = config_get('layout.settings', 'excluded_templates');

    foreach ($layouts as $layout) {
      if (!empty($layout['hidden'])) {
        continue;
      }

      $rows[] = array(
        array('value' => $layout['name']),
        array('value' => $layout['title']),
        array('value' => $layout['type']),
        array('value' => !in_array($layout['name'], $disabled_layouts) ? bt('Enabled') : bt('Disabled')),
        array('value' => $layout['version']),
      );
    }
  }

  return array(
    array(
      'type' => 'table',
      'variables' => array(
        'rows' => $rows,
        'header' => array(
          array('value' => bt('Project')),
          array('value' => bt('Name')),
          array('value' => bt('Type')),
          array('value' => bt('Status')),
          array('value' => bt('Version')),
        ),
      ),
    ),
  );
}

/**
 * Command callback: Enable one or more projects (modules, themes, layouts).
 */
function enable_b_callback($arguments, $options) {
  $modules = array();
  $themes = array();
  $layouts = array();
  $disabled_layouts = array();
  $enable_dependencies = !isset($options['no-dependency-checking']);
  $flush_caches = FALSE;

  foreach ($arguments['projects'] as $project) {
    $already_enabled = FALSE;
    $success = FALSE;

    // Get project type.
    if (!$type = projects_b_get_type($project)) {
      b_message(bt('Project `@name` could not be found.', array(
        '@name' => $project,
      )), 'error');
      continue;
    }

    switch ($type) {
      case 'module':
        if (empty($modules)) {
          $modules = system_rebuild_module_data();
        }

        // Get module info.
        $module = $modules[$project];
        $name = $module->info['name'];

        // Check if already enabled.
        if ($module->status) {
          $already_enabled = TRUE;
          break;
        }

        // Enable module.
        if (module_enable(array($project), $enable_dependencies)) {
          $success = TRUE;
        }
        else {
          b_message(bt('The `@name` module could not be enabled due to missing dependencies.', array(
            '@name' => $name,
          )), 'error');
        }

        break;
      case 'theme':
        if (empty($themes)) {
          $themes = system_rebuild_theme_data();
        }

        // Get theme info.
        $theme = $themes[$project];
        $name = $theme->info['name'];

        // Check if already enabled.
        if ($theme->status) {
          $already_enabled = TRUE;
          break;
        }

        // Enable theme.
        theme_enable(array($project));
        $success = TRUE;

        break;
      case 'layout':
        $config = config('layout.settings');

        if (empty($layouts)) {
          $layouts = layout_get_layout_template_info();
          $disabled_layouts = $config->get('excluded_templates');
        }

        // Get layout info.
        $layout = $layouts[$project];
        $name = $layout->title;

        // Check if already enabled.
        if (!in_array($project, $disabled_layouts)) {
          $already_enabled = TRUE;
          break;
        }

        // Enable layout (i.e. remove it from the disabled list).
        $key = array_search($project, $disabled_layouts);
        unset($disabled_layouts[$key]);
        $config->set('excluded_templates', $disabled_layouts);
        $config->save();
        $success = TRUE;

        break;
    }

    // Display a message if the project was already enabled.
    if ($already_enabled) {
      b_message(bt('The `@name` @type is already enabled.', array(
        '@name' => $name,
        '@type' => $type,
      )), 'info');
    }

    // Display a message if the project was successfully enabled.
    if ($success) {
      b_message(bt('The `@name` @type was enabled.', array(
        '@name' => $name,
        '@type' => $type,
      )), 'success');

      $flush_caches = TRUE;
    }
  }

  // Flush caches once after all projects have been enabled.
  if ($flush_caches) {
    backdrop_flush_all_caches();
  }
}

/**
 * Command callback: Disable one or more projects (modules, themes, layouts).
 */
function disable_b_callback($arguments, $options) {
  $modules = array();
  $themes = array();
  $layouts = array();
  $disabled_layouts = array();
  $disable_dependents = !isset($options['no-dependency-checking']);
  $flush_caches = FALSE;

  foreach ($arguments['projects'] as $project) {
    $already_disabled = FALSE;
    $success = FALSE;

    // Get project type.
    if (!$type = projects_b_get_type($project)) {
      b_message(bt('Project `@name` could not be found.', array(
        '@name' => $project,
      )), 'error');
      continue;
    }

    switch ($type) {
      case 'module':
        if (empty($modules)) {
          $modules = system_rebuild_module_data();
        }

        // Get module info.
        $module = $modules[$project];
        $name = $module->info['name'];

        // Check if already disabled.
        if (!$module->status) {
          $already_disabled = TRUE;
          break;
        }

        // Disable module.
        module_disable(array($project), $disable_dependents);
        $success = TRUE;

        break;
      case 'theme':
        if (empty($themes)) {
          $themes = system_rebuild_theme_data();
        }

        // Get theme info.
        $theme = $themes[$project];
        $name = $theme->info['name'];

        // Check if already disabled.
        if ($theme->status) {
          $already_disabled = TRUE;
          break;
        }

        // Disable theme.
        theme_disable(array($project));
        $success = TRUE;

        break;
      case 'layout':
        $config = config('layout.settings');

        if (empty($layouts)) {
          $layouts = layout_get_layout_template_info();
          $disabled_layouts = $config->get('excluded_templates');
        }

        // Get layout info.
        $layout = $layouts[$project];
        $name = $layout->title;

        // Check if already disabled.
        if (in_array($project, $disabled_layouts)) {
          $already_disabled = TRUE;
          break;
        }

        // Disable layout (i.e. add it to the disabled list).
        $disabled_layouts[] = $project;
        $config->set('excluded_templates', $disabled_layouts);
        $config->save();
        $success = TRUE;

        break;
    }

    // Display a message if the project was already disabled.
    if ($already_disabled) {
      b_message(bt('The `@name` @type is already disabled.', array(
        '@name' => $name,
        '@type' => $type,
      )), 'info');
    }

    // Display a message if the project was successfully disabled.
    if ($success) {
      b_message(bt('The `@name` @type was disabled.', array(
        '@name' => $name,
        '@type' => $type,
      )), 'success');

      $flush_caches = TRUE;
    }
  }

  // Flush caches once after all projects have been disabled.
  if ($flush_caches) {
    backdrop_flush_all_caches();
  }
}

/**
 * Command callback: Uninstall one or more modules.
 */
function uninstall_b_callback($arguments, $options) {
  $modules = array();
  $uninstall_dependents = !isset($options['no-dependency-checking']);
  $flush_caches = FALSE;

  foreach ($arguments['modules'] as $module_name) {
    // Get module info.
    if (empty($modules)) {
      $modules = system_rebuild_module_data();
    }
    if (!isset($modules[$module_name])) {
      b_message(bt('The `@name` module could not be found.', array(
        '@name' => $module_name,
      )), 'error');
      continue;
    }
    $module = $modules[$module_name];

    // Make sure module is disabled.
    if ($module->status) {
      b_message(bt('The `@name` module is still enabled and so cannot be uninstalled. Disable it and try again.', array(
        '@name' => $module->info['name'],
      )), 'error');
      break;
    }

    // Check if already uninstalled.
    if ($module->schema_version == SCHEMA_UNINSTALLED) {
      b_message(bt('The `@name` module is already uninstalled.', array(
        '@name' => $module->info['name'],
      )), 'info');
      break;
    }

    // Uninstall module.
    if (backdrop_uninstall_modules(array($module), $uninstall_dependents)) {
      b_message(bt('The `@name` module was uninstalled.', array(
        '@name' => $module->info['name'],
      )), 'success');

      $flush_caches = TRUE;
    }
    else {
      b_message(bt('The `@name` module could not be uninstalled as other enabled modules depend on it.', array(
        '@name' => $module->info['name'],
      )), 'error');
    }
  }

  // Flush caches once after all projects have been disabled.
  if ($flush_caches) {
    backdrop_flush_all_caches();
  }
}

/**
 * Command callback: Set the default theme.
 */
function theme_default_b_callback($arguments, $options) {
  // Get theme info.
  $theme_name = $arguments['theme'];
  $themes = system_rebuild_theme_data();
  if (!isset($themes[$theme_name])) {
    b_message(bt('The `@name` theme could not be found.', array(
      '@name' => $theme_name,
    )), 'error');
    return;
  }
  $theme = $themes[$theme_name];

  // Enable if not already.
  if (!$theme->status) {
    theme_enable(array($theme_name));
  }

  // Set as default theme.
  config_set('system.core', 'theme_default', $theme_name);

  // Display success message.
  b_message(bt('`@name` was set as the default theme.', array(
    '@name' => $theme->info['name'],
  )), 'success');

  // Flush caches.
  backdrop_flush_all_caches();
}

/**
 * Command callback: Set the admin theme.
 */
function theme_admin_b_callback($arguments, $options) {
  // Get theme info.
  $theme_name = $arguments['theme'];
  $themes = system_rebuild_theme_data();
  if (!isset($themes[$theme_name])) {
    b_message(bt('The `@name` theme could not be found.', array(
      '@name' => $theme_name,
    )), 'error');
    return;
  }
  $theme = $themes[$theme_name];

  // Enable if not already.
  if (!$theme->status) {
    theme_enable(array($theme_name));
  }

  // Set as admin theme.
  config_set('system.core', 'admin_theme', $theme_name);

  // Display success message.
  b_message(bt('`@name` was set as the admin theme.', array(
    '@name' => $theme->info['name'],
  )), 'success');

  // Flush caches.
  backdrop_flush_all_caches();
}

/**
 * Show information about a single project.
 */
function projects_b_show_project($project) {
  $rows = array();

  // Get project type.
  if (!$type = projects_b_get_type($project)) {
    b_message(bt('Project `@name` could not be found.', array(
      '@name' => $project,
    )), 'error');
    return;
  }

  // Get project information.
  switch ($type) {
    case 'module':
      $modules = system_rebuild_module_data();
      $module = $modules[$project];

      $rows[] = array(
        array('value' => bt('Name')),
        array('value' => $module->info['name']),
      );
      $rows[] = array(
        array('value' => bt('Type')),
        array('value' => ucfirst($type)),
      );
      $rows[] = array(
        array('value' => bt('Project')),
        array('value' => $module->info['project']),
      );
      $rows[] = array(
        array('value' => bt('Description')),
        array('value' => $module->info['description']),
      );
      $rows[] = array(
        array('value' => bt('Configure')),
        array('value' => $module->info['configure']),
      );
      $rows[] = array(
        array('value' => bt('Package')),
        array('value' => $module->info['package']),
      );
      $rows[] = array(
        array('value' => bt('Tags')),
        array('value' => !empty($module->info['tags']) ? implode(', ', $module->info['tags']) : ''),
      );
      $rows[] = array(
        array('value' => bt('Version')),
        array('value' => $module->info['version']),
      );
      $rows[] = array(
        array('value' => bt('Path')),
        array('value' => dirname($module->uri)),
      );
      $rows[] = array(
        array('value' => bt('Status')),
        array('value' => $module->status ? bt('Enabled'): bt('Disabled')),
      );
      $rows[] = array(
        array('value' => bt('Requires')),
        array('value' => !empty($module->requires) ? implode(', ', $module->requires) : ''),
      );
      $rows[] = array(
        array('value' => bt('Required by')),
        array('value' => !empty($module->required_by) ? implode(', ', $module->required_by) : ''),
      );

      break;
    case 'theme':
      $themes = system_rebuild_theme_data();
      $theme = $themes[$project];

      $rows[] = array(
        array('value' => bt('Name')),
        array('value' => $theme->info['name']),
      );
      $rows[] = array(
        array('value' => bt('Type')),
        array('value' => ucfirst($type)),
      );
      $rows[] = array(
        array('value' => bt('Description')),
        array('value' => $theme->info['description']),
      );
      $rows[] = array(
        array('value' => bt('Version')),
        array('value' => $theme->info['version']),
      );
      $rows[] = array(
        array('value' => bt('Path')),
        array('value' => dirname($theme->uri)),
      );
      $rows[] = array(
        array('value' => bt('Status')),
        array('value' => $theme->status ? bt('Enabled'): bt('Disabled')),
      );

      break;
    case 'layout':
      $layout = layout_get_layout_template_info($project);

      $rows[] = array(
        array('value' => bt('Name')),
        array('value' => $layout['title']),
      );
      $rows[] = array(
        array('value' => bt('Type')),
        array('value' => ucfirst($type)),
      );
      $rows[] = array(
        array('value' => bt('Version')),
        array('value' => $layout['version']),
      );
      $rows[] = array(
        array('value' => bt('Path')),
        array('value' => $layout['path']),
      );
      $rows[] = array(
        array('value' => bt('Default region')),
        array('value' => $layout['default region']),
      );

      break;
  }

  return array(
    array(
      'type' => 'table',
      'variables' => array(
        'rows' => $rows,
        'delimiter' => '   ',
        'delimiter_left' => ' ',
        'delimiter_right' => ' ',
      ),
    ),
  );
}

/**
 * Get a project's type (i.e. module, theme or layout).
 */
function projects_b_get_type($project) {
  // Get the type from the `system` table.
  $type = db_query('SELECT type FROM {system} WHERE name = :name;', array(
    ':name' => $project,
  ))->fetchField();

  // Because only modules and themes are listed in the `system` table, no type
  // found means the project is either a layout, or doesn't exist.
  if (!$type) {
    if (layout_get_layout_template_info($project)) {
      $type = 'layout';
    }
    else {
      // Project doesn't appear to be a module, theme or layout.
      return FALSE;
    }
  }

  return $type;
}