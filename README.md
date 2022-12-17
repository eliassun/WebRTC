# WebRTC
These scripts will automate a voice/video service based on FreeSWITCH by Terraform, including:

* Install, update and upgrade Ubuntu20 to the latest version

* Install FreeSWITCH from source code

* Make FreeSWITCH configured and be ready to service on AWS

* Install Coturn

* Configure security group for SIP, RTP and STUN

More:

* FreeSWITCH installation from source code
https://eliassun.github.io/freeswitch_setup.txt

* Configure FreeSWITCH NAT Traversal, STUN/TURN and Debug
https://eliassun.github.io/freeswitch_sip.txt

The automation process will take about 35 minutes to finish. During the installation, it will keep output logs 
saved into /home/ubuntu/install. The last log name will be done.log. Finally, check the ports.log to see if the 
freeswitch and coturn services are in service state.

* Create an EC2 instance in AWS and run the FreeSWITCH service
Add the AWS keys into vairable.tf or put the keys file to the correct location
terraform init
terraform apply --auto-approve
