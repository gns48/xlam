#pragma once

#include <elf.h>
#include <cstring>
#include <cerrno>
#include <iostream>
#include <fstream>
#include <exception>
#include <boost/format.hpp>

//ofset, size
typedef std::pair<off_t, size_t> section_t;

template <typename T> class ElfDumper {
    const T* m_eheader;       // ptr to ELF header (either Elf32_Ehdr or Elf64_Ehdr)
    off_t m_shoff;            // Section table offset
    size_t m_shentsize;       // Section table entry size
    int m_shnum;              // Section table entry count
    const char* m_strings;    // Section names offset
    
    // section iterator
    // find section with specified name and return offset & size
    // return <0, 0> if section not found
    const section_t findSection(const char* sname) const;
    
public:
    ElfDumper(const ElfDumper&) = delete;
    ElfDumper& operator = (const ElfDumper&) = delete;
    explicit ElfDumper(const void* header);
    
    void dumpSection(const char* sname, const char* fname) const {
        const auto& [off, size] = findSection(sname);
        if(off) { // section found
            boost::format fmt("Extracting %d bytes of section %s at offset %d into %s\n");
            std::cout << fmt % size % sname % off % fname;
            std::ofstream fs(fname, std::ios::binary | std::ios::trunc);
            fs.write(reinterpret_cast<const char*>(m_eheader) + off, size);
        }
        else {
            boost::format fmt("Can not find section: %s");
            fmt % sname;
            throw std::logic_error(fmt.str());
        }
    }
};

#define CONSTRUCT(STYPE)                                                \
    m_shoff = m_eheader->e_shoff;                                       \
    m_shentsize = m_eheader->e_shentsize;                               \
    m_shnum = m_eheader->e_shnum;                                       \
    const STYPE* sections = (const STYPE*)((const char*)header + m_shoff); \
    const STYPE* ssect_header = &sections[m_eheader->e_shstrndx];       \
    m_strings = (const char*)header + ssect_header->sh_offset

//we need explicit template specialization to correctly calculate header size & offsets
// constructor for 32-bit ELF
template<> ElfDumper<Elf32_Ehdr>::ElfDumper(const void* header)
    : m_eheader(reinterpret_cast<const Elf32_Ehdr*>(header))
{    
    CONSTRUCT(Elf32_Shdr);
}

// constructor for 64-bit ELF
template<> ElfDumper<Elf64_Ehdr>::ElfDumper(const void* header)
    : m_eheader(reinterpret_cast<const Elf64_Ehdr*>(header))
{
    CONSTRUCT(Elf64_Shdr);
}

// Iterator body for both iterators
#define ITERATOR(STYPE)                                                 \
    const STYPE* sections = (const STYPE*)((const char*)m_eheader + m_shoff); \
    for(int i = 0; i < m_shnum; i++)                                    \
        if(strcmp(m_strings + sections[i].sh_name, sname) == 0)         \
            return section_t {sections[i].sh_offset, sections[i].sh_size}; \
    return section_t {0, 0}
 
// section iterator for 64-bit ELF
template<> const section_t ElfDumper<Elf64_Ehdr>::findSection(const char* sname) const {
    ITERATOR(Elf64_Shdr);
}

// section iterator for 32-bit ELF
template<> const section_t ElfDumper<Elf32_Ehdr>::findSection(const char* sname) const {
    ITERATOR(Elf32_Shdr);
}




