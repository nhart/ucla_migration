ucla_migration
==============

This drush script will migrate an entire collection based on a METS manifest.

Dependencies
------------
Obviously, this script depends on Islandora, in particular the islandora module.  In addition to this, the dblog module must be downloaded and enabled for watchdog logging to occur.  
Also, in order to generate FITS technical metadata, the islandora_fits module must be downloaded, enabled, and properly configured.  See https://github.com/Islandora/islandora_fits for more info.

Installation
------------
Clone this module into your server's sites/all/modules folder:
git clone https://github.com/discoverygarden/ucla_migration.git

Then enable using either Drupal's admin interface or Drush.  Using Drush, the command to enable is:
drush en ucla_migration

Usage
----

drush -u 1 -l *server_url* ucla_migrate_mets *mets_path* *ingest_namespace*

For example:
drush -u 1 -l http://lit250v.library.ucla.edu ucla_migrate_mets http://digidev.library.ucla.edu/mets/pierce/21198-zz00294nxr.xml piercephoto

Logging
-------

This script uses watchdog to log information as it executes.  Tailing the logs is a pretty good idea while running the script.  You can tail the watchdog logs by executing the following command:
drush ws --tail --full
