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
  $items['ucla_migrate_mets'] = array(
    'callback' => 'ucla_migration_migrate_mets',
    'description' => 'Ingest content from METS record.',
    'arguments' => array(
      'path' => 'The file path to the METS file.'
    ),
    'options' => array(
      'path' => 'The file path to the METS file.'
    ),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com ucla_migrate_mets /home/dgiuser/21198-zz00294nxr.xml'
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}

/**
 * Main function of this drush script.
 *
 * Executes the migration based on a mets file.
 *
 * @param string $file_path
 *   The path to the mets file serving as the manifest for this ingest.
 *
 * @param string $fedora_namespace
 *   The Fedora namespace to ingest everything under.
 */
function ucla_migration_migrate_mets($file_path) {
  module_load_include('inc', 'islandora', 'includes/utilities');

  $fedora_namespace = "uclaTest";

  // Check to make sure the path to the mets file has been provided.
  if (!isset($file_path)) {
    drush_set_error('File path not provided',
      'Path to mets file not provided as command line argument',
      'Error: ');
    return;
  }

  // Attempt to grab the mets file.
  $mets_file = file_get_contents($file_path);
  if (!$mets_file) {
    drush_set_error('File not found',
      "Could not locate mets file $file_path",
      "Error: ");
    return;
  }

  $mets_xml = simplexml_load_string($mets_file);

  // Register namespaces of the root element
  $xml_namespaces = $mets_xml->getDocNamespaces();
  $mets_xml->registerXPathNamespace('mets', $xml_namespaces['mets']);
  $mets_xml->registerXPathNamespace('xlink', $xml_namespaces['xlink']);

  // Create the collection first.
  $collection = ucla_migration_migrate_collection_object($fedora_namespace, $mets_xml);

  // Exit if ingest of collection object failed.
  // The ucla_migration_migrate_collection_object() function will appropriately
  // call drush_set_error if there was an error, so no need for another.  Just
  // exit the function.
  if (!collection) {
    return;
  }

  drush_log("SUCCESS", 'ok');
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
    drush_set_error("Invalid XPath",
      "Error in XPath query $collection_id_section_query",
      "Error: ");
    return FALSE;
  }

  if (empty($collection_id_section_array)) {
    drush_set_error("No XPath Results",
      "No results for XPath query $collection_id_section_query",
      "Error: ");
    return FALSE;
  }

  if (count($collection_id_section_array) > 1) {
    drush_set_error("Too many XPath Results",
      "More than one result for XPath query $collection_id_section_query",
      "Error: ");
    return FALSE;
  }

  // Actually get the id.
  $collection_id = $collection_id_section_array[0]['DMDID'];

  // Query for the collection based on id.
  $collection_section_query = "//mets:dmdSec[@ID='$collection_id']";
  $collection_section_array = $mets_xml->xpath($collection_section_query);

  // Validate results.
  if (!is_array($collection_section_array)) {
    drush_set_error("Invalid XPath",
      "Error in XPath query $collection_section_query",
      "Error: ");
    return FALSE;
  }

  if (empty($collection_section_array)) {
    drush_set_error("No XPath Results",
      "No results for XPath query $collection_section_query",
      "Error: ");
    return FALSE;
  }

  if (count($collection_section_array) > 1) {
    drush_set_error("Too many XPath Results",
      "More than one result for XPath query $collection_section_query",
      "Error: ");
    return FALSE;
  }

  // Get the children elements of the collection section that have a mets
  // namespace.
  $xml_namespaces = $mets_xml->getDocNamespaces();
  $collection_section = $collection_section_array[0]->children($xml_namespaces['mets']);

  // Validate.
  if ($collection_section->count() == 0) {
    drush_set_error('No mdWrap element.',
      "Could not locate mdWrap element for $collection_id in the METS.",
      "Error: ");
    return FALSE;
  }

  if ($collection_section->count() > 1) {
    drush_set_error('Too many mdWrap elements.',
      "Multiple mdWrap elements for $collection_id in the METS.",
      "Error: ");
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
    drush_set_error('No mods element.',
      "Could not locate mods element for $collection_id in the METS.",
      "Error: ");
    return FALSE;
  }

  if ($collection_mods->count() > 1) {
    drush_set_error('Too many mods elements.',
      "Multiple mods elements for $collection_id in the METS.",
      "Error: ");
    return FALSE;
  }

  // Grab the mods string.
  $mods_string = $collection_mods->mods->asXML();

  // Create the NewFedoraObject for the collection.
  $collection = ucla_migration_create_collection_object(
    $fedora_namespace,
    $collection_label,
    $collection_mods->asXML());

  // Ingest the NewFedoraObject.
  $ingest_results = islandora_add_object($collection);

  // Validate.
  if (!$ingest_results) {
    drush_set_error('Failed ingest',
      "Ingest for the collection object has failed.  Check the watchdog logs.",
      "Error: ");
    return FALSE;
  }

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
 * Adds relationships and content models for the collection.
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
  return $collection;
}
