# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.

# 

"""Class to subscribe to and receive ZMQ events."""

import zmq
import json

class Zmq(object):

    def __init__(self, protocol='tcp', host='127.0.0.1', port=0, idx=0):
        self.host = host
        self.port = port
        self.socket = zmq.Context().socket(zmq.SUB)
        self.socket.connect('{protocol}://{host}:{port}'.format(protocol=protocol, host=host, port=port if port else 18480+idx))

    def sub(self, topic):
        self.socket.setsockopt_string(zmq.SUBSCRIBE, topic)

    def recv(self, topic):
        msg = self.socket.recv()
        data = msg.decode().split(topic + ":")[1]
        return json.loads(data)
