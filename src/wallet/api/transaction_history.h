// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "wallet/api/wallet2_api.h"
#include <boost/thread/shared_mutex.hpp>

namespace Monero {

class WalletImpl;

class TransactionHistoryImpl : public TransactionHistory
{
public:
    TransactionHistoryImpl(WalletImpl * wallet);
    ~TransactionHistoryImpl();
    virtual int count() const;
    virtual TransactionInfo * transaction(int index)  const;
    virtual TransactionInfo * transaction(const std::string &id) const;
    virtual std::vector<TransactionInfo*> getAll() const;
    virtual void refresh();
    virtual void setTxNote(const std::string &txid, const std::string &note);

private:

    // TransactionHistory is responsible of memory management
    std::vector<TransactionInfo*> m_history;
    WalletImpl *m_wallet;
    mutable boost::shared_mutex   m_historyMutex;
};

}
