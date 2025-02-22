// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
//

#include "device.hpp"
#include "device_default.hpp"
#ifdef WITH_DEVICE_LEDGER
#include "device_ledger.hpp"
#endif
#include "misc_log_ex.h"


namespace hw {
    
    /* ======================================================================= */
    /*  SETUP                                                                  */
    /* ======================================================================= */

    static device_registry *get_device_registry(bool clear = false){
      static device_registry *registry = new device_registry();
      if (clear)
      {
        delete registry;
        registry = NULL;
      }
      return registry;
    }

    static void clear_device_registry(){
      get_device_registry(true);
    }

    device_registry::device_registry(){
        hw::core::register_all(registry);
        #ifdef WITH_DEVICE_LEDGER
        hw::ledger::register_all(registry);
        #endif
        atexit(clear_device_registry);
    }

    bool device_registry::register_device(const std::string & device_name, device * hw_device){
        auto search = registry.find(device_name);
        if (search != registry.end()){
            return false;
        }

        registry.insert(std::make_pair(device_name, std::unique_ptr<device>(hw_device)));
        return true;
    }

    device& device_registry::get_device(const std::string & device_descriptor){
        // Device descriptor can contain further specs after first :
        auto delim = device_descriptor.find(':');
        auto device_descriptor_lookup = device_descriptor;
        if (delim != std::string::npos) {
            device_descriptor_lookup = device_descriptor.substr(0, delim);
        }

        auto device = registry.find(device_descriptor_lookup);
        if (device == registry.end()) {
            MERROR("Device not found in registry: '" << device_descriptor << "'. Known devices: ");
            for( const auto& sm_pair : registry ) {
                MERROR(" - " << sm_pair.first);
            }
            throw std::runtime_error("device not found: " + device_descriptor);
        }
        return *device->second;
    }

    device& get_device(const std::string & device_descriptor) {
        device_registry *registry = get_device_registry();
        return registry->get_device(device_descriptor);
    }

    bool register_device(const std::string & device_name, device * hw_device){
        device_registry *registry = get_device_registry();
        return registry->register_device(device_name, hw_device);
    }

}
