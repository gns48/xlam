#pragma once

#include <elf.h>
#include <cstring>
#include <cerrno>
#include <iostream>
#include <fstream>
#include <exception>
#include <type_traits>
#include <boost/format.hpp>

//ofset, size
typedef std::pair<off_t, size_t> section_t;

template <typename T, typename S> class ElfDumper {
    const char* m_eheader;  // ptr to the beginning of the elf image (and to the ELF header also)
    const T*  m_elfHeader;  // ptr to ELF header (either Elf32_Ehdr or Elf64_Ehdr)
    const S* m_sections;    // sections header list
    const char* m_strings;  // Section names offset
    
    // section iterator
    // find section with specified name and return offset & size
    // return <0, 0> if section not found
    const section_t findSection(const char* sname) const {
        for(int i = 0; i < m_elfHeader->e_shnum; i++)
            if(strcmp(m_strings + m_sections[i].sh_name, sname) == 0)
                return section_t {m_sections[i].sh_offset, m_sections[i].sh_size};
        return section_t {0, 0};
    }
    
public:
    ElfDumper(const ElfDumper&) = delete;
    ElfDumper& operator = (const ElfDumper&) = delete;
    
     explicit ElfDumper(const void* header) {
        // contract check
        static_assert(std::is_same_v<T, Elf32_Ehdr> && std::is_same_v<S, Elf32_Shdr> ||
                      std::is_same_v<T, Elf64_Ehdr> && std::is_same_v<S, Elf64_Shdr>);

        m_eheader   = (const char*)header;
        m_elfHeader = new ((T*)header) T; // placement new, no delete
        m_sections  = new ((S*)(m_eheader + m_elfHeader->e_shoff)) S[m_elfHeader->e_shnum];
        m_strings = m_eheader + m_sections[m_elfHeader->e_shstrndx].sh_offset;
    }

    void dumpSection(const char* sname, const char* fname) const {
        const auto& [off, size] = findSection(sname);
        if(off) { // section found
            boost::format fmt("Extracting %d bytes of section %s at offset %d into %s\n");
            std::cout << fmt % size % sname % off % fname;
            std::ofstream fs(fname, std::ios::binary | std::ios::trunc);
            fs.write(m_eheader + off, size);
        }
        else {
            boost::format fmt("Can not find section: %s");
            fmt % sname;
            throw std::logic_error(fmt.str());
        }
    }
};







