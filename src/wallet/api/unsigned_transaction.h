// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "wallet/api/wallet2_api.h"
#include "wallet/wallet2.h"

#include <string>
#include <vector>


namespace Monero {

class WalletImpl;
class UnsignedTransactionImpl : public UnsignedTransaction
{
public:
    UnsignedTransactionImpl(WalletImpl &wallet);
    ~UnsignedTransactionImpl();
    int status() const override;
    std::string errorString() const override;
    std::vector<uint64_t> amount() const override;
    std::vector<uint64_t> fee() const override;
    std::vector<uint64_t> mixin() const override;
    std::vector<std::string> paymentId() const override;
    std::vector<std::string> recipientAddress() const override;
    uint64_t txCount() const override;
    // sign txs and save to file
    bool sign(const std::string &signedFileName) override;
    std::string confirmationMessage() const override {return m_confirmationMessage;}
    uint64_t minMixinCount() const override;

private:
    // Callback function to check all loaded tx's and generate confirmationMessage
    bool checkLoadedTx(const std::function<size_t()> get_num_txes, const std::function<const tools::wallet2::tx_construction_data&(size_t)> &get_tx, const std::string &extra_message);
    
    friend class WalletImpl;
    WalletImpl &m_wallet;

    int  m_status;
    std::string m_errorString;
    tools::wallet2::unsigned_tx_set m_unsigned_tx_set;
    std::string m_confirmationMessage;
};


}
