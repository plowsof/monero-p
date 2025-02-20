#!/usr/bin/env python3
#encoding=utf-8

# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
# 

"""Test URI RPC
"""

try:
  from urllib import quote as urllib_quote
except:
  from urllib.parse import quote as urllib_quote

from framework.wallet import Wallet

class URITest():
    def run_test(self):
      self.create()
      self.test_monero_uri()

    def create(self):
        print('Creating wallet')
        wallet = Wallet()
        # close the wallet if any, will throw if none is loaded
        try: wallet.close_wallet()
        except: pass
        seed = 'velvet lymph giddy number token physics poetry unquoted nibs useful sabotage limits benches lifestyle eden nitrogen anvil fewest avoid batch vials washing fences goat unquoted'
        res = wallet.restore_deterministic_wallet(seed = seed)
        assert res.address == '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm'
        assert res.seed == seed

    def test_monero_uri(self):
        print('Testing monero: URI')
        wallet = Wallet()

        utf8string = [u'えんしゅう', u'あまやかす']
        quoted_utf8string = [urllib_quote(x.encode('utf8')) for x in utf8string]

        ok = False
        try: res = wallet.make_uri()
        except: ok = True
        assert ok
        ok = False
        try: res = wallet.make_uri(address = '')
        except: ok = True
        assert ok
        ok = False
        try: res = wallet.make_uri(address = 'kjshdkj')
        except: ok = True
        assert ok

        for address in [
            '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm',
            '4BxSHvcgTwu25WooY4BVmgdcKwZu5EksVZSZkDd6ooxSVVqQ4ubxXkhLF6hEqtw96i9cf3cVfLw8UWe95bdDKfRQeYtPwLm1Jiw7AKt2LY',
            '8AsN91rznfkBGTY8psSNkJBg9SZgxxGGRUhGwRptBhgr5XSQ1XzmA9m8QAnoxydecSh5aLJXdrgXwTDMMZ1AuXsN1EX5Mtm'
        ]:
            res = wallet.make_uri(address = address)
            assert res.uri == 'monero:' + address
            res = wallet.parse_uri(res.uri)
            assert res.uri.address == address
            assert res.uri.payment_id == ''
            assert res.uri.amount == 0
            assert res.uri.tx_description == ''
            assert res.uri.recipient_name == ''
            assert not 'unknown_parameters' in res or len(res.unknown_parameters) == 0
            res = wallet.make_uri(address = address, amount = 11000000000)
            assert res.uri == 'monero:' + address + '?tx_amount=0.011' or res.uri == 'monero:' + address + '?tx_amount=0.011000000000'
            res = wallet.parse_uri(res.uri)
            assert res.uri.address == address
            assert res.uri.payment_id == ''
            assert res.uri.amount == 11000000000
            assert res.uri.tx_description == ''
            assert res.uri.recipient_name == ''
            assert not 'unknown_parameters' in res or len(res.unknown_parameters) == 0

        address = '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm'

        res = wallet.make_uri(address = address, tx_description = utf8string[0])
        assert res.uri == 'monero:' + address + '?tx_description=' + quoted_utf8string[0]
        res = wallet.parse_uri(res.uri)
        assert res.uri.address == address
        assert res.uri.payment_id == ''
        assert res.uri.amount == 0
        assert res.uri.tx_description == utf8string[0]
        assert res.uri.recipient_name == ''
        assert not 'unknown_parameters' in res or len(res.unknown_parameters) == 0

        res = wallet.make_uri(address = address, recipient_name = utf8string[0])
        assert res.uri == 'monero:' + address + '?recipient_name=' + quoted_utf8string[0]
        res = wallet.parse_uri(res.uri)
        assert res.uri.address == address
        assert res.uri.payment_id == ''
        assert res.uri.amount == 0
        assert res.uri.tx_description == ''
        assert res.uri.recipient_name == utf8string[0]
        assert not 'unknown_parameters' in res or len(res.unknown_parameters) == 0

        res = wallet.make_uri(address = address, recipient_name = utf8string[0], tx_description = utf8string[1])
        assert res.uri == 'monero:' + address + '?recipient_name=' + quoted_utf8string[0] + '&tx_description=' + quoted_utf8string[1]
        res = wallet.parse_uri(res.uri)
        assert res.uri.address == address
        assert res.uri.payment_id == ''
        assert res.uri.amount == 0
        assert res.uri.tx_description == utf8string[1]
        assert res.uri.recipient_name == utf8string[0]
        assert not 'unknown_parameters' in res or len(res.unknown_parameters) == 0

        res = wallet.make_uri(address = address, recipient_name = utf8string[0], tx_description = utf8string[1], amount = 1000000000000)
        assert res.uri == 'monero:' + address + '?tx_amount=1.000000000000&recipient_name=' + quoted_utf8string[0] + '&tx_description=' + quoted_utf8string[1]
        res = wallet.parse_uri(res.uri)
        assert res.uri.address == address
        assert res.uri.payment_id == ''
        assert res.uri.amount == 1000000000000
        assert res.uri.tx_description == utf8string[1]
        assert res.uri.recipient_name == utf8string[0]
        assert not 'unknown_parameters' in res or len(res.unknown_parameters) == 0

        # external payment ids are not supported anymore
        ok = False
        try: res = wallet.make_uri(address = address, recipient_name = utf8string[0], tx_description = utf8string[1], amount = 1000000000000, payment_id = '1' * 64)
        except: ok = True
        assert ok

        # spaces must be encoded as %20
        res = wallet.make_uri(address = address, tx_description = ' ' + utf8string[1] + ' ' + utf8string[0] + ' ', amount = 1000000000000)
        assert res.uri == 'monero:' + address + '?tx_amount=1.000000000000&tx_description=%20' + quoted_utf8string[1] + '%20' + quoted_utf8string[0] + '%20'
        res = wallet.parse_uri(res.uri)
        assert res.uri.address == address
        assert res.uri.payment_id == ''
        assert res.uri.amount == 1000000000000
        assert res.uri.tx_description == ' ' + utf8string[1] + ' ' + utf8string[0] + ' '
        assert res.uri.recipient_name == ''
        assert not 'unknown_parameters' in res or len(res.unknown_parameters) == 0

        # the example from the docs
        res = wallet.parse_uri('monero:46BeWrHpwXmHDpDEUmZBWZfoQpdc6HaERCNmx1pEYL2rAcuwufPN9rXHHtyUA4QVy66qeFQkn6sfK8aHYjA3jk3o1Bv16em?tx_amount=239.39014&tx_description=donation')
        assert res.uri.address == '46BeWrHpwXmHDpDEUmZBWZfoQpdc6HaERCNmx1pEYL2rAcuwufPN9rXHHtyUA4QVy66qeFQkn6sfK8aHYjA3jk3o1Bv16em'
        assert res.uri.amount == 239390140000000
        assert res.uri.tx_description == 'donation'
        assert res.uri.recipient_name == ''
        assert res.uri.payment_id == ''
        assert not 'unknown_parameters' in res or len(res.unknown_parameters) == 0

        # malformed/invalid
        for uri in [
            '',
            ':',
            'monero',
            'notmonero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm',
            'MONERO:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm',
            'MONERO::42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm',
            'monero:',
            'monero:badaddress',
            'monero:tx_amount=10',
            'monero:?tx_amount=10',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm?tx_amount=-1',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm?tx_amount=1e12',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm?tx_amount=+12',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm?tx_amount=1+2',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm?tx_amount=A',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm?tx_amount=0x2',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm?tx_amount=222222222222222222222',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDn?tx_amount=10',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm&',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm&tx_amount',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm&tx_amount=',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm&tx_amount=10=',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm&tx_amount=10=&',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm&tx_amount=10=&foo=bar',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm?tx_amount=10&tx_amount=20',
            'monero:42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm?tx_payment_id=1111111111111111',
            'monero:4BxSHvcgTwu25WooY4BVmgdcKwZu5EksVZSZkDd6ooxSVVqQ4ubxXkhLF6hEqtw96i9cf3cVfLw8UWe95bdDKfRQeYtPwLm1Jiw7AKt2LY?tx_payment_id=' + '1' * 64,
            'monero:9ujeXrjzf7bfeK3KZdCqnYaMwZVFuXemPU8Ubw335rj2FN1CdMiWNyFV3ksEfMFvRp9L9qum5UxkP5rN9aLcPxbH1au4WAB',
            'monero:5K8mwfjumVseCcQEjNbf59Um6R9NfVUNkHTLhhPCmNvgDLVS88YW5tScnm83rw9mfgYtchtDDTW5jEfMhygi27j1QYphX38hg6m4VMtN29',
            'monero:7A1Hr63MfgUa8pkWxueD5xBqhQczkusYiCMYMnJGcGmuQxa7aDBxN1G7iCuLCNB3VPeb2TW7U9FdxB27xKkWKfJ8VhUZthF',
        ]:
            ok = False
            try: res = wallet.parse_uri(uri)
            except: ok = True
            assert ok, res

        # unknown parameters but otherwise valid
        res = wallet.parse_uri('monero:' + address + '?tx_amount=239.39014&foo=bar')
        assert res.uri.address == address
        assert res.uri.amount == 239390140000000
        assert res.unknown_parameters == ['foo=bar'], res
        res = wallet.parse_uri('monero:' + address + '?tx_amount=239.39014&foo=bar&baz=quux')
        assert res.uri.address == address
        assert res.uri.amount == 239390140000000
        assert res.unknown_parameters == ['foo=bar', 'baz=quux'], res
        res = wallet.parse_uri('monero:' + address + '?tx_amount=239.39014&%20=%20')
        assert res.uri.address == address
        assert res.uri.amount == 239390140000000
        assert res.unknown_parameters == ['%20=%20'], res
        res = wallet.parse_uri('monero:' + address + '?tx_amount=239.39014&unknown=' + quoted_utf8string[0])
        assert res.uri.address == address
        assert res.uri.amount == 239390140000000
        assert res.unknown_parameters == [u'unknown=' + quoted_utf8string[0]], res



if __name__ == '__main__':
    URITest().run_test()
