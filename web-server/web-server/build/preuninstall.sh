#!/bin/sh
rm /etc/apache2/conf.d/reverseproxy.conf
#rm -rf /var/spool/storage/SD_DISK/lib
systemctl reload httpd
