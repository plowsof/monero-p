// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

/*!
 * \file singleton.h
 * 
 * \brief A singleton helper class based on template.
 */

/*!
 * \namespace Language
 * \brief Mnemonic language related namespace.
 */
namespace Language
{
  /*!
   * \class Singleton
   * 
   * \brief Single helper class.
   * 
   * Do Language::Singleton<YourClass>::instance() to create a singleton instance
   * of `YourClass`.
   */
  template <class T>
  class Singleton
  {
    Singleton() {}
    Singleton(Singleton &s) = delete;
    Singleton& operator=(const Singleton&) = delete;
  public:
    static T* instance()
    {
      static T* obj = new T;
      return obj;
    }
  };
}
