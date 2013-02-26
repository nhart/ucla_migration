<?php

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

function ucla_migration_migrate_mets($file_path) {
  module_load_include('inc', 'islandora', 'includes/utilities');

  $mets_file = file_get_contents($file_path);
  if (!$mets_file) {
    drush_set_error('File not found', 'File not found');  //@TODO: better error message
  }

  $mets_xml = simplexml_load_string($mets_file);

  $namespaces = $mets_xml->getDocNamespaces();
  $mets_xml->registerXPathNamespace('mets', $namespaces['mets']);
  $mets_xml->registerXPathNamespace('xlink', $namespaces['xlink']);

  foreach ($mets_xml->children($namespaces['mets'])->dmdSec AS $section) {
    $id = (string) $section->attributes()->ID;
    $label = (string) $section->mdWrap->attributes()->LABEL;
    $mods = $section->mdWrap->xmlData->children('http://www.loc.gov/mods/v3');
    $file_id_xpath = $mets_xml->xpath("//mets:div[@DMDID='" . $id . "']/mets:fptr");
    $file_id = (string) $file_id_xpath[0]['FILEID'];
    $file_xpath = $mets_xml->xpath("//mets:file[@ID='" . $file_id . "']/mets:FLocat/@xlink:href");
    $file_path = (string) $file_xpath[0]['href'];

    // Construct the object
    //islandora_prepare_new_object($namespace = NULL, $label = NULL, $datastreams = array(), $content_models = array(), $relationships = array());
  }
}