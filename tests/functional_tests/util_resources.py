#!/usr/bin/env python3

# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.

#

"""
    Help determine how much CPU power is available at the given time
    by running numerical calculations
"""

import subprocess
import psutil
import os
import errno

def available_ram_gb():
    ram_bytes = psutil.virtual_memory().available
    kilo = 1024.0
    ram_gb = ram_bytes / kilo**3
    return ram_gb

def get_time_pi_seconds(cores, app_dir='.'):
    app_path = '{}/cpu_power_test'.format(app_dir)
    time_calc = subprocess.check_output([app_path, str(cores)])
    decoded = time_calc.decode('utf-8')
    miliseconds = int(decoded)

    return miliseconds / 1000.0

def remove_file(name):
    WALLET_DIRECTORY = os.environ['WALLET_DIRECTORY']
    assert WALLET_DIRECTORY != ''
    try:
        os.unlink(WALLET_DIRECTORY + '/' + name)
    except OSError as e:
        if e.errno != errno.ENOENT:
            raise

def get_file_path(name):
    WALLET_DIRECTORY = os.environ['WALLET_DIRECTORY']
    assert WALLET_DIRECTORY != ''
    return WALLET_DIRECTORY + '/' + name

def remove_wallet_files(name):
    for suffix in ['', '.keys', '.background', '.background.keys', '.address.txt']:
        remove_file(name + suffix)

def file_exists(name):
    WALLET_DIRECTORY = os.environ['WALLET_DIRECTORY']
    assert WALLET_DIRECTORY != ''
    return os.path.isfile(WALLET_DIRECTORY + '/' + name)
