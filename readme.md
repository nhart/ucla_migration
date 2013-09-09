ucla_migration
==============

This drush script will migrate an entire collection based on a METS manifest.

Dependencies
------------
Obviously, this script depends on Islandora, in particular the islandora module.  In addition to this, the dblog module must be downloaded and enabled for watchdog logging to occur.  
Also, in order to generate FITS technical metadata, the islandora_fits module must be downloaded, enabled, and properly configured.  See https://github.com/Islandora/islandora_fits for more info.

tl;dr

Considering ${DRUPAL_HOME} as where you have Drupal installed and ${DRUPAL_USER} as the system user that Drupal runs as ('apache' on RHEL, 'www-data' on Ubuntu):

* $ sudo -u ${DRUPAL_USER} drush -r ${DRUPAL_HOME} en dblog
* $ cd ~
* $ wget http://fits.googlecode.com/files/fits-0.6.1.zip
* $ sudo unzip -d /usr/share fits-0.6.1.zip
* $ sudo ln -s /usr/share/fits-0.6.1 /usr/share/fits
* $ sudo chown -R ${DRUPAL_USER}:${DRUPAL_USER} /usr/share/fits-0.6.1
* $ cd ${DRUPAL_HOME}/sites/all/modules
* $ git clone https://github.com/Islandora/islandora_fits
* $ sudo -u ${DRUPAL_USER} drush -r ${DRUPAL_HOME} en islandora_fits
* Go to your site's admin/islandora/fits page and set the path to the fits file to be /usr/share/fits/fits.sh

Installation
------------
Clone this module into your server's sites/all/modules folder.  Then enable using either Drupal's admin interface or Drush.

tl;dr
* $ cd ${DRUPAL_HOME}/sites/all/modules
* $ git clone https://github.com/discoverygarden/ucla_migration.git
* $ sudo -u ${DRUPAL_USER} drush -r ${DRUPAL_HOME} en ucla_migration

Usage
----
Usage is very straightforward.  You can start a new migration with
* drush -u 1 -l *server_url* ucla_migrate_mets *mets_path* *ingest_namespace*

For example:
* drush -u 1 -l http://lit250v.library.ucla.edu ucla_migrate_mets http://digidev.library.ucla.edu/mets/pierce/21198-zz00294nxr.xml piercephoto

You can recover a failed migration by entering the numerical portion of the pid of the last successfully ingested object.  You can discover the last successfully ingested object by tailing the logs.
* drush -i 1 -l *server_url* ucla_migrate_mets *mets_path* *ingest_namespace* *last_ingest_number*

For example, if your logs tell you the last ingested object was piercephoto:60, you can use:
* drush -u 1 -l http://lit250v.library.ucla.edu ucla_migrate_mets http://digidev.library.ucla.edu/mets/pierce/21198-zz00294nxr.xml piercephoto 60

This will automatically delete the failed ingest of piercephoto:61, if it's floating around in Fedora, then restart the migration.  It will start by attempting to re-ingest the last failed object as piercephoto:62.

Running in the background
-------------------------
Chances are, you're not going to babysit your terminal while this migration is happening.  If you'd like for the execution of the drush script to live beyond the life your shell session, you'll need to run the command using *nohup*.  If you'd like your shell back, you'll also need to send the nohup process to the background by hitting *ctrl+z* to stop the process and put it in the background.  Entering *bg* at your bash prompt will re-start the process in the background.

tl;dr
* $ nohup drush -u 1 -l *server_url* ucla_migrate_mets *mets_path* *ingest_namespace*
* ctrl+z
* $ bg

Logging
-------
This script uses watchdog to log information as it executes.  You can tail the watchdog logs by executing the following command within a drupal filesystem:
* $ drush ws --tail --full

All errors, as well as the pid of each successfully ingested object, gets written to the logs.  You can use this info to debug a failed migration, and even recover one by passing the numerical portion of the pid of the last successfully ingested object into the *ucla_migrate_mets* drush command.

Fixing versionable datastreams
------------------------------
This script has run into the FCREPO-849 bug when creating the DC datatreams from the MODS records in the METS.  At discoverygarden, we've patched up the Tuque library to work around it so it will never happen again.  But if you've already run migrations, there's a few hoops you'll have to jump through to fix the already ingested objects.  You'll need to run the ucla_save_pids_for_namespace drush command for all the namespaces you've already ingested under.  Then you need to disable the resource index in the fedora.fcfg config file and restart the Fedora stack.  Then execute the ucla_make_namespace_versionable drush command for each broken namespace.  Afterwards, re-enable the resource index in fedora.fcfg and restart the Fedora stack.

If anyone was purging or ingesting objects into Fedora while the resource index was turned off, you'll have to rebuild the resource index, which can be time consuming depending on the number of ingested objects.  It's best to inform any interested parties to stay off the server while you're executing this fix.

tl;dr
* $ cd /var/www/html/drupal/sites/all/modules
* $ drush -u 1 -l *server_url* ucla_save_pids_for_namespace *ingest_namespace*
* Edit line 351 of /usr/local/feodra/server/config/fedora.fcfg to &lt;param name="level" value="0"&gt;
* $ sudo /etc/init.d/fedora restart
* $ drush -u 1 -l *server_url* ucla_make_namespace_versionable *ingest_namespace*
* Edit line 351 of /usr/local/feodra/server/config/fedora.fcfg to &lt;param name="level" value="1"&gt;
* $ sudo /etc/init.d/fedora restart
