// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers


#include "wallet/api/wallet2_api.h"
#include "net/http.h"
#include <string>

namespace Monero {

class WalletManagerImpl : public WalletManager
{
public:
    Wallet * createWallet(const std::string &path, const std::string &password,
                          const std::string &language, NetworkType nettype, uint64_t kdf_rounds = 1) override;
    Wallet * openWallet(const std::string &path, const std::string &password, NetworkType nettype, uint64_t kdf_rounds = 1, WalletListener * listener = nullptr) override;
    virtual Wallet * recoveryWallet(const std::string &path,
                                       const std::string &password,
                                       const std::string &mnemonic,
                                       NetworkType nettype,
                                       uint64_t restoreHeight,
                                       uint64_t kdf_rounds = 1,
                                       const std::string &seed_offset = {}) override;
    virtual Wallet * createWalletFromKeys(const std::string &path,
                                             const std::string &password,
                                             const std::string &language,
                                             NetworkType nettype,
                                             uint64_t restoreHeight,
                                             const std::string &addressString,
                                             const std::string &viewKeyString,
                                             const std::string &spendKeyString = "",
                                             uint64_t kdf_rounds = 1) override;
    // next two methods are deprecated - use the above version which allow setting of a password
    virtual Wallet * recoveryWallet(const std::string &path, const std::string &mnemonic, NetworkType nettype, uint64_t restoreHeight) override;
    // deprecated: use createWalletFromKeys(..., password, ...) instead
    virtual Wallet * createWalletFromKeys(const std::string &path, 
                                                    const std::string &language,
                                                    NetworkType nettype, 
                                                    uint64_t restoreHeight,
                                                    const std::string &addressString,
                                                    const std::string &viewKeyString,
                                                    const std::string &spendKeyString = "") override;
    virtual Wallet * createWalletFromDevice(const std::string &path,
                                            const std::string &password,
                                            NetworkType nettype,
                                            const std::string &deviceName,
                                            uint64_t restoreHeight = 0,
                                            const std::string &subaddressLookahead = "",
                                            uint64_t kdf_rounds = 1,
                                            WalletListener * listener = nullptr) override;
    virtual bool closeWallet(Wallet *wallet, bool store = true) override;
    bool walletExists(const std::string &path) override;
    bool verifyWalletPassword(const std::string &keys_file_name, const std::string &password, bool no_spend_key, uint64_t kdf_rounds = 1) const override;
    bool queryWalletDevice(Wallet::Device& device_type, const std::string &keys_file_name, const std::string &password, uint64_t kdf_rounds = 1) const override;
    std::vector<std::string> findWallets(const std::string &path) override;
    std::string errorString() const override;
    void setDaemonAddress(const std::string &address) override;
    bool connected(uint32_t *version = NULL) override;
    uint64_t blockchainHeight() override;
    uint64_t blockchainTargetHeight() override;
    uint64_t networkDifficulty() override;
    double miningHashRate() override;
    uint64_t blockTarget() override;
    bool isMining() override;
    bool startMining(const std::string &address, uint32_t threads = 1, bool background_mining = false, bool ignore_battery = true) override;
    bool stopMining() override;
    std::string resolveOpenAlias(const std::string &address, bool &dnssec_valid) const override;
    bool setProxy(const std::string &address) override;

private:
    WalletManagerImpl();
    friend struct WalletManagerFactory;
    net::http::client m_http_client;
    std::string m_errorString;
};

} // namespace
