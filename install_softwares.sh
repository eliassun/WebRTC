#! /bin/bash

cd /home/ubuntu
mkdir install
cd install
sudo apt-get update > update.log
sudo apt-get -y upgrade > upgrade.log
sudo apt-get -y install strongswan strongswan-pki libcharon-extra-plugins libcharon-extauth-plugins libstrongswan-extra-plugins libtss2-tcti-tabrmd0 net-tools > strong.log
echo "installing certs..." > certs.log
sudo ipsec pki --gen --size 4096 --type rsa --outform pem > /etc/ipsec.d/private/ca.key.pem
sudo ipsec pki --self --in /etc/ipsec.d/private/ca.key.pem --type rsa --dn "CN=site1@elias" --ca --lifetime 3650 --outform pem ca.cert.pem > /etc/ipsec.d/cacerts/ca.cert.pem
sudo ipsec pki --gen --size 4096 --type rsa --outform pem > /etc/ipsec.d/private/server.key.pem
IP=`curl http://checkip.amazonaws.com`
echo $IP > ip.log
sudo ipsec pki --pub --in /etc/ipsec.d/private/server.key.pem --type rsa | ipsec pki --issue --lifetime 3650 --cacert /etc/ipsec.d/cacerts/ca.cert.pem --cakey /etc/ipsec.d/private/ca.key.pem --dn "CN=<$IP>" --san="<$IP>" --flag serverAuth --flag ikeIntermediate --outform pem > /etc/ipsec.d/certs/server.cert.pem
echo "enable forwarding..." > forward.log
sudo echo "net.ipv4.ip_forward=1" >> /etc/sysctl.conf
sudo echo "net.ipv6.conf.all.forwarding=1" >> /etc/sysctl.conf
sudo echo "net.ipv4.conf.all.accept_redirects = 0" >>  /etc/sysctl.conf
sudo echo "net.ipv6.conf.all.accept_redirects = 0" >>  /etc/sysctl.conf
sudo sysctl -p
echo "Done" > done.log

