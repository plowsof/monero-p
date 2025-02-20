#!/usr/bin/env python3

# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.

# 

"""Test speed of various procedures

Test the following RPCs:
    - generateblocks
    - transfer
    - [TODO: many tests still need to be written]

"""


import time
from time import sleep

from framework.daemon import Daemon
from framework.wallet import Wallet


class SpeedTest():
    def reset(self):
        print('Resetting blockchain')
        daemon = Daemon()
        res = daemon.get_height()
        daemon.pop_blocks(res.height - 1)
        daemon.flush_txpool()

    def run_test(self):
        self.reset()

        daemon = Daemon()
        wallet = Wallet()

        # close the wallet if any, will throw if none is loaded
        try: wallet.close_wallet()
        except: pass
        wallet.restore_deterministic_wallet('velvet lymph giddy number token physics poetry unquoted nibs useful sabotage limits benches lifestyle eden nitrogen anvil fewest avoid batch vials washing fences goat unquoted')

        destinations = []
        for i in range(3):
            destinations.append({"amount":1,"address":'888tNkZrPN6JsEgekjMnABU4TBzc2Dt29EPAvkRxbANsAnjyPbb3iQ1YBRk1UXcdRsiKc9dhwMVgN5S9cQUiyoogDavup3H'})

        self._test_speed_generateblocks(daemon=daemon, blocks=70)
        for i in range(1, 10):
            while wallet.get_balance().unlocked_balance == 0:
                print('Waiting for wallet to refresh...')
                sleep(1)
            self._test_speed_transfer_split(wallet=wallet)
        self._test_speed_generateblocks(daemon=daemon, blocks=10)

    def _test_speed_generateblocks(self, daemon, blocks):
        print('Test speed of block generation')
        start = time.time()

        res = daemon.generateblocks('42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm', blocks)

        print('generating ', blocks, 'blocks took: ', time.time() - start, 'seconds')

    def _test_speed_transfer_split(self, wallet):
        print('Test speed of transfer')
        start = time.time()

        destinations = [{"amount":1,"address":'888tNkZrPN6JsEgekjMnABU4TBzc2Dt29EPAvkRxbANsAnjyPbb3iQ1YBRk1UXcdRsiKc9dhwMVgN5S9cQUiyoogDavup3H'}]
        res = wallet.transfer_split(destinations)

        print('generating tx took: ', time.time() - start, 'seconds')


if __name__ == '__main__':
    SpeedTest().run_test()
