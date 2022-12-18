# WebRTC
This project will automate a voice/video service based on FreeSWITCH by Terraform, including:

* Install FreeSWITCH from source code

* Make FreeSWITCH configured and be ready to serve on AWS

* Install and enable Coturn for STUN service

* Configure security group for SIP, RTP, STUN, http, https, WebSocket and SSH

* Enable Verto WebRTC client on FreeSWITCH: https://eliassun.github.io/freeswitch_webrtc.txt 

More:

* Instruction to install FreeSWITCH from source code manually
https://eliassun.github.io/freeswitch_setup.txt

* Instruction to configure FreeSWITCH NAT Traversal, STUN/TURN and Debug manually
https://eliassun.github.io/freeswitch_sip.txt

The automation process will take about 35 minutes to finish. During the installation, it will keep output logs 
saved into /home/ubuntu/install. The last log name will be done.log. Finally, check the ports.log to see if the 
freeswitch and coturn services are in service state.

The automation process doesn't install the Verto WebRTC client, but the detail instruction is at https://eliassun.github.io/freeswitch_webrtc.txt .

* Create an EC2 instance in AWS and run the FreeSWITCH service
Add the AWS keys into vairable.tf or put the keys file to the correct location
terraform init
terraform apply --auto-approve
