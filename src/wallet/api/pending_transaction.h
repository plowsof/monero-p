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
class PendingTransactionImpl : public PendingTransaction
{
public:
    PendingTransactionImpl(WalletImpl &wallet);
    ~PendingTransactionImpl();
    int status() const override;
    std::string errorString() const override;
    bool commit(const std::string &filename = "", bool overwrite = false) override;
    uint64_t amount() const override;
    uint64_t dust() const override;
    uint64_t fee() const override;
    std::vector<std::string> txid() const override;
    uint64_t txCount() const override;
    std::vector<uint32_t> subaddrAccount() const override;
    std::vector<std::set<uint32_t>> subaddrIndices() const override;
    // TODO: continue with interface;

    std::string multisigSignData() override;
    void signMultisigTx() override;
    std::vector<std::string> signersKeys() const override;

private:
    friend class WalletImpl;
    WalletImpl &m_wallet;

    int  m_status;
    std::string m_errorString;
    std::vector<tools::wallet2::pending_tx> m_pending_tx;
    std::unordered_set<crypto::public_key> m_signers;
    std::vector<std::string> m_tx_device_aux;
    std::vector<crypto::key_image> m_key_images;
};


}
