#!/usr/bin/env python3

# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
# 

from framework.daemon import Daemon
from framework.wallet import Wallet

import time


DAEMON_IDX = 4
DAEMON_USER = "md5_lover"
DAEMON_PASS = "Z1ON0101"
WALLET_IDX = 6
WALLET_USER = "kyle"
WALLET_PASS = "reveille"
WALLET_SEED = "velvet lymph giddy number token physics poetry unquoted nibs useful sabotage limits \
               benches lifestyle eden nitrogen anvil fewest avoid batch vials washing fences goat unquoted"

class HttpDigestAuthTest():
    def run_test(self):
        self.test_daemon_login_required()
        self.test_wallet_login_required()

        self.make_daemon_conn()
        self.create_wallet()

        self.mine_through_wallet()

    def test_daemon_login_required(self):
        print('Attempting to connect to daemon loginless with RPC digest authentication required...')
        bad_daemon = Daemon(idx = DAEMON_IDX)
        try:
            res = bad_daemon.get_height()
            assert(False)
        except:
            pass
    
    def test_wallet_login_required(self):
        print('Attempting to connect to wallet server loginless with RPC digest authentication required...')
        bad_wallet = Wallet(idx = WALLET_IDX)
        try:
            res = bad_wallet.get_balance()
            assert(False)
        except:
            pass

    def make_daemon_conn(self):
        print('Connecting to daemon with RPC digest authentication required...')
        self.daemon = Daemon(idx = DAEMON_IDX, username = DAEMON_USER, password = DAEMON_PASS)
        res = self.daemon.get_height()
        self.daemon.pop_blocks(res.height - 1)
        self.daemon.flush_txpool()

    def create_wallet(self):
        print('Connecting to wallet server with RPC digest authentication required...')
        self.wallet = Wallet(idx = WALLET_IDX, username = WALLET_USER, password = WALLET_PASS)
        # close the wallet if any, will throw if none is loaded
        try: self.wallet.close_wallet()
        except: pass
        res = self.wallet.restore_deterministic_wallet(seed = WALLET_SEED)

    def mine_through_wallet(self):
        print('Telling login-required daemon to start mining through login-required wallet server...')
        start_height = self.daemon.get_height().height
        self.wallet.start_mining(2)

        print("Waiting a few seconds for mining to occur...")
        for tries in range(20):
            time.sleep(1)

            stop_height = self.daemon.get_height().height
            if stop_height > start_height:
                break

        print('Telling login-required daemon to stop mining through login-required wallet server...')
        self.wallet.stop_mining()

        num_blocks_mined = stop_height - start_height
        assert num_blocks_mined > 0
        print('Mined {} blocks!'.format(num_blocks_mined))

if __name__ == '__main__':
    HttpDigestAuthTest().run_test()
