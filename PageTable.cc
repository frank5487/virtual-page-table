#include "./PageTable.h"

namespace simplevm {

    // TODO: implment PageTable member functions
    PageTable::PageTable(std::string swap_file_name, size_t page_capacity) {
        swap_file_.open(swap_file_name, std::fstream::in | std::fstream::out);
        capacity_ = page_capacity;

        visitedTime = 0;
    }

    PageTable::~PageTable() {
        flush_all_pages();
        for (auto &item: m) {
            delete item.second;
        }
        swap_file_.close();
    }

    Page &PageTable::get_page(uint32_t virtual_address) {
        pno_t pNum = virtual_address >> 12;
        if (page_available(pNum)) {
            int t = 0;
            for (auto &item: m) {
                Page *p = item.second;
                if (p->pno() == pNum) {
                    t = item.first;
                }
            }
            Page *p = m[t];
            m.erase(t);
            m[visitedTime] = p;
            visitedTime++;
            return *p;
        }

        if (loaded_pages() >= capacity_) {
            evict_page();
        }

        Page *p = new Page(swap_file_, pNum);
        m[visitedTime] = p;
        visitedTime++;
        return *p;
    }

    size_t PageTable::capacity() {
        return capacity_;
    }

    size_t PageTable::loaded_pages() {
        return m.size();
    }

    bool PageTable::page_available(pno_t virtual_pno) {
        for (auto &item: m) {
            Page *p = item.second;
            if (p->pno() == virtual_pno) {
                return true;
            }
        }
        return false;
    }

    void PageTable::flush_all_pages() {
        for (auto &item: m) {
            Page *p = item.second;
            if (p->dirty()) {
                p->flush();
            }
        }
    }

    void PageTable::flush_page(pno_t virtual_pno) {
        for (auto &item: m) {
            Page *p = item.second;
            if (p->pno() == virtual_pno) {
                if (p->dirty()) {
                    p->flush();
                }
                return;
            }
        }
    }

    void PageTable::discard_page(pno_t virtual_pno) {
        for (auto &item: m) {
            Page *p = item.second;
            if (p->pno() == virtual_pno) {
                if (p->dirty()) {
                    p->flush();
                }
                m.erase(item.first);
                delete p;
                return;
            }
        }
    }

    void PageTable::evict_page() {
        if (loaded_pages() == 0) {
            return;
        }

        int minTime = m.begin()->first;
        for (auto &item: m) {
            if (minTime > item.first) {
                minTime = item.first;
            }
        }
        Page *evict = m[minTime];
        if (evict->dirty()) {
            evict->flush();
        }
        m.erase(minTime);
        delete evict;
    }
}