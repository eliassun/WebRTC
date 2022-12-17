access_key         = "your_aws_access_key"
secret_key         = "your_aws_secret_key"
defaul_region      = "us-east-2"

prod_vpc_cidrs     = ["172.17.0.0/16"]
subnet_prod_vpc_1  = [{cidr_block = "172.17.1.0/24", name = "prod-172.17.1.0-24"}]

prod_1_vpc_name            = "prod_vpc_fs"
prod_1_route_table_cidr_v4 = "0.0.0.0/0"
prod_1_route_table_cidr_v6 = "::/0"

web_server_01_nic_ips = ["172.17.1.100"]

us_east_2_ec2_ubuntu_ami   = "ami-0ada6d94f396377f2"
us_east_2_ec2_ubuntu_size  = "t2.nano" # "t2.micro" #"t2.nano"
us_east_2_ec2_key_name     = "ohio-demo-keypair"
us_east_2_ec2_ubuntu_az    = "us-east-2b"

us-east-2-az = ["us-east-2b"]
