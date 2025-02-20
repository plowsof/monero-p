#!/usr/bin/env python3

# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.

# 

"""Test daemon RPC calls

Test the following RPCs:
    - get_info
    - hard_fork_info

"""

import os
from framework.daemon import Daemon

class DaemonGetInfoTest():
    def run_test(self):
        self._test_hardfork_info()
        self._test_get_info()

    def _test_hardfork_info(self):
        print('Test hard_fork_info')

        daemon = Daemon()
        res = daemon.hard_fork_info()

        # hard_fork version should be set at height 1
        assert 'earliest_height' in res.keys()
        #assert res['earliest_height'] == 1;
        assert res.earliest_height == 1

    def _test_get_info(self):
        print('Test get_info')

        daemon = Daemon()
        res = daemon.get_info()

        # difficulty should be set to 1 for this test
        assert 'difficulty' in res.keys()
        assert res.difficulty == int(os.environ['DIFFICULTY'])

        # nettype should not be TESTNET
        assert 'testnet' in res.keys()
        assert res.testnet == False;

        # nettype should not be STAGENET
        assert 'stagenet' in res.keys()
        assert res.stagenet == False;

        # nettype should be FAKECHAIN
        assert 'nettype' in res.keys()
        assert res.nettype == "fakechain";

        # free_space should be > 0
        assert 'free_space' in res.keys()
        assert res.free_space > 0

        # height should be greater or equal to 1
        assert 'height' in res.keys()
        assert res.height >= 1


if __name__ == '__main__':
    DaemonGetInfoTest().run_test()
