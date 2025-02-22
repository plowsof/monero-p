// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once 

#define GET_FIELD_FROM_JSON_RETURN_ON_ERROR(json, name, type, jtype, mandatory, def) \
  type field_##name = static_cast<type>(def); \
  bool field_##name##_found = false; \
  (void)field_##name##_found; \
  do if (json.HasMember(#name)) \
  { \
    if (json[#name].Is##jtype()) \
    { \
      field_##name = static_cast<type>(json[#name].Get##jtype()); \
      field_##name##_found = true; \
    } \
    else \
    { \
      LOG_ERROR("Field " << #name << " found in JSON, but not " << #jtype); \
      return false; \
    } \
  } \
  else if (mandatory) \
  { \
    LOG_ERROR("Field " << #name << " not found in JSON"); \
    return false; \
  } while(0)

