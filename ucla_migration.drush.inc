<?php

/**
 * @file
 * A drush script for UCLA collection migration.
 */

/**
 * Implements hook_drush_command().
 */
function ucla_migration_drush_command() {
  $items = array();
  $items['ucla_save_pids_for_namespace'] = array(
    'callback' => 'ucla_migration_save_pids_for_namespace',
    'description' => 'Gets all PIDS for the given namespace and saves them to a file.',
    'arguments' => array(
      'fedora_namespace' => "The Fedora namespace whose PIDS you're looking for.",
    ),
    'options' => array(),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com ucla_save_pids_for_namespace your_namespace',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['ucla_make_namespace_versionable'] = array(
    'callback' => 'ucla_migration_make_all_datastreams_versionable',
    'description' => "Sets all datastreams for all objects to versionable in the given namespace. Only run after executing ucla_save_pids_for_namespace and confirming that Fedora's resource index is turned off.",
    'arguments' => array(
      'fedora_namespace' => "The Fedora namespace whose objects' datastreams you wish to make versionable.",
    ),
    'options' => array(),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com ucla_make_namespace_versionable your_namespace',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['ucla_migrate_mets'] = array(
    'callback' => 'ucla_migration_migrate_mets',
    'description' => 'Ingest content from METS file for all records.',
    'arguments' => array(
      'file_path' => 'The file path to the METS file.',
      'fedora_namespace' => "The Fedora namespace to ingest everything under.",
      'recovery_number' => "The numerical portion of the PID of the last ingested object.",
    ),
    'options' => array(),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com ucla_migrate_mets /home/dgiuser/21198-zz00294nxr.xml uclans',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['ucla_migrate_from_dmdid'] = array(
    'callback' => 'ucla_migration_migrate_from_dmdid',
    'description' => 'Ingest content from METS file for a single record.',
    'arguments' => array(
      'file_path' => 'The file path to the METS file.',
      'fedora_namespace' => "The Fedora namespace to ingest the record under.",
      'dmdid' => "The record ID provided in the MODS for this particular record.",
    ),
    'options' => array(),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com ucla_migrate_from_dmdid /home/dgiuser/21198-zz00294nxr.xml uclans 21198-zz002bzvvs',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['ucla_migrate_datastream'] = array(
    'callback' => 'ucla_migration_migrate_datastream',
    'description' => 'Ingest content from METS file to update datastreams to objects that already exist in Fedora.',
    'arguments' => array(
      'file_path' => 'The file path to the METS file.',
      'datastream_path' => 'The file path to the directory containing the datastreams (can be remote or local)',
      'fedora_namespace' => 'The Fedora namespace to update the datastreams under.',
    ),
    'options' => array(),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com ucla_migrate_datastream http://example.com/datastream_dir edu.ucla.library.tahrir',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['ucla_migrate_attach_datastream'] = array(
    'callback' => 'ucla_migration_attach_datastream',
    'description' => 'Ingest content from METS file to update datastreams to objects that already exist in Fedora.',
    'arguments' => array(
      'datastream_path' => 'The file path to the file containing paths to the assets.',
      'datastream_id' => 'The datastream id to assign to the added asset, will also be used for the ds label',
      'mime_type' => 'The mime type to add to the attached datastream',
      'fedora_namespace' => 'The Fedora namespace to update the datastreams under.',
    ),
    'options' => array(),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com ucla_migrate_attach_datastream http://example.com/datastream_file_of_assets PDF_FILE application/pdf edu.ucla.library.project ',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  $items['ucla_migrate_attach_datastream'] = array(
    'callback' => 'ucla_migration_attach_datastream',
    'description' => 'Ingest content from a specified source to update datastreams to objects that already exist in Fedora.',
    'arguments' => array(
      'datastream_path' => 'The file path to the file containing paths to the assets.',
      'datastream_id' => 'The datastream id to assign to the added asset, will also be used for the ds label',
      'mime_type' => 'The mime type to add to the attached datastream',
      'fedora_namespace' => 'The Fedora namespace to update the datastreams under.',
    ),
    'options' => array(),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com ucla_migrate_attach_datastream http://example.com/datastream_file_of_assets PDF_FILE application/pdf edu.ucla.library.project ',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  $items['ucla_migrate_delete_datastream'] = array(
    'callback' => 'ucla_migration_delete_datastream',
    'description' => 'Delete a datastream given a datastream id and options for deletion.',
    'options' => array(
      'datastream_id' => array(
        'description' => 'The datastream id to assign to the added asset, will also be used for the ds label',
        'required' => 'true'),
      'pid_numbers' => array(
        'description' => 'The numeric values to combine with the namespace to generate pids to take action on',
        'required' => 'true'),
      'fedora_namespace' => array(
        'description' => 'The Fedora namespace to update the datastreams under.',
        'required' => 'true'),
    ),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com ucla_migrate_delete_datastream',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  $items['ucla_migrate_min_metadata'] = array(
    'callback' => 'minimum_metadata_ingest',
    'description' => 'Ingest content given digital assets but no metadata files.',
    'options' => array(
      'fedora_namespace' => array(
        'description' => 'The Fedora namespace to update the datastreams under.',
        'required' => 'true'),
      'manifest_file_path' => array(
        'description' => 'The path to the file containing a list of assets to ingest.',
        'required' => 'true'),
      'overwrite' => array(
        'description' => 'If true, remove existing Fedora objects in the namespace with the same filename/title.',
        'required' => 'true'),
    ),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com ucla_migrate_min_medatata fedoranamespace /path/to/manifest/file.txt',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  return $items;
}

/**
 * Saves all pids in a Fedora namespace to a file.
 *
 * This is done so ucla_make_namespace_versionable can have a list of all pids
 * for the given namespace after Fedora's resource index gets disabled.  Output
 * file is written to the ucla_migration module folder.  It is given the name
 * of the fedora namespace and an extension of '.pid'.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace whose PIDS you want.
 */
function ucla_migration_save_pids_for_namespace($fedora_namespace) {
  // Get a connection to Fedora.
  $connection = islandora_get_tuque_connection();
  $repo = $connection->repository;

  // SPARQL query to get all objects in a given namespace.
  $query =
      "
    SELECT ?obj
    FROM <#ri>
    WHERE {
      ?obj <info:fedora/fedora-system:def/model#hasModel> ?model .
      FILTER(regex(str(?obj), '^info:fedora/$fedora_namespace:'))
    }
  ";

  // Execute the query.
  $results = $repo->ri->sparqlQuery($query);

  // Put all the pids in an array.
  $pids = array();
  foreach ($results as $result) {
    array_push($pids, $result['obj']['value']);
  }

  // JSON encode the array and write it to a file.
  $path = DRUPAL_ROOT . '/' . drupal_get_path('module', 'ucla_migration') . "/$fedora_namespace.pids";
  $fp = fopen($path, 'w');
  if (!$fp) {
    drush_set_error("Can't open file to write", "Cannot open file $path to write.  Check your permissions for the ucla_migration module folder or run this script using sudo.", "Error: "
    );
    return;
  }

  if (!fwrite($fp, json_encode($pids))) {
    drush_set_error("Can't write pids to file", "Cannot write pids to $path.  Check your permissions for the ucla_migration module folder or run this script using sudo.", "Error: "
    );
    fclose($fp);
    return;
  }

  fclose($fp);
  return;
}

/**
 * Sets all datastreams for all objects in a Fedora namespace to versionable.
 *
 * Reads in the output of ucla_migration_save_pids_for_namespace because this
 * cannot be run with Fedora's resource index enabled.  Be sure to execute the
 * ucla_save_pids_for_namespace Drush command first, then disable the resource
 * index and restart Fedora.  Then run this command.  Then re-enable the
 * resource index and restart Fedora again.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace whose datastreams you want to fix.
 */
function ucla_migration_make_all_datastreams_versionable($fedora_namespace) {
  // Open up the pid file to read.
  $path = DRUPAL_ROOT . '/' . drupal_get_path('module', 'ucla_migration') . "/$fedora_namespace.pids";
  $fp = fopen($path, 'r');

  if (!$fp) {
    drush_set_error("Can't open file to read", "Cannot open file $path to read.  Does it exist?  Check your permissions for the ucla_migration module folder or run this script using sudo.", "Error: "
    );
    return;
  }

  // Grab its contents.
  $contents = fread($fp, filesize($path));
  if (!$contents) {
    drush_set_error("Can't read file contents.", "Cannot read contents of $path.  Does it exist?  Check your permissions for the ucla_migration module folder or run this script using sudo.", "Error: "
    );
    fclose($fp);
    return;
  }

  // Close the file pointer so we're not leaking all over the place.
  fclose($fp);

  // JSON decode the array of pids.
  $pids = json_decode($contents);

  // Set each datastream for each object in the namespace to versionable.
  foreach ($pids as $pid) {
    $obj = islandora_object_load($pid);
    foreach ($obj as $dsid => $dsObj) {
      $dsObj->versionable = TRUE;
    }
  }
}

/**
 * Migrates all records in a METS file.
 *
 * Executes the migration based on a mets file.  Can recover a previously
 * failed ingest if the numeric portion of the last successfully ingested
 * object is provided as a command line argument.
 *
 * @param string $file_path
 *   The path to the mets file serving as the manifest for this ingest.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace to ingest everything under.
 *
 * @param string $recovery_number
 *   The number portion of the PID from the last successfully ingested object.
 */
function ucla_migration_migrate_mets($file_path, $fedora_namespace, $recovery_number = 0) {
  // Check to make sure the path to the mets file has been provided.
  if (!isset($file_path)) {
    drush_set_error('File path not provided', 'Path to mets file not provided as command line argument', 'Error: ');
    return;
  }

  // Attempt to grab the mets file.
  $mets_file = file_get_contents($file_path);
  if (!$mets_file) {
    drush_set_error('File not found', "Could not locate mets file $file_path", "Error: ");
    return;
  }

  $mets_xml = simplexml_load_string($mets_file);

  // Register namespaces of the root element.
  $xml_namespaces = $mets_xml->getDocNamespaces();
  $mets_xml->registerXPathNamespace('mets', $xml_namespaces['mets']);
  $mets_xml->registerXPathNamespace('xlink', $xml_namespaces['xlink']);

  // Either start a new migration, or recover a previously failed one,
  // whichever is appropriate.
  $results = FALSE;
  if ($recovery_number < 1) {
    $results = ucla_migration_start_new_migration($mets_xml, $fedora_namespace);
  }
  else {
    $results = ucla_migration_recover_migration($mets_xml, $fedora_namespace, $recovery_number);
  }

  // Final message to user.
  if ($results) {
    drush_log("Successfully migrated {$collection->label} collection.", 'ok');
  }
  else {
    drush_log("Failed to migrate {$collection->label} collection.  Check the watchdog logs for more info.", 'error');
  }

  return;
}

/**
 * Starts a new migration from scratch.
 *
 * Executes the migration based on a mets file.  Objects are ingested under the
 * supplied namespace.
 *
 * @param SimpleXMLElement $mets_xml
 *   SimpleXML object that represents the mets file from the path provided to
 *   the drush script.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace to ingest everything under.
 *
 * @return bool
 *   Returns TRUE if success, FALSE if error.
 */
function ucla_migration_start_new_migration($mets_xml, $fedora_namespace) {
  // Create the collection first.
  $collection = ucla_migration_migrate_collection_object($fedora_namespace, $mets_xml);

  // Exit if ingest of collection object failed.
  if (!$collection) {
    watchdog('UCLA Migration', 'Collection object ingest failed.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  return ucla_migration_migrate_all_children($fedora_namespace, $mets_xml, $collection);
}

/**
 * Recovers a failed migration.
 *
 * Starting with the last successfully ingested object, objects from the mets
 * xmls are ingested into the supplied namespace.
 *
 * @param SimpleXMLElement $mets_xml
 *   SimpleXML object that represents the mets file from the path provided to
 *   the drush script.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace to ingest everything under.
 *
 * @param string $recovery_number
 *   The number portion of the PID from the last successfully ingested object.
 *
 * @return bool
 *   Returns TRUE if success, FALSE if error.
 */
function ucla_migration_recover_migration($mets_xml, $fedora_namespace, $recovery_number) {
  module_load_include('inc', 'islandora', 'includes/utilities');

  // Attempt to get the object specified by the recovery number,
  // that is, the last successfully ingested object.
  $last_good_pid = $fedora_namespace . ":" . $recovery_number;
  $last_good_object = islandora_object_load($last_good_pid);

  // Validate.
  if (is_null($last_good_object)) {
    watchdog('UCLA Migration', 'Could not find last successfully ingested object with pid @pid', array('@pid' => $last_good_pid), WATCHDOG_ERROR);
    return FALSE;
  }

  // Get the parents of the last good object.
  $parents = islandora_get_parents_from_rels_ext($last_good_object);

  // Validate.
  if (empty($parents)) {
    watchdog('UCLA Migration', 'Could not find parents of @pid, the last successfully ingested object.', array('@pid' => $last_good_pid), WATCHDOG_ERROR);
    return FALSE;
  }

  if (count($parents) > 1) {
    watchdog('UCLA Migration', 'Found more than one parent for @pid, the last successfully ingested object.', array('@pid' => $last_good_pid), WATCHDOG_ERROR);
    return FALSE;
  }

  // Unbox the parent.
  $parent = $parents[0];

  // Migrate all the children if the parent id is the root collection.  This
  // would indicate that only the collection object has been ingested, so
  // basically start over.
  if ($parent->id == "islandora:root") {
    return ucla_migration_migrate_all_children($fedora_namespace, $mets_xml, $parent);
  }
  // Otherwise recover starting with the object after the last successfully
  // ingested object.
  else {
    $starting_pid = $fedora_namespace . ":" . ($recovery_number + 1);
    return ucla_migration_migrate_children_after($fedora_namespace, $mets_xml, $parent, $last_good_object, $starting_pid);
  }
}

/**
 * Migrates a single object based on record ID.
 *
 * @param string $file_path
 *   The path to the mets file serving as the manifest for this ingest.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace to ingest everything under.
 *
 * @param string $dmdid
 *   The record ID provided in the MODS for the record to migrate.
 */
function ucla_migration_migrate_from_dmdid($file_path, $fedora_namespace, $dmdid) {
  // Check to make sure the path to the mets file has been provided.
  if (!isset($file_path)) {
    drush_set_error('File path not provided', 'Path to mets file not provided as command line argument', 'Error: ');
    return;
  }

  // Attempt to grab the mets file.
  $mets_file = file_get_contents($file_path);
  if (!$mets_file) {
    drush_set_error('File not found', "Could not locate mets file $file_path", "Error: ");
    return;
  }

  $mets_xml = simplexml_load_string($mets_file);

  // Register namespaces of the root element.
  $xml_namespaces = $mets_xml->getDocNamespaces();
  $mets_xml->registerXPathNamespace('mets', $xml_namespaces['mets']);
  $mets_xml->registerXPathNamespace('xlink', $xml_namespaces['xlink']);

  // Get the collection object.
  // Probably a bad assumption, but it'll work for now.
  $collection = islandora_object_load($fedora_namespace . ":1");

  if (!$collection) {
    drush_set_error('No Collection Object', 'Could not load collection object.  Perhaps the fedora namespace is wrong, or the collection object is not the first object in the namespace.', 'Error: ');
    return;
  }

  // Query for the tiff file.
  $dmdid = "DMD-" . $dmdid;
  $image_query = "//mets:div[@TYPE='Image' and @DMDID='$dmdid']";
  $image_array = $mets_xml->xpath($image_query);

  // Validate results.
  if (!is_array($image_array)) {
    drush_set_error('XPath Error', "Error in XPath query $image_query", "Error: ");
    return;
  }

  if (empty($image_array)) {
    drush_set_error('XPath Error', "No results for XPath query $image_query", "Error: ");
    return;
  }

  if (count($image_array) > 1) {
    drush_set_error('XPath Error', "Multiple results for XPath query $image_query", "Error: ");
    return;
  }

  $results = ucla_migration_migrate_child_object($fedora_namespace, $image_array[0], $mets_xml, $collection);

  // Final message to user.
  if (!$results) {
    drush_set_error('Migration error', "Error migrating object from dmdid $dmdid", "Error: ");
    return;
  }

  drush_log("Migration success.  Check watchdog logs for more information.", 'ok');

  return;
}

/**
 * Migrates the main collection object for the provided mets xml.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace to ingest everything under.
 *
 * @param SimpleXMLElement $mets_xml
 *   SimpleXML object that represents the mets file from the path provided to
 *   the drush script.
 *
 * @return NewFedoraObject
 *   The newly ingested collection object.  Returns FALSE if there is an error
 *   at any point in the process.
 */
function ucla_migration_migrate_collection_object($fedora_namespace, $mets_xml) {
  // Query for id.
  $collection_id_section_query = "//mets:div[@TYPE='Collection']";
  $collection_id_section_array = $mets_xml->xpath($collection_id_section_query);

  // Validate results.
  if (!is_array($collection_id_section_array)) {
    watchdog('UCLA Migration', "Error in XPath query @query", array('@query' => $collection_id_section_query), WATCHDOG_ERROR);
    return FALSE;
  }

  if (empty($collection_id_section_array)) {
    watchdog('UCLA Migration', "No results for XPath query @query", array('@query' => $collection_id_section_query), WATCHDOG_ERROR);
    return FALSE;
  }

  if (count($collection_id_section_array) > 1) {
    watchdog('UCLA Migration', "More than one result for XPath query @query", array('@query' => $collection_id_section_query), WATCHDOG_ERROR);
    return FALSE;
  }

  // Actually get the id.
  $collection_id = $collection_id_section_array[0]['DMDID'];

  // Query for the collection based on id.
  $collection_section_query = "//mets:dmdSec[@ID='$collection_id']";
  $collection_section_array = $mets_xml->xpath($collection_section_query);

  // Validate results.
  if (!is_array($collection_id_section_array)) {
    watchdog('UCLA Migration', "Error in XPath query @query", array('@query' => $collection_section_query), WATCHDOG_ERROR);
    return FALSE;
  }

  if (empty($collection_id_section_array)) {
    watchdog('UCLA Migration', "No results for XPath query @query", array('@query' => $collection_section_query), WATCHDOG_ERROR);
    return FALSE;
  }

  if (count($collection_id_section_array) > 1) {
    watchdog('UCLA Migration', "More than one result for XPath query @query", array('@query' => $collection_section_query), WATCHDOG_ERROR);
    return FALSE;
  }

  // Get the children elements of the collection section that have a mets
  // namespace.
  $xml_namespaces = $mets_xml->getDocNamespaces();
  $collection_section = $collection_section_array[0]->children($xml_namespaces['mets']);

  // Validate.
  if ($collection_section->count() == 0) {
    watchdog('UCLA Migration', "Could not locate mdWrap element for @id in the METS.", array('@id' => $collection_id), WATCHDOG_ERROR);
    return FALSE;
  }

  if ($collection_section->count() > 1) {
    watchdog('UCLA Migration', "Multiple mdWrap elements for @id in the METS.", array('@id' => $collection_id), WATCHDOG_ERROR);
    return FALSE;
  }

  // Grab the collection label.
  $collection_label = $collection_section->mdWrap->attributes()->LABEL;

  // Get the children elements of the collection section that have a mods
  // namespace.  We didn't register this namespace because it's not in the root
  // element, so we just use it manually.
  $collection_mods = $collection_section->mdWrap->xmlData->children('http://www.loc.gov/mods/v3');

  // Validate.
  if ($collection_mods->count() == 0) {
    watchdog('UCLA Migration', "Could not locate mods element for @id in the METS.", array('@id' => $collection_id), WATCHDOG_ERROR);
    return FALSE;
  }

  if ($collection_mods->count() > 1) {
    watchdog('UCLA Migration', "Multiple mods elements for @id in the METS.", array('@id' => $collection_id), WATCHDOG_ERROR);
    return FALSE;
  }

  // Grab the mods string.
  $mods_string = $collection_mods->mods->asXML();
  // Clean it up by adding the xsi namespace so SimpleXML and DOMDocument don't
  // cry every time something is done with the MODS.  The namespaces is in the
  // root node of the mets, but not the root node of the MODS.  So we'll just
  // add it in here for completeness.
  $mods_string = ucla_migration_add_xsi_namespace_to_mods_txt($mods_string);

  // Create the NewFedoraObject for the collection.
  $collection = ucla_migration_create_collection_object(
      $fedora_namespace, $collection_label, $mods_string);

  // Ingest the NewFedoraObject.
  $ingest_results = islandora_add_object($collection);

  // Validate.
  if (!$ingest_results) {
    watchdog('UCLA Migration', "Ingest for the collection object has failed.", array(), WATCHDOG_ERROR);
    return FALSE;
  }

  watchdog('UCLA Migration', "Ingested @id.", array('@id' => $collection->id), WATCHDOG_INFO);

  // Return the ingested object.
  return $ingest_results;
}

/**
 * Migrates all children of the collection object in the provided mets xml.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace to ingest everything under.
 *
 * @param SimpleXMLElement $mets_xml
 *   SimpleXML object that represents the mets file from the path provided to
 *   the drush script.
 *
 * @param NewFedoraObject $collection
 *   The collection this child will be a member of.
 *
 * @return bool
 *   Returns TRUE if succesful, FALSE if error.
 */
function ucla_migration_migrate_all_children($fedora_namespace, $mets_xml, $collection) {
  // Get all the children objects.  Each one will correspond to a tiff file.
  $image_array = ucla_migration_get_all_children_xmls($mets_xml);

  // Exit early if there are no children.
  if (empty($image_array)) {
    return FALSE;
  }

  // Iterate over the query results, making and ingesting a child object for
  // each tiff.
  $ingest_success = TRUE;
  for ($i = 0; $i < count($image_array) && $ingest_success; $i++) {
    $ingest_success = ucla_migration_migrate_child_object($fedora_namespace, $image_array[$i], $mets_xml, $collection);
    if ($ingest_success) {
      watchdog('UCLA Migration', 'Migrated object @idx out of @total', array('@idx' => $i + 1, '@total' => count($image_array)), WATCHDOG_INFO);
    }
  }

  // Exit if ingest of any child object failed.
  if (!$ingest_success) {
    watchdog('UCLA Migration', 'Child object ingest failed.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  return TRUE;
}

/**
 * Migrates all children after the supplied object in mets xml.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace to ingest everything under.
 *
 * @param SimpleXMLElement $mets_xml
 *   SimpleXML object that represents the mets file from the path provided to
 *   the drush script.
 *
 * @param NewFedoraObject $collection
 *   The collection this child will be a member of.
 *
 * @param NewFedoraObject $last_good_object
 *   The last successfully ingested object from the previous migration attempt.
 *
 * @return bool
 *   Returns TRUE if successful, FALSE if there is an error.
 */
function ucla_migration_migrate_children_after($fedora_namespace, $mets_xml, $collection, $last_good_object, $starting_pid) {
  // Try to delete the object AFTER the last successfully ingested object.  If
  // it exists, it's defnitely broken, so let's get rid of it before restarting
  // the migration.
  $starting_object = islandora_object_load($starting_pid);

  if (isset($starting_object) && $starting_object) {
    islandora_delete_object($starting_object);
  }

  // Get the last successfully ingested object's id (not PID, this comes from
  // the MODS).
  $last_good_record_id = ucla_migration_get_identifier_from_object_mods($last_good_object);

  if (!$last_good_record_id) {
    return FALSE;
  }

  // Get all the children objects' mets records.
  $image_array = ucla_migration_get_all_children_xmls($mets_xml);

  if (!$image_array) {
    return FALSE;
  }

  // Cycle through until you find the last successfully ingested object.
  $found = FALSE;
  $i = 0;
  for ($i; $i < count($image_array) && !$found; $i++) {
    $image_array_id = $image_array[$i]->attributes()->DMDID;
    if ($image_array[$i]->attributes()->DMDID == "DMD-$last_good_record_id") {
      $found = TRUE;
    }
  }

  // Start one record after the last successfully ingested object.
  $ingest_success = TRUE;
  for ($i; $i < count($image_array) && $ingest_success; $i++) {
    $ingest_success = ucla_migration_migrate_child_object($fedora_namespace, $image_array[$i], $mets_xml, $collection);
    if ($ingest_success) {
      watchdog('UCLA Migration', 'Migrated object @idx / @total', array('@idx' => $i + 1, '@total' => count($image_array)), WATCHDOG_INFO);
    }
  }

  // Exit if ingest of any child object failed.
  if (!$ingest_success) {
    watchdog('UCLA Migration', 'Child object ingest failed.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  return TRUE;
}

/**
 * Gets all of the TYPE='Image' divs from the mets xml.
 *
 * Each record in the results corresponds to a child object in the collection
 * that is being migrated.
 *
 * @param SimpleXMLElement $mets_xml
 *   The SimpleXMLElement representing the root node of the mets file.
 *
 * @return array
 *   All divs corresponding to children objects of the collection being
 *   migrated.  Returns FALSE if there's an error.
 */
function ucla_migration_get_all_children_xmls($mets_xml) {
  // Query for each of the tiff files.
  // Each one will correspond to a child of the recently created collection.
  $image_query = "//mets:div[@TYPE='Image']";
  $image_array = $mets_xml->xpath($image_query);

  // Validate results.
  if (!is_array($image_array)) {
    watchdog('UCLA Migration', "Error in XPath query @query", array('@query' => $image_query), WATCHDOG_ERROR);
    return FALSE;
  }

  if (empty($image_array)) {
    watchdog('UCLA Migration', "No results for XPath query @query", array('@query' => $image_query), WATCHDOG_ERROR);
    return FALSE;
  }

  return $image_array;
}

/**
 * Grabs the record identifier from an object's MODS datastream.
 *
 * @param NewFedoraObject $object
 *   The objects whose record identifier you are looking for.
 *
 * @return string
 *   The record identifier from this object's MODS.  Returns FALSE if there's
 *   an error.
 */
function ucla_migration_get_identifier_from_object_mods($object) {
  // Exit early if there's no MODS datastream.
  if (!(isset($object['MODS']) && !empty($object['MODS']))) {
    watchdog('UCLA Migration', "No MODS for object @pid", array('@pid' => $object->id), WATCHDOG_ERROR);
    return FALSE;
  }

  // Make a SimpleXMLElement for the MODS.
  $mods_txt = $object['MODS']->content;
  $mods = simplexml_load_string($mods_txt);

  // Register namespaces of the root element.
  $xml_namespaces = $mods->getDocNamespaces();
  $mods->registerXPathNamespace('mods', $xml_namespaces['mods']);

  // Query for the recordInfo div.
  $id_query = "//mods:mods/mods:recordInfo";
  $id_array = $mods->xpath($id_query);

  // Validate results.
  if (!is_array($id_array)) {
    watchdog('UCLA Migration', "Error in XPath query @query", array('@query' => $id_query), WATCHDOG_ERROR);
    return FALSE;
  }

  if (empty($id_array)) {
    watchdog('UCLA Migration', "No results for XPath query @query", array('@query' => $id_query), WATCHDOG_ERROR);
    return FALSE;
  }

  if (count($id_array) > 1) {
    watchdog('UCLA Migration', "More than one result for XPath query @query", array('@query' => $id_query), WATCHDOG_ERROR);
    return FALSE;
  }

  // Grab the children of the recordInfo div.  THERE CAN BE ONLY ONE!!!
  $children = $id_array[0]->children($xml_namespaces['mods']);

  if ($children->count() == 0) {
    watchdog('UCLA Migration', "The recordInfo div has no children.", array(), WATCHDOG_ERROR);
    return FALSE;
  }

  if ($children->count() > 1) {
    watchdog('UCLA Migration', "The recordInfo div has too many children.", array(), WATCHDOG_ERROR);
    return FALSE;
  }

  return $children->recordIdentifier;
}

/**
 * Migrates a child object of the collection object in the provided mets xml.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace to ingest everything under.
 *
 * @param SimpleXMLElement $image_xml
 *   SimpleXML object that represents the mets div containing info about the
 *   tiff to migrate.
 *
 * @param SimpleXMLElement $mets_xml
 *   SimpleXML object that represents the mets file from the path provided to
 *   the drush script.
 *
 * @param NewFedoraObject $collection
 *   The collection this child will be a member of.
 *
 * @return NewFedoraObject
 *   The newly ingested child object.  Returns FALSE if there is an error
 *   at any point in the process.
 */
function ucla_migration_migrate_child_object($fedora_namespace, $image_xml, $mets_xml, $collection) {
  $dmdid = $image_xml->attributes()->DMDID;
  $label = $image_xml->attributes()->LABEL;

  // Get the children elements of the collection section that have a mets
  // namespace.  This should correspond to a single mets:fptr element.
  $xml_namespaces = $mets_xml->getDocNamespaces();
  $fptr_section = $image_xml->children($xml_namespaces['mets']);

  // Validate.
  if ($fptr_section->count() == 0) {
    watchdog('UCLA Migration', "Could not locate fptr element for @id in the METS.", array('@id' => (string) $dmdid), WATCHDOG_ERROR);
    return FALSE;
  }

  if ($fptr_section->count() > 1) {
    watchdog('UCLA Migration', "Multiple fptr elements for @id in the METS.", array('@id' => (string) $dmdid), WATCHDOG_ERROR);
    return FALSE;
  }

  // Grab the file id from the ftpr element.
  $file_id = $fptr_section->attributes()->FILEID;

  // Use the file id to lookup the file path.
  $file_path_query = "//mets:file[@ID='$file_id']/mets:FLocat";
  $file_path_array = $mets_xml->xpath($file_path_query);

  // Validate.
  if (!is_array($file_path_array)) {
    watchdog('UCLA Migration', "Error in XPath query @query", array('@query' => $file_path_query), WATCHDOG_ERROR);
    return FALSE;
  }

  if (empty($file_path_array)) {
    watchdog('UCLA Migration', "No results for XPath query @query", array('@query' => $file_path_query), WATCHDOG_ERROR);
    return FALSE;
  }

  if (count($file_path_array) > 1) {
    watchdog('UCLA Migration', "More than one result for XPath query @query", array('@query' => $file_path_query), WATCHDOG_ERROR);
    return FALSE;
  }

  // Grab the file path from the XPath results.
  $file_path = $file_path_array[0]->attributes($xml_namespaces['xlink'])->href;

  // Use the DMDID to lookup the MODS record for this image.
  $mods_query = "//mets:dmdSec[@ID='$dmdid']/mets:mdWrap[@MDTYPE='MODS']/mets:xmlData";
  $mods_array = $mets_xml->xpath($mods_query);

  // Validate.
  if (!is_array($mods_array)) {
    watchdog('UCLA Migration', "Error in XPath query @query", array('@query' => $mods_query), WATCHDOG_ERROR);
    return FALSE;
  }

  if (empty($mods_array)) {
    watchdog('UCLA Migration', "No results for XPath query @query", array('@query' => $mods_query), WATCHDOG_ERROR);
    return FALSE;
  }

  if (count($mods_array) > 1) {
    watchdog('UCLA Migration', "More than one result for XPath query @query", array('@query' => $mods_query), WATCHDOG_ERROR);
    return FALSE;
  }

  // Attempt to to get the MODS element from the XPath results.
  $mods_element = $mods_array[0]->children('http://www.loc.gov/mods/v3');

  // Validate.
  if ($mods_element->count() == 0) {
    watchdog('UCLA Migration', "Could not locate mods element for @id in the METS.", array('@id' => (string) $dmdid), WATCHDOG_ERROR);
    return FALSE;
  }

  if ($mods_element->count() > 1) {
    watchdog('UCLA Migration', "Multiple mods elements for @id in the METS.", array('@id' => (string) $dmdid), WATCHDOG_ERROR);
    return FALSE;
  }

  // Grab the MODS record as a string.
  $mods_string = $mods_element->asXML();
  // Clean it up by adding the xsi namespace so SimpleXML and DOMDocument don't
  // cry every time something is done with the MODS.  The namespaces is in the
  // root node of the mets, but not the root node of the MODS.  So we'll just
  // add it in here for completeness.
  $mods_string = ucla_migration_add_xsi_namespace_to_mods_txt($mods_string);

  // Create the NewFedoraObject for this image.
  $child = ucla_migration_create_child_object($fedora_namespace, $label, $file_path, $mods_string, $collection);

  // Ingest the NewFedoraObject.
  $ingest_results = islandora_add_object($child);

  // Validate.
  if (!$ingest_results) {
    watchdog('UCLA Migration', "Ingest for the child object @id has failed.", array('@id' => (string) $dmdid), WATCHDOG_ERROR);
    return FALSE;
  }

  watchdog('UCLA Migration', 'Ingested @id as @pid', array('@id' => (string) $dmdid, '@pid' => $ingest_results->id), WATCHDOG_INFO);

  // Return the ingested object.
  return $ingest_results;
}

/**
 * Creates a NewFedoraObject for further processing and ingesting.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace to ingest everything under.
 *
 * @param string $label
 *   The label to give the NewFedoraObject
 *
 * @return NewFedoraObject
 *   The newly created object.
 */
function ucla_migration_create_fedora_object($fedora_namespace, $label) {
  global $user;
  $tuque = islandora_get_tuque_connection();
  $object = $tuque->repository->constructObject($fedora_namespace);
  $object->owner = isset($user->name) ? $user->name : $object->owner;
  $object->label = $label;
  return $object;
}

/**
 * Adds a MODS datastream to the supplied object from the supplied xml text.
 *
 * @param NewFedoraObject $object
 *   The object you wish to add the datastream to.
 *
 * @param string $mods_txt
 *   A string of valid MODS text.
 */
function ucla_migration_add_mods_to_object($object, $mods_txt) {
  $datastream = $object->constructDatastream('MODS', 'M');
  $datastream->label = "MODS Record";
  $datastream->mimetype = "text/xml";
  $datastream->setContentFromString($mods_txt);
  $object->ingestDatastream($datastream);
}

/**
 * Adds a DC datastream to the supplied object from the supplied MODS text.
 *
 * @param NewFedoraObject $object
 *   The object you wish to add the datastream to.
 *
 * @param string $mods_txt
 *   A string of valid MODS text.
 */
function ucla_migration_add_dc_to_object_from_mods($object, $mods_txt) {
  // Prepare the MODS to DC transform.
  $xslt_proc = new XSLTProcessor();
  $xslt_file = drupal_get_path('module', 'ucla_migration') . '/transforms/mods_to_dc.xsl';
  $xslt_dom = new DOMDocument();
  $xslt_dom->load($xslt_file);
  $xslt_proc->importStyleSheet($xslt_dom);

  // Prepare the mods DOM to transform.
  $mods_dom = new DOMDocument();
  $mods_dom->loadXML($mods_txt);

  // Transform the MODS to DC.
  $dc_txt = $xslt_proc->transformToXML($mods_dom);

  // Add DC record to the object as a datastream.
  $datastream = $object->constructDatastream('DC', 'M');
  $datastream->label = "Dublin Core Record";
  $datastream->mimetype = "text/xml";
  $datastream->setContentFromString($dc_txt);
  $object->ingestDatastream($datastream);
}

/**
 * Adds a namespace to the MODS text so SimpleXML won't cry.
 *
 * @param string $mods_txt
 *   Valid string of XML MODS that requires the extra namespace.
 *
 * @return string
 *   The patched up MODS text.
 */
function ucla_migration_add_xsi_namespace_to_mods_txt($mods_txt) {
  return str_replace("xmlns:mods=\"http://www.loc.gov/mods/v3\"", "xmlns:mods=\"http://www.loc.gov/mods/v3\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"", $mods_txt);
}

/**
 * Adds a tiff (from path) to the supplied object as the OBJ datastream.
 *
 * @param NewFedoraObject $object
 *   The object you wish to add the datastream to.
 *
 * @param string $tiff_path
 *   Valid path to a tiff file.
 */
function ucla_migration_add_tiff_to_object($object, $tiff_path) {
  $datastream = $object->constructDatastream('OBJ', 'M');
  $datastream->label = "High Quality Tiff";
  $datastream->mimetype = "image/tiff";
  $datastream->setContentFromFile($tiff_path);
  $object->ingestDatastream($datastream);
}

/**
 * Creates the collection object for this migration.
 *
 * Adds relationships and content models for the collection, as well as a MODS
 * datastream.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace to ingest everything under.
 *
 * @param string $label
 *   The label to give the NewFedoraObject
 *
 * @param string $mods_txt
 *   A string of valid MODS text.
 *
 * @return NewFedoraObject
 *   The fully constructed and setup collection object.
 */
function ucla_migration_create_collection_object($fedora_namespace, $label, $mods_txt) {
  $collection = ucla_migration_create_fedora_object($fedora_namespace, $label);
  $collection->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:collectionCModel');
  $collection->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', 'islandora:root');
  ucla_migration_add_mods_to_object($collection, $mods_txt);
  ucla_Migration_add_dc_to_object_from_mods($collection, $mods_txt);
  return $collection;
}

/**
 * Creates a child object for the collection in this migration.
 *
 * Adds relationships and content models for the child, as well as MODS and OBJ
 * datastreams.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace to ingest everything under.
 *
 * @param string $label
 *   The label to give the NewFedoraObject
 *
 * @param string $tiff_path
 *   Path to a high-res tiff for the OBJ datastream.
 *
 * @param string $mods_txt
 *   A string of valid MODS text.
 *
 * @param NewFedoraObject $collection
 *   The collection this child will be a member of.
 *
 * @return NewFedoraObject
 *   The fully constructed and setup collection object.
 */
function ucla_migration_create_child_object($fedora_namespace, $label, $tiff_path, $mods_txt, $collection) {
  $child = ucla_migration_create_fedora_object($fedora_namespace, $label);
  $child->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:sp_large_image_cmodel');
  $child->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection->id);
  ucla_migration_add_mods_to_object($child, $mods_txt);
  ucla_Migration_add_dc_to_object_from_mods($child, $mods_txt);
  ucla_migration_add_tiff_to_object($child, $tiff_path);
  return $child;
}

function ucla_migration_create_id_to_pid_map($fedora_namespace, $datastream_id, $xpath_query) {
  $remote_id_to_fedora_pid = array();
  $pids = ucla_migration_get_pids_for_namespace($fedora_namespace);

  foreach ($pids as $pid) {
    $query_result = ucla_migration_get_value_from_datastream($pid, $datastream_id, $xpath_query);
    if ($query_result) {
      $identifier = $query_result[0];
      $remote_id_to_fedora_pid["$identifier"] = $pid;
    }
  }

  return $remote_id_to_fedora_pid;
}

function ucla_migration_get_value_from_datastream($pid, $datastream_id, $xpath_query) {
  $object = islandora_object_load($pid);
  if ($object[$datastream_id]) {

    libxml_use_internal_errors(true);

    $xml_txt = $object[$datastream_id]->content;
    $xml_obj = simplexml_load_string($xml_txt);

    if (!$xml_obj !== false) {
      $xml_namespaces = $xml_obj->getDocNamespaces();
      $xml_obj->registerXPathNamespace('mods', $xml_namespaces['mods']);
      $xpath_results = $xml_obj->xpath($xpath_query);
      return $xpath_results;
    }
    else {
      foreach (libxml_get_errors() as $error) {
        drush_print('ERROR XML PARSE for pid ' . $pid . ' Message:' . $error->message);
      }
    }

    libxml_clear_errors();
    return $xpath_results;
  }
}

/**
 * 
 * Updates a specific datastream rather than importing all digital assets
 * associated with an item. This particular use case updates a TRANSLATION
 * datastream source
 * 
 * 
 * @param type $mets_file_path
 *  The path to the METS file containing both asset and metadata. This can be a
 *  remote url or a local file
 * 
 * @param type $datastream_path
 * The path to the root folder of the datastreams to be updated, translation 
 * files
 * 
 * @param type $fedora_namespace
 *  The Fedora Namespace you want to update the datastream for. Example:
 *  Supplying com.example.namespace, will examine each item in this namespace
 *  and look for a related datastream to attach
 * 
 * @return type
 * 
 */
function ucla_migration_migrate_datastream($mets_file_path, $datastream_path, $fedora_namespace) {


// Check to make sure the path to the mets file has been provided.
  if (!isset($mets_file_path)) {
    drush_set_error('File path not provided', 'Path to mets file not provided as command line argument', 'Error: ');
    return;
  }

  // Attempt to grab the mets file.
  $mets_file = file_get_contents($mets_file_path);
  if (!$mets_file) {
    drush_set_error('File not found', "Could not locate mets file $mets_file_path", "Error: ");
    return;
  }

  $datastream_to_update = 'MODS';
  $xpath_query = '//mods:mods/mods:recordInfo/mods:recordIdentifier';
  $pid_map = ucla_migration_create_id_to_pid_map($fedora_namespace, $datastream_to_update, $xpath_query);


  $mets_xml = ucla_migration_mets_file_to_xml_object($mets_file);
  $mods_query = "//mets:dmdSec/mets:mdWrap/mets:xmlData";
  $mods_array = $mets_xml->xpath($mods_query);

  // For each MODS record:
  // - Extract resourceIdentifier (the ark)
  // - Compare ark to the pid-ark map, to get the pid of related Fedora object
  // - With the correct pid we can now update the datastream

  for ($i = 0; $i < sizeof($mods_array); $i++) {
    if (ucla_evaluate_mods_record_to_ingest($mods_array[$i], $pid_map)) {
      $ark_id = ucla_migration_get_ark_id_from_mods($mods_array[$i]);
      $fedora_pid = $pid_map[$ark_id];

      // The translation files have been named with 2nd half of ark
      $file_root = substr($ark_id, strrpos($ark_id, '-') + 1);
      $translation_filename = $datastream_path . '/' . $file_root . '.txt';

      $translation_file = @file_get_contents($translation_filename);

      if ($translation_file) {
        $translation_file = mb_convert_encoding($translation_file, "UTF-8");
        ucla_migration_add_datastream_to_object($fedora_pid, "TRANSLATION-EN", "text/plain", $translation_file);
      }
    }
  }
}

function ucla_migration_get_ark_id_from_mods($mods_simple_xml) {

  $mods_record = $mods_simple_xml->children('http://www.loc.gov/mods/v3');
  $mods_string = $mods_record->asXML();
  $mods_string = ucla_migration_add_xsi_namespace_to_mods_txt($mods_string);
  $mods_object = simplexml_load_string($mods_string);

  $id_query = "//mods:mods/mods:recordInfo";
  $id_array = $mods_object->xpath($id_query);
  $children = $id_array[0]->children('http://www.loc.gov/mods/v3');

  $ark_identifier = (string) $children->recordIdentifier;

  return $ark_identifier;
}

function ucla_evaluate_mods_record_to_ingest($mods_simple_xml, $pid_ark_map) {
  $ark_identifier = ucla_migration_get_ark_id_from_mods($mods_simple_xml);
  if (array_key_exists($ark_identifier, $pid_ark_map)) {
    return TRUE;
  }

  return FALSE;
}

function ucla_migration_get_pids_for_namespace($fedora_namespace) {
  // Get a connection to Fedora.
  $connection = islandora_get_tuque_connection();
  $repo = $connection->repository;

  // SPARQL query to get all objects in a given namespace.
  $query =
      "
    SELECT ?obj
    FROM <#ri>
    WHERE {
      ?obj <info:fedora/fedora-system:def/model#hasModel> ?model .
      FILTER(regex(str(?obj), '^info:fedora/$fedora_namespace:'))
    }
  ";

  // Execute the query.
  $results = $repo->ri->sparqlQuery($query);

  // Put all the pids in an array.
  $pids = array();
  foreach ($results as $result) {
    array_push($pids, $result['obj']['value']);
  }

  return $pids;
}

function ucla_migration_mets_file_to_xml_object($mets_file) {
  $mets_xml = simplexml_load_string($mets_file);

  // Register namespaces of the root element.
  $xml_namespaces = $mets_xml->getDocNamespaces();
  $mets_xml->registerXPathNamespace('mets', $xml_namespaces['mets']);
  $mets_xml->registerXPathNamespace('xlink', $xml_namespaces['xlink']);

  return $mets_xml;
}

function ucla_migration_add_datastream_to_object($fedora_pid, $datastream_id, $mime_type, $datastream_content, $content_from_file = 0) {
  $fedora_object = islandora_object_load($fedora_pid);

  if (!is_null($fedora_object)) {
    $fedora_object->purgeDatastream("$datastream_id");

    $datastream = $fedora_object->constructDatastream($datastream_id, 'M');
    $datastream->label = "$datastream_id";
    $datastream->mimetype = "$mime_type";

    if ($content_from_file != 0) {
      $datastream->setContentFromFile($datastream_content);
    }
    else {
      $datastream->setContentFromString($datastream_content);
    }

    $fedora_object->ingestDatastream($datastream);
  }
}

/**
 * 
 * Updates a specific datastream rather than importing all digital assets
 * associated with an item. The entries in the $datastream_path file should
 * contain a path to the digital asset to add. This filename will contain the
 * item ark, which is used to match the digital asset to the Fedora object to
 * add the content to.
 * 
 * 
 * @param type $datastream_path
 *  The path to the plain text file containing a path to the digital asset to be
 *  loaded. One path per line.
 * 
 * @param type $datastream_id
 * The datastream Id to attach to this content when adding to the Fedora object,
 * this will also be used for the label value.
 * 
 * @param type $mime_type
 * This is the mime type to attach to the data stream
 * 
 * @param type $fedora_namespace
 *  The Fedora Namespace you want to update the datastream for. Example:
 *  Supplying com.example.namespace, will examine each item in this namespace
 *  and look for a related datastream to attach
 * 
 * @return type
 * 
 */
function ucla_migration_attach_datastream($datastream_path, $datastream_id, $mime_type, $fedora_namespace) {

  if (!isset($datastream_path)) {
    drush_set_error('File path not provided', 'A path to the file containing the assets to load must be added', 'Error: ');
    return;
  }

  if (!isset($datastream_id)) {
    drush_set_error('Datastream Id not provided', 'A datastream id to apply to the asset is required', 'Error: ');
    return;
  }

  if (!isset($mime_type)) {
    drush_set_error('Mime type not provided', 'A Mime type for this added asset is required', 'Error: ');
    return;
  }

  if (!isset($fedora_namespace)) {
    drush_set_error('Namespace not provided', 'A namespace to match the assets to is required', 'Error: ');
    return;
  }


  // Attempt to grab the file containing list of content locations
  $datastream_asset_file = file($datastream_path);

  if (!$datastream_asset_file) {
    drush_set_error('File not found', "Could not locate mets file $datastream_path", "Error: ");
    return;
  }

  // Use the MODS datastream with a pointer to the recordIdentifer as the source
  // to generate the ark to pid mapping

  $datastream_containing_id = 'MODS';
  $xpath_query = '//mods:mods/mods:recordInfo/mods:recordIdentifier';
  $pid_map = ucla_migration_create_id_to_pid_map($fedora_namespace, $datastream_containing_id, $xpath_query);

  for ($i = 0; $i < sizeof($datastream_asset_file); $i++) {

    $file_to_get = chop($datastream_asset_file[$i]);
    $remote_ark = ucla_migration_get_ark_from_filename($file_to_get);
    $fedora_pid = $pid_map[$remote_ark];

    if (isset($fedora_pid)) {
      $file_to_attach = file_get_contents($file_to_get);

      if ($file_to_attach) {
        ucla_migration_add_datastream_to_object($fedora_pid, $datastream_id, $mime_type, $file_to_get, 1);

        $fedora_object = islandora_object_load($fedora_pid);
        if (!is_null($fedora_object)) {

          // Should abstract this out somehow for the user, all derivative ds
          // must be removed before re-generated. If not removed, the function 
          // does NOT overwrite the datastreams

          $fedora_object->purgeDatastream("TN");
          $fedora_object->purgeDatastream("PREVIEW");

          islandora_pdf_islandora_sp_pdf_islandora_object_ingested($fedora_object);
        }
      }
    }
  }
}

function ucla_migration_get_ark_from_filename($filename) {
  $ucla_naan = "21198";

  // Extract ark from filename, assuming naming convention of DLCS
  $ark_start_pos = strpos($filename, $ucla_naan);
  $ucla_naan_end_pos = $ark_start_pos + 6;
  $ark_end_pos = strpos($filename, "-", $ucla_naan_end_pos);
  $ark_value = substr($filename, $ark_start_pos, $ark_end_pos - $ark_start_pos);

  return $ark_value;
}

/**
 * 
 *  Given Datastream ID, namespace, and range of numbers, delete the datastream
 * `from these Fedora objects
 *  Example input: DS_TO_DELETE, my.object.namespace, 1,2-5,10-12
 *  Return array: 1,2,3,4,5,10,11,12
 * 
 *  The options are not passed as function parameters, but rather as drush
 *  command line options
 * 
 * @param type $datastream_id
 *  The datastream id of the dsid you wish to delete from the objects
 * 
 * @param type $fedora_namespace
 *  The namespace base that will be used, along with the number range, to
 *  generate the pids to remove
 * 
 * @param type $pid_numbers
 *  A string containing single numeric values and ranges of values, separated
 *  by a comma. Ranges are designated by a dash '-'
 * 
 * 
 * @return type 
 * 
 */
function ucla_migration_delete_datastream() {

  $dsid = drush_get_option('datastream_id');
  $fedora_namespace = drush_get_option('fedora_namespace');
  $pid_range = drush_get_option('pid_numbers');

  $parse_pids = _parse_number_range($pid_range);
  $delete_count = 0;

  foreach ($parse_pids as $parse_pid) {
    $full_name = $fedora_namespace . ":" . $parse_pid;
    $fedora_object = islandora_object_load($full_name);

    if ($fedora_object) {
      $result_code = $fedora_object->purgeDatastream($dsid);
      $result_text = 'Failure';

      if ($result_code == 1) {
        $result_text = 'Success';
        $delete_count++;
      }
      drush_log("Delete {$dsid} datastream from {$full_name} - Result : {$result_text}", 'ok');
    }
  }
  drush_log("Deleted {$dsid} from {$delete_count} total objects", 'ok');
}

/**
 * 
 *  Given a string value representing ranges and single numbers, extracts and
 *  converts the input into a single array containing unique values.
 *  Example input: 1,2-5,10-12
 *  Return array: 1,2,3,4,5,10,11,12
 * 
 * 
 * @param type $range_values
 *  A string containing single numeric values and ranges of values, separated
 *  by a comma. Ranges are designated by a dash '-'
 * 
 * 
 * @return type $pid_numbers
 *  An array containing a unique list of values extracted from the input range
 * 
 */
function _parse_number_range($range_values) {

  $number_chunks = split(",", $range_values);
  $pid_numbers = array();

  foreach ($number_chunks as $number_chunk) {
    $range_value = split("-", $number_chunk);

    if (sizeof($range_value) == 1) {
      if (is_numeric($range_value[0])) {
        array_push($pid_numbers, $range_value[0]);
      }
    }

    if (sizeof($range_value) == 2) {
      if (is_numeric($range_value[0]) && is_numeric($range_value[1])) {
        foreach (range($range_value[0], $range_value[1]) as $val) {
          array_push($pid_numbers, $val);
        }
      }
    }
  }
  $pid_numbers = array_unique($pid_numbers);

  return $pid_numbers;
}

/**
 * 
 *  A function to provide minimum metadata data for ingest of large numbers of 
 *  assets. Content type will be assumed based on the extension
 * 
 *  TIF - Will use a large image content model
 *  PDF - Will use the basic PDF content model
 * 
 *  The metadata attached to the content will be a MODS record containing the 
 *  following values
 * 
 *  title : The filename of the file ingested
 *  identifier : The filename of the file ingested  
 * 
 *  Both these values will be crosswalked to DC and available via triplestore 
 *  query
 * 
 * @param type $fedora_namespace
 *  The namespace in Fedora to relate as the collection parent for the minimal
 *  import
 * 
 * @param type $manifest_file_path
 *  A path to a file containing a file path to a single asset on each line
 * 
 * @param type $overwrite
 *  If true, remove existing Fedora objects in this namespace with the same
 * filename
 * 
 * 
 */
function minimum_metadata_ingest() {

  $fedora_namespace = drush_get_option('fedora_namespace');
  $manifest_file_path = drush_get_option('manifest_file_path');
  $overwrite = drush_get_option('overwrite');


  $manifest_file = file($manifest_file_path);
  if (!$manifest_file) {
    drush_set_error('File not found', "Could not locate manifest file file $manifest_file_path", "Error: ");
    return;
  }

  if (!isset($fedora_namespace)) {
    drush_set_error('Fedora namespace not provided', 'A Fedora namespace to associate with the ingest must be provided', 'Error: ');
    return;
  }

  if (!isset($overwrite)) {
    drush_set_error('Overwrite flag not specified', '', 'Error: ');
    return;
  }


  $collection = islandora_object_load($fedora_namespace . ":1");

  foreach ($manifest_file as $file_path) {
    
    $file_path = trim($file_path);
    
    if (_is_valid_extension($file_path)) {
      
      $file_path = _url_encode_http_address($file_path);
      
      if (fopen($file_path, "r")) {

        $file_name = basename($file_path);

        $fedora_object = ucla_migration_create_fedora_object($fedora_namespace, $file_name);
        $fedora_object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection->id);


        _add_datastream_to_object($fedora_object, $file_path);
        $ingest_results = islandora_add_object($fedora_object);


        watchdog('Ingest Success', 'Migrated object @file_name', array('@file_name' => $file_name), WATCHDOG_INFO);
      }
      else {
        watchdog('Ingest Error', 'Child object ingest failed.', array(), WATCHDOG_ERROR);
      }
    }
  }
}

/**
 * Determines datastream format based on Mime type and calls the appropriate 
 * function to attach the content
 *
 * @param FedoraObject $object
 *   The object you wish to add the datastream to.
 *
 * @param string $file_path
 *   Valid path to a file to be added as a datastream.
 */
function _add_datastream_to_object($object, $file_path) {

  $ext = strtolower(pathinfo($file_path, PATHINFO_EXTENSION));

  if ($ext === 'tif') {
    _add_tiff_to_object($object, $file_path);
  }

  if ($ext === 'tiff') {
    _add_tiff_to_object($object, $file_path);
  }

  if ($ext === 'pdf') {
    _add_pdf_to_object($object, $file_path);
  }

  return $object;
}

/**
 * Based on the extension of a file, determine if this is a valid file to be
 * ingested
 *
 * @param string $file_path
 *   Path to a file to test extension
 */
function _is_valid_extension($file_path) {

  $ext = strtolower(pathinfo($file_path, PATHINFO_EXTENSION));
  
  if ($ext === 'tif') {
    return true;
  }
  
  if ($ext === 'tiff') {
    return true;
  }

  if ($ext === 'pdf') {
    return true;
  }

  return false;
}

/**
 * Adds a tiff to the supplied object as the OBJ datastream.
 *
 * @param FedoraObject $object
 *   The object you wish to add the datastream to.
 *
 * @param string $tiff_path
 *   Valid path to a tiff file.
 */
function _add_tiff_to_object($object, $tiff_path) {
  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:sp_large_image_cmodel');
  $datastream = $object->constructDatastream('OBJ', 'M');
  $datastream->label = "High Quality Tiff";
  $datastream->mimetype = "image/tiff";
  $datastream->setContentFromFile($tiff_path);
  $object->ingestDatastream($datastream);
}

/**
 * Adds a PDF to the supplied object as the OBJ datastream.
 *
 * @param FedoraObject $object
 *   The object you wish to add the datastream to.
 *
 * @param string $pdf_path
 *   Valid path to a pdf file.
 */
function _add_pdf_to_object($object, $pdf_path) {
  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:sp_pdf');
  $datastream = $object->constructDatastream('OBJ', 'M');
  $datastream->label = "PDF Document";
  $datastream->mimetype = "application/pdf";
  $datastream->setContentFromFile($pdf_path);
  $object->ingestDatastream($datastream);
}

/**
 * Given a list of pids, purges the object from Fedora
 *
 * @param $list_of_pids
 *   A list of pids for objects to be removed
 * 
 */
function _purge_object_by_pid($pids) {

  foreach ($pids as $pid) {
    $fedora_obj = islandora_object_load($pid);

    if ($fedora_obj) {
      if (islandora_delete_object($fedora_obj)) {
        drush_log("Successfully purged pid {$pid} ", 'ok');
      }
      else {
        drush_log("Failure to purge pid {$pid} ", 'ok');
      }
    }
  }
}

/**
 * Given a Fedora namespace, return a title/label to pid mapping
 * the given title and namespace in the triplestore
 *
 * @param string $fedora_namespace
 *  The fedora namespace to search for the given title
 * 
 */
function _create_pid_title_map_by_namespace($fedora_namespace) {

  $connection = islandora_get_tuque_connection();
  $repo = $connection->repository;
  $query =
      "
        SELECT ?identifier ?label
        FROM <#ri>
        WHERE {
          ?obj <info:fedora/fedora-system:def/model#hasModel> ?model .
          FILTER(regex(str(?obj), '^info:fedora/$fedora_namespace:')) .
          ?obj <fedora-model:label> ?label .
          ?obj <dc:identifier> ?identifier .
          }
        ORDER BY ?identifier";

  $results = $repo->ri->sparqlQuery($query);

  $title_to_pid = array();

  foreach ($results as $result) {
    $title_to_pid[$result['label']['value']] = $result['identifier']['value'];
  }

  return $title_to_pid;
}

function _url_encode_http_address($address_to_encode) {
  
  $url_parsed = parse_url($address_to_encode);
  $path_to_encode = $url_parsed['path'];
  $sections_to_encode = explode('/', $path_to_encode);

  for($i=0;$i<sizeof($sections_to_encode); $i++) {
    $sections_to_encode[$i] = rawurlencode($sections_to_encode[$i]);
  }
  
  $encoded_path = implode('/', $sections_to_encode);
  $full_url = $url_parsed['scheme'] . '://' . $url_parsed['host'] . $encoded_path;
  
  return $full_url;
}
