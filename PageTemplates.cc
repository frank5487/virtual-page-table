#include "./Page.h"

namespace simplevm {
  // TODO: implement all template member functions for Page
  template <typename T>
  T Page::access(uint32_t virtual_address) {
      pno_t address = virtual_address & 0x00000FFF;
      uint8_t* dest = bytes_ + address;
      return *(reinterpret_cast<T*>(dest));
  }

    template <typename T>
    void Page::store(uint32_t virtual_address, const T& to_write) {
        pno_t address = virtual_address & 0x00000FFF;
        void* dest = bytes_ + address;
        memcpy(dest, &to_write, sizeof(to_write));
        dirty_ = true;
    }

}
