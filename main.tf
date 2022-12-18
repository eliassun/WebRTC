# Create a network by Terraform
# github: https://github.com/eliassun/net4clouds

terraform {

}

# get my current public IP and keys

data "external" "local_env" {
  program = ["python3","local_env.py"]
  query = {
  }
}

# CSP is AWS
provider "aws" {
    region     = var.defaul_region
    access_key = var.access_key != "your_aws_access_key" ? var.access_key : data.external.local_env.result.aws_access_key
    secret_key = var.secret_key != "your_aws_secret_key" ? var.secret_key : data.external.local_env.result.aws_secret_key
}

# Create a VPC
resource "aws_vpc" "prod_vpc_1" {
    cidr_block = var.prod_vpc_cidrs[0]
    tags = {
        Name = "production-1"
    }
}

# Create an Internet Gateway

resource "aws_internet_gateway" "prod_internet_gw_1" {
    vpc_id = aws_vpc.prod_vpc_1.id
}

# Create a route table for the production network

resource "aws_route_table" "prod_route_table_1" {
    vpc_id = aws_vpc.prod_vpc_1.id
    route {
        cidr_block = var.prod_1_route_table_cidr_v4
        gateway_id = aws_internet_gateway.prod_internet_gw_1.id
    }
    route {
        ipv6_cidr_block = var.prod_1_route_table_cidr_v6
        gateway_id = aws_internet_gateway.prod_internet_gw_1.id
    }
    tags = {
        Name = "prod_1"
    }
}

# Create a subnet for the production network
resource "aws_subnet" "prod_vpc_1_subnet_1" {
    vpc_id            = aws_vpc.prod_vpc_1.id
    cidr_block        = var.subnet_prod_vpc_1[0].cidr_block
    availability_zone = var.us_east_2_ec2_ubuntu_az
    tags = {
        Name = var.subnet_prod_vpc_1[0].name
    }
}

# Associate the production network subnet to routetable 

resource "aws_route_table_association" "prod_1_net_1" {
    subnet_id      = aws_subnet.prod_vpc_1_subnet_1.id
    route_table_id = aws_route_table.prod_route_table_1.id
}

# Create security group

resource "aws_security_group" "prod_1_sg_1" {
    name        = "prod_1_sg_1"
    description = "production net-1 security-group-1"
    vpc_id      = aws_vpc.prod_vpc_1.id
    ingress {
        description = "IPSec-ISAKMP"
        from_port   = 500
        to_port     = 500
        protocol    = "udp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    ingress {
        description = "IPSec-ESP"
        from_port   = 4500
        to_port     = 4500
        protocol    = "udp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    ingress {
        description = "stun"
        from_port   = 3478
        to_port     = 3479
        protocol    = "udp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    ingress {
        description = "SIP"
        from_port   = 5060
        to_port     = 5060
        protocol    = "udp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    ingress {
        description = "SIP"
        from_port   = 5060
        to_port     = 5060
        protocol    = "tcp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    ingress {
        description = "SIP"
        from_port   = 5080
        to_port     = 5080
        protocol    = "tcp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    ingress {
        description = "SIP"
        from_port   = 5080
        to_port     = 5080
        protocol    = "udp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    ingress {
        description = "Websocket"
        from_port   = 5066
        to_port     = 5066
        protocol    = "tcp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    ingress {
        description = "Websocket"
        from_port   = 7443
        to_port     = 7443
        protocol    = "tcp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    ingress {
        description = "Websocket"
        from_port   = 8081
        to_port     = 8082
        protocol    = "tcp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    ingress {
        description = "fs-rtp"
        from_port   = 16383
        to_port     = 32769
        protocol    = "udp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    ingress {
        description = "SSH"
        from_port   = 22
        to_port     = 22
        protocol    = "tcp"
        cidr_blocks = ["${data.external.local_env.result.local_public_ip}/32"]      
    }
    ingress {
        description = "https"
        from_port   = 443
        to_port     = 443
        protocol    = "tcp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    ingress {
        description = "http"
        from_port   = 80
        to_port     = 80
        protocol    = "tcp"
        cidr_blocks = ["0.0.0.0/0"]
    }
    egress {
        description = "ICMP"
        from_port   = 0
        to_port     = 0
        protocol    = "-1"
        cidr_blocks = ["0.0.0.0/0"]
    }
    tags = {
        Name = "prod-1-sg-1"
    }
}

# Create a network interface with an IP in the subnet 

resource "aws_network_interface" "web_server_nic_01" {
    subnet_id       = aws_subnet.prod_vpc_1_subnet_1.id
    private_ips     = var.web_server_01_nic_ips
    security_groups = [aws_security_group.prod_1_sg_1.id]
}

# Assign an elastic IP to the network interface created

resource "aws_eip" "web_server_eip_01" {
    vpc                       = true
    network_interface         = aws_network_interface.web_server_nic_01.id
    associate_with_private_ip = var.web_server_01_nic_ips[0]
    depends_on                = [aws_internet_gateway.prod_internet_gw_1]
}

# output elastic IP 
output "output_web_server_eip_01" {
    value = aws_eip.web_server_eip_01
} 

# Create an ubuntu ec2 and install apache2

resource "aws_instance" "test-server-ec2-01" {
    ami               = var.us_east_2_ec2_ubuntu_ami
    instance_type     = var.us_east_2_ec2_ubuntu_size
    availability_zone = var.us_east_2_ec2_ubuntu_az
    key_name          = var.us_east_2_ec2_key_name
    network_interface {
        device_index = 0
        network_interface_id = aws_network_interface.web_server_nic_01.id
    }
    user_data = "${file("install_softwares.sh")}"
    tags = {
        Name = "test-server-ec2-01"
    }
}

# output the private ip of test-server-ec2-01

output "output-test-server-ec2-01-private-ip" {
    value = aws_instance.test-server-ec2-01.private_ip
}

# output the "server-id" of test-server-ec2-01

output "output-test-server-ec2-01-server-id" {
    value = aws_instance.test-server-ec2-01.id
}
