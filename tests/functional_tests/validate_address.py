#!/usr/bin/env python3

# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
# 

"""Test address validation RPC calls
"""

from framework.wallet import Wallet

class AddressValidationTest():
    def run_test(self):
      self.create()
      self.check_bad_addresses()
      self.check_good_addresses()
      self.check_openalias_addresses()

    def create(self):
        print('Creating wallet')
        seed = 'velvet lymph giddy number token physics poetry unquoted nibs useful sabotage limits benches lifestyle eden nitrogen anvil fewest avoid batch vials washing fences goat unquoted'
        address = '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm'
        self.wallet = Wallet()
        # close the wallet if any, will throw if none is loaded
        try: self.wallet.close_wallet()
        except: pass
        res = self.wallet.restore_deterministic_wallet(seed = seed)
        assert res.address == address
        assert res.seed == seed

    def check_bad_addresses(self):
        print('Validating bad addresses')
        bad_addresses = ['', 'a', '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWD9', ' ', '@', '42ey']
        for address in bad_addresses:
            res = self.wallet.validate_address(address, any_net_type = False)
            assert not res.valid
            res = self.wallet.validate_address(address, any_net_type = True)
            assert not res.valid

    def check_good_addresses(self):
        print('Validating good addresses')
        addresses = [
            [ 'mainnet',  '', '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm' ],
            [ 'mainnet',  '', '44Kbx4sJ7JDRDV5aAhLJzQCjDz2ViLRduE3ijDZu3osWKBjMGkV1XPk4pfDUMqt1Aiezvephdqm6YD19GKFD9ZcXVUTp6BW' ],
            [ 'testnet',  '', '9ujeXrjzf7bfeK3KZdCqnYaMwZVFuXemPU8Ubw335rj2FN1CdMiWNyFV3ksEfMFvRp9L9qum5UxkP5rN9aLcPxbH1au4WAB' ],
            [ 'stagenet', '', '53teqCAESLxeJ1REzGMAat1ZeHvuajvDiXqboEocPaDRRmqWoVPzy46GLo866qRFjbNhfkNckyhST3WEvBviDwpUDd7DSzB' ],
            [ 'mainnet', 'i', '4BxSHvcgTwu25WooY4BVmgdcKwZu5EksVZSZkDd6ooxSVVqQ4ubxXkhLF6hEqtw96i9cf3cVfLw8UWe95bdDKfRQeYtPwLm1Jiw7AKt2LY' ],
            [ 'mainnet', 's', '8AsN91rznfkBGTY8psSNkJBg9SZgxxGGRUhGwRptBhgr5XSQ1XzmA9m8QAnoxydecSh5aLJXdrgXwTDMMZ1AuXsN1EX5Mtm' ],
            [ 'mainnet', 's', '86kKnBKFqzCLxtK1Jmx2BkNBDBSMDEVaRYMMyVbeURYDWs8uNGDZURKCA5yRcyMxHzPcmCf1q2fSdhQVcaKsFrtGRsdGfNk' ],
            [ 'testnet', 'i', 'AApMA1VuhiCaHzr5X2KXi2Zc9oJ3VaGjkfChxxpRpxkyKf1NetvbRbQTbFMrGkr85DjnEH7JsBaoUFsgKwZnmtnVWnoB8MDotCsLb7eWwz' ],
            [ 'testnet', 's', 'BdKg9udkvckC5T58a8Nmtb6BNsgRAxs7uA2D49sWNNX5HPW5Us6Wxu8QMXrnSx3xPBQQ2iu9kwEcRGAoiz6EPmcZKbF62GS' ],
            [ 'testnet', 's', 'BcFvPa3fT4gVt5QyRDe5Vv7VtUFao9ci8NFEy3r254KF7R1N2cNB5FYhGvrHbMStv4D6VDzZ5xtxeKV8vgEPMnDcNFuwZb9' ],
            [ 'stagenet', 'i', '5K8mwfjumVseCcQEjNbf59Um6R9NfVUNkHTLhhPCmNvgDLVS88YW5tScnm83rw9mfgYtchtDDTW5jEfMhygi27j1QYphX38hg6m4VMtN29' ],
            [ 'stagenet', 's', '73LhUiix4DVFMcKhsPRG51QmCsv8dYYbL6GcQoLwEEFvPvkVvc7BhebfA4pnEFF9Lq66hwvLqBvpHjTcqvpJMHmmNjPPBqa' ],
            [ 'stagenet', 's', '7A1Hr63MfgUa8pkWxueD5xBqhQczkusYiCMYMnJGcGmuQxa7aDBxN1G7iCuLCNB3VPeb2TW7U9FdxB27xKkWKfJ8VhUZthF' ],
        ]
        for any_net_type in [True, False]:
            for address in addresses:
                res = self.wallet.validate_address(address[2], any_net_type = any_net_type)
                if any_net_type or address[0] == 'mainnet':
                    assert res.valid
                    assert res.integrated == (address[1] == 'i')
                    assert res.subaddress == (address[1] == 's')
                    assert res.nettype == address[0]
                    assert res.openalias_address == ''
                else:
                    assert not res.valid

    def check_openalias_addresses(self):
        print('Validating openalias addresses')
        addresses = [
            ['donate@getmonero.org', '888tNkZrPN6JsEgekjMnABU4TBzc2Dt29EPAvkRxbANsAnjyPbb3iQ1YBRk1UXcdRsiKc9dhwMVgN5S9cQUiyoogDavup3H']
        ]
        for address in addresses:
            res = self.wallet.validate_address(address[0])
            assert not res.valid
            res = self.wallet.validate_address(address[0], allow_openalias = True)
            assert res.valid
            assert not res.integrated
            assert res.subaddress
            assert res.nettype == 'mainnet'
            assert res.openalias_address == address[1]

if __name__ == '__main__':
    AddressValidationTest().run_test()
