// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "wallet/api/wallet2_api.h"
#include <string>
#include <ctime>

namespace Monero {

class TransactionHistoryImpl;

class TransactionInfoImpl : public TransactionInfo
{
public:
    TransactionInfoImpl();
    ~TransactionInfoImpl();
    //! in/out
    virtual int direction() const override;
    //! true if hold
    virtual bool isPending() const override;
    virtual bool isFailed() const override;
    virtual bool isCoinbase() const override;
    virtual uint64_t amount() const override;
    //! always 0 for incoming txes
    virtual uint64_t fee() const override;
    virtual uint64_t blockHeight() const override;
    virtual std::string description() const override;
    virtual std::set<uint32_t> subaddrIndex() const override;
    virtual uint32_t subaddrAccount() const override;
    virtual std::string label() const override;

    virtual std::string hash() const override;
    virtual std::time_t timestamp() const override;
    virtual std::string paymentId() const override;
    virtual const std::vector<Transfer> &transfers() const override;
    virtual uint64_t confirmations() const override;
    virtual uint64_t unlockTime() const override;

private:
    int         m_direction;
    bool        m_pending;
    bool        m_failed;
    bool        m_coinbase;
    uint64_t    m_amount;
    uint64_t    m_fee;
    uint64_t    m_blockheight;
    std::string m_description;
    std::set<uint32_t> m_subaddrIndex;        // always unique index for incoming transfers; can be multiple indices for outgoing transfers
    uint32_t m_subaddrAccount;
    std::string m_label;
    std::string m_hash;
    std::time_t m_timestamp;
    std::string m_paymentid;
    std::vector<Transfer> m_transfers;
    uint64_t    m_confirmations;
    uint64_t    m_unlock_time;

    friend class TransactionHistoryImpl;

};

} // namespace
