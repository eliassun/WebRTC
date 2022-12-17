# WebRTC
These scripts will automate the setup of a FreeSWITCH by Terraform, including:

1. Install, update and upgrade Ubuntu 22 to the latest version

2. Install FreeSWITCH from source code

3. Configure FreeSWITCH to run on AWS

4. Install Coturn

5. Configure security group for SIP, RTP and STUN

More:

1. FreeSWITCH installation from source code
https://eliassun.github.io/freeswitch_setup.txt

2. Configure FreeSWITCH NAT Traversal, STUN/TURN and Debug
https://eliassun.github.io/freeswitch_sip.txt

The automation process will take about 30 minutes to finish. During the installation, it will keep output logs 
saved into /home/ubuntu/install. The last log name will be done.log.
