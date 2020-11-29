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
    const char* m_eheader; // ptr to ELF header (either Elf32_Ehdr or Elf64_Ehdr)
    off_t m_shoff;         // Section table offset
    size_t m_shentsize;    // Section table entry size
    int m_shnum;           // Section table entry count
    const char* m_strings; // Section names offset
    
    // section iterator
    // find section with specified name and return offset & size
    // return <0, 0> if section not found
    const section_t findSection(const char* sname) const {
        if constexpr (std::is_same_v<T, Elf64_Ehdr>) {
           const Elf64_Shdr* sections = (const Elf64_Shdr*)(m_eheader + m_shoff);
           for(int i = 0; i < m_shnum; i++)
               if(strcmp(m_strings + sections[i].sh_name, sname) == 0)
                   return section_t {sections[i].sh_offset, sections[i].sh_size};
        }
        else {
            const Elf32_Shdr* sections = (const Elf32_Shdr*)(m_eheader + m_shoff);
            for(int i = 0; i < m_shnum; i++)
               if(strcmp(m_strings + sections[i].sh_name, sname) == 0)
                   return section_t {sections[i].sh_offset, sections[i].sh_size};
        }
        return section_t {0, 0};
    }
    
public:
    ElfDumper(const ElfDumper&) = delete;
    ElfDumper& operator = (const ElfDumper&) = delete;
    
    explicit ElfDumper(const void* header)
        : m_eheader(reinterpret_cast<const char*>(header))
    {
        m_shoff =     ((const T*)m_eheader)->e_shoff;
        m_shentsize = ((const T*)m_eheader)->e_shentsize;
        m_shnum =     ((const T*)m_eheader)->e_shnum;
        
        if constexpr (std::is_same_v<T, Elf64_Ehdr>) {
            const Elf64_Shdr* sections = (const Elf64_Shdr*)(m_eheader + m_shoff);
            const Elf64_Shdr* ssect_header = &sections[((const T*)m_eheader)->e_shstrndx];
            m_strings = m_eheader + ssect_header->sh_offset;
        }
        else if constexpr (std::is_same_v<T, Elf32_Ehdr>) {
            const Elf32_Shdr* sections = (const Elf32_Shdr*)(m_eheader + m_shoff);
            const Elf32_Shdr* ssect_header = &sections[((const T*)m_eheader)->e_shstrndx];
            m_strings = m_eheader + ssect_header->sh_offset;
        }
        else
            throw std::logic_error("Unsupported ELF header type");
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







