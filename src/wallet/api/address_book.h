// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "wallet/api/wallet2_api.h"
#include "wallet/wallet2.h"

namespace Monero {

class WalletImpl;

class AddressBookImpl : public AddressBook
{
public:
    AddressBookImpl(WalletImpl * wallet);
    ~AddressBookImpl();
    
    // Fetches addresses from Wallet2
    void refresh() override;
    std::vector<AddressBookRow*> getAll() const override;
    bool addRow(const std::string &dst_addr , const std::string &payment_id, const std::string &description) override;
    bool setDescription(std::size_t index, const std::string &description) override;
    bool deleteRow(std::size_t rowId) override;
     
    // Error codes. See AddressBook:ErrorCode enum in wallet2_api.h
    std::string errorString() const override {return m_errorString;}
    int errorCode() const override {return m_errorCode;}

    int lookupPaymentID(const std::string &payment_id) const override;
    
private:
    void clearRows();
    void clearStatus();
    
private:
    WalletImpl *m_wallet;
    std::vector<AddressBookRow*> m_rows;
    std::string m_errorString;
    int m_errorCode;
};

}
