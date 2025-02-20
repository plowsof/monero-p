#!/usr/bin/env python3

# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
# 

import time

"""Test peer baning RPC calls

Test the following RPCs:
    - set_bans
    - get_bans

"""

from framework.daemon import Daemon

class BanTest():
    def run_test(self):
        print('Testing bans')

        daemon = Daemon()
        res = daemon.get_bans()
        assert 'bans' not in res or len(res.bans) == 0

        daemon.set_bans([{'host': '1.2.3.4', 'ban': True, 'seconds': 100}])
        res = daemon.get_bans()
        assert len(res.bans) == 1
        assert res.bans[0].host == '1.2.3.4'
        assert res.bans[0].seconds >= 98 and res.bans[0].seconds <= 100 # allow for slow RPC

        daemon.set_bans([{'host': '5.6.7.8', 'ban': True, 'seconds': 100}])
        res = daemon.get_bans()
        assert len(res.bans) == 2
        for i in range(2):
          assert res.bans[i].host == '1.2.3.4' or res.bans[i].host == '5.6.7.8'
          assert res.bans[i].seconds >= 7 and res.bans[0].seconds <= 100 # allow for slow RPC

        daemon.set_bans([{'host': '1.2.3.4', 'ban': False}])
        res = daemon.get_bans()
        assert len(res.bans) == 1
        assert res.bans[0].host == '5.6.7.8'
        assert res.bans[0].seconds >= 98 and res.bans[0].seconds <= 100 # allow for slow RPC

        time.sleep(2)

        res = daemon.get_bans()
        assert len(res.bans) == 1
        assert res.bans[0].host == '5.6.7.8'
        assert res.bans[0].seconds >= 96 and res.bans[0].seconds <= 98 # allow for slow RPC

        daemon.set_bans([{'host': '3.4.5.6', 'ban': False}])
        res = daemon.get_bans()
        assert len(res.bans) == 1
        assert res.bans[0].host == '5.6.7.8'
        assert res.bans[0].seconds >= 96 and res.bans[0].seconds <= 98 # allow for slow RPC

        daemon.set_bans([{'host': '3.4.5.6', 'ban': True, 'seconds': 2}])
        res = daemon.get_bans()
        assert len(res.bans) == 2
        for i in range(2):
          assert res.bans[i].host == '5.6.7.8' or res.bans[i].host == '3.4.5.6'
          if res.bans[i].host == '5.6.7.8':
            assert res.bans[i].seconds >= 96 and res.bans[0].seconds <= 98 # allow for slow RPC
          else:
            assert res.bans[i].seconds >= 1 and res.bans[0].seconds <= 2 # allow for slow RPC

        time.sleep(2)
        res = daemon.get_bans()
        assert len(res.bans) == 1
        assert res.bans[0].host == '5.6.7.8'
        assert res.bans[0].seconds >= 94 and res.bans[0].seconds <= 96 # allow for slow RPC

        daemon.set_bans([{'host': '5.6.7.8', 'ban': True, 'seconds': 20}])
        res = daemon.get_bans()
        assert len(res.bans) == 1
        assert res.bans[0].host == '5.6.7.8'
        assert res.bans[0].seconds >= 18 and res.bans[0].seconds <= 20 # allow for slow RPC

        daemon.set_bans([{'host': '5.6.7.8', 'ban': True, 'seconds': 200}])
        res = daemon.get_bans()
        assert len(res.bans) == 1
        assert res.bans[0].host == '5.6.7.8'
        assert res.bans[0].seconds >= 198 and res.bans[0].seconds <= 200 # allow for slow RPC

        daemon.set_bans([{'host': '5.6.7.8', 'ban': False}])
        res = daemon.get_bans()
        assert 'bans' not in res or len(res.bans) == 0


if __name__ == '__main__':
    BanTest().run_test()
