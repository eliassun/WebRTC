
variable "defaul_region" {
    type = string
}

variable "access_key" {
    type = string
}

variable "secret_key" {
    type = string
}

variable "prod_1_vpc_name" {
    type = string
}

variable "prod_vpc_cidrs" {
    type = list
}

variable "subnet_prod_vpc_1" {
    type = list
}

variable "prod_1_route_table_cidr_v4" {
    type = string
}

variable "prod_1_route_table_cidr_v6" {
    type = string
}

variable "web_server_01_nic_ips" {
    type = list
}

variable "us_east_2_ec2_ubuntu_ami" {
    type = string
}

variable "us_east_2_ec2_ubuntu_size" {
    type = string
}

variable "us_east_2_ec2_key_name" {
    type = string
}

variable "us_east_2_ec2_ubuntu_az" {
    type = string
}

variable "us-east-2-az" {
    type = list
}

