# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.

# 

import requests
from requests.auth import HTTPDigestAuth
import json

class Response(dict):
    def __init__(self, d):
        for k, v in d.items():
            self[k] = self._decode(v)

    @staticmethod
    def _decode(o):
        if isinstance(o, dict):
            return Response(o)
        elif isinstance(o, list):
            return [Response._decode(i) for i in o]
        else:
            return o

    def __getattr__(self, key):
        return self[key]
    def __setattr__(self, key, value):
        self[key] = value

class JSONRPC(object):
    def __init__(self, url, username=None, password=None):
        self.url = url
        self.username = username
        self.password = password

    def send_request(self, path, inputs, result_field = None):
        res = requests.post(
            self.url + path,
            data=json.dumps(inputs),
            headers={'content-type': 'application/json'},
            auth=HTTPDigestAuth(self.username, self.password) if self.username is not None else None)
        res = res.json()

        assert 'error' not in res, res

        if result_field:
            res = res[result_field]

        return Response(res)

    def send_json_rpc_request(self, inputs):
        return self.send_request("/json_rpc", inputs, 'result')



