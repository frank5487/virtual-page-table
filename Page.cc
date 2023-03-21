#include "./Page.h"


namespace simplevm {
  // TODO: implement all non template member functions for Page
  Page::Page(fstream &swap_file, pno_t virtual_pno):
  swap_file_(swap_file), virtual_pno_(virtual_pno),
  bytes_(new uint8_t[PAGE_SIZE]), dirty_(false) {

      pno_t offset = virtual_pno_ * PAGE_SIZE;
      swap_file_.seekg(offset);
      swap_file_.read(reinterpret_cast<char*>(bytes_), PAGE_SIZE);

  }

    Page::Page(const Page &other):
    swap_file_(other.swap_file_), virtual_pno_(other.virtual_pno_),
    bytes_(new uint8_t[PAGE_SIZE]), dirty_(other.dirty_) {
      copy_bytes(bytes_, other.bytes_);
//        for (size_t i = 0; i < PAGE_SIZE; i++) {
//            bytes_[i] = other.bytes_[i];
//        }
    }


    Page::~Page() {
      if (dirty_) {
          flush();
      }
      delete[] bytes_;
    }

    Page &Page::operator=(const Page &rhs) {
      if (this == &rhs) {
          return *this;
      }

      virtual_pno_ = rhs.virtual_pno_;
      dirty_ = rhs.dirty_;
      delete[] bytes_;
      bytes_ = new uint8_t[PAGE_SIZE];
      copy_bytes(bytes_, rhs.bytes_);
//        for (size_t i = 0; i < PAGE_SIZE; i++) {
//            bytes_[i] = rhs.bytes_[i];
//        }
      return *this;
    }

    bool Page::operator<(const Page &rhs) {
      return virtual_pno_ < rhs.virtual_pno_;
    }

    pno_t Page::pno() {
        return virtual_pno_;
    }

    bool Page::dirty() {
        return dirty_;
    }

    void Page::flush() {
        if (dirty_) {
            pno_t offset = virtual_pno_ * PAGE_SIZE;
            swap_file_.seekg(offset);
            swap_file_.write(reinterpret_cast<const char*>(bytes_), PAGE_SIZE);
            swap_file_.flush();
            dirty_ = false;
        }
    }

    void Page::copy_bytes(uint8_t *bytes, uint8_t *bytes1) {
      for (size_t i = 0; i < PAGE_SIZE; i++) {
          bytes[i] = bytes1[i];
      }
    }

}