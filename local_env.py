# This script will contain all local environment settings.
# github: https://github.com/eliassun/net4clouds

import sys
import os
import json
import subprocess

KEYS_FILE_PATH = os.path.expanduser('~')+'/.keys'

# Get local public IP

outs = subprocess.run(["curl", "ifconfig.me"], capture_output=True)

if not outs.stdout.decode('utf-8'):
    sys.exit(outs.stderr.decode('utf-8'))

output = {
    "local_public_ip": str(outs.stdout.decode('utf-8')),  # For the security group
}

try:
    with open(KEYS_FILE_PATH, 'r') as keys_file:
        keys = json.load(keys_file)
except (IOError, ValueError, TypeError) as err:
    pass # The main.tf may use the variables in other tf files
else:
    output.update(keys)

output_json = json.dumps(output, indent=2)

print(output_json)
