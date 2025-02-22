// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

namespace cryptonote
{
    // This class is meant to create a batch when none currently exists.
    // If a batch exists, it can't be from another thread, since we can
    // only be called with the txpool lock taken, and it is held during
    // the whole prepare/handle/cleanup incoming block sequence.
    class LockedTXN {
    public:
      LockedTXN(BlockchainDB &db): m_db(db), m_batch(false), m_active(false) {
        m_batch = m_db.batch_start();
        m_active = true;
      }
      void commit() { try { if (m_batch && m_active) { m_db.batch_stop(); m_active = false; } } catch (const std::exception &e) { MWARNING("LockedTXN::commit filtering exception: " << e.what()); } }
      void abort() { try { if (m_batch && m_active) { m_db.batch_abort(); m_active = false; } } catch (const std::exception &e) { MWARNING("LockedTXN::abort filtering exception: " << e.what()); } }
      ~LockedTXN() { abort(); }
    private:
      BlockchainDB &m_db;
      bool m_batch;
      bool m_active;
    };
}
