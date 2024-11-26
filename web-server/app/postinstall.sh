#!/bin/sh
#mkdir -p /var/spool/storage/SD_DISK/lib/
#curl -L https://www.snap4city.org/download/video/lib/Object-Detector-Lib-v0-1.tar.gz --output /var/spool/storage/SD_DISK/lib/lib.tar.gz
#tar -xzvf /var/spool/storage/SD_DISK/lib/lib.tar.gz -C /var/spool/storage/SD_DISK/lib/
cp  /usr/local/packages/disit_app/reverseproxy.conf /etc/apache2/conf.d/reverseproxy.conf
systemctl reload httpd
