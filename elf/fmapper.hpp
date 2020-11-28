#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>
#include <exception>
#include <boost/format.hpp>

class FileMapper {
    int m_fd;
    const void *m_addr;
    off_t m_size;

public:
    FileMapper(const FileMapper&) = delete;
    FileMapper& operator = (const FileMapper&) = delete;
    
    explicit FileMapper(const char *fname)
        : m_fd(-1), m_addr(nullptr), m_size(0)
    {
        m_fd = open(fname, O_RDONLY);
        if(m_fd < 0) {
            boost::format fmt("%s: %s");
            fmt % fname % strerror(errno);
            throw std::runtime_error(fmt.str());
        }
        
        struct stat st;
        fstat(m_fd, &st);
        m_size = st.st_size;
        m_addr = mmap(NULL, m_size, PROT_READ, MAP_PRIVATE, m_fd, 0);
        if(!m_addr) {
            close(m_fd);
            boost::format fmt("map %s: %s");
            fmt % fname % strerror(errno);
            throw std::runtime_error(fmt.str());
        }
    }
    
    ~FileMapper() {
        if(m_fd > 0) {
            if(m_addr) munmap((void*)m_addr, m_size);
            close(m_fd);
        }
    }

    const void* addr() const { return m_addr; }
};






