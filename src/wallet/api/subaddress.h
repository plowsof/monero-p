// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#include "wallet/api/wallet2_api.h"
#include "wallet/wallet2.h"

namespace Monero {

class WalletImpl;

class SubaddressImpl : public Subaddress
{
public:
    SubaddressImpl(WalletImpl * wallet);
    ~SubaddressImpl();
    
    // Fetches addresses from Wallet2
    void refresh(uint32_t accountIndex) override;
    std::vector<SubaddressRow*> getAll() const override;
    void addRow(uint32_t accountIndex, const std::string &label) override;
    void setLabel(uint32_t accountIndex, uint32_t addressIndex, const std::string &label) override;

private:
    void clearRows();
    
private:
    WalletImpl *m_wallet;
    std::vector<SubaddressRow*> m_rows;
};

}
