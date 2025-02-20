// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#include "subaddress_account.h"
#include "wallet.h"
#include "crypto/hash.h"
#include "wallet/wallet2.h"
#include "common_defines.h"

#include <vector>

namespace Monero {
  
SubaddressAccount::~SubaddressAccount() {}
  
SubaddressAccountImpl::SubaddressAccountImpl(WalletImpl *wallet)
    : m_wallet(wallet) {}

void SubaddressAccountImpl::addRow(const std::string &label)
{
  m_wallet->m_wallet->add_subaddress_account(label);
  refresh();
}

void SubaddressAccountImpl::setLabel(uint32_t accountIndex, const std::string &label)
{
  m_wallet->m_wallet->set_subaddress_label({accountIndex, 0}, label);
  refresh();
}

void SubaddressAccountImpl::refresh() 
{
  LOG_PRINT_L2("Refreshing subaddress account");
  
  clearRows();
  for (uint32_t i = 0; i < m_wallet->m_wallet->get_num_subaddress_accounts(); ++i)
  {
    m_rows.push_back(new SubaddressAccountRow(
      i,
      m_wallet->m_wallet->get_subaddress_as_str({i,0}),
      m_wallet->m_wallet->get_subaddress_label({i,0}),
      cryptonote::print_money(m_wallet->m_wallet->balance(i, false)),
      cryptonote::print_money(m_wallet->m_wallet->unlocked_balance(i, false))
    ));
  }
}

void SubaddressAccountImpl::clearRows() {
   for (auto r : m_rows) {
     delete r;
   }
   m_rows.clear();
}

std::vector<SubaddressAccountRow*> SubaddressAccountImpl::getAll() const
{
  return m_rows;
}

SubaddressAccountImpl::~SubaddressAccountImpl()
{
  clearRows();
}

} // namespace
