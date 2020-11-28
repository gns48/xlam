#include "extract-sig.hpp"
#include "fmapper.hpp"

static void extract_sig(const void *base, const char* outfile) {
    const char *magic = reinterpret_cast<const char *>(base);
    
    if(memcmp(magic, ELFMAG, SELFMAG) != 0) 
        throw std::runtime_error("Bad ELF header!");
    
    switch(magic[EI_CLASS]) {
    case ELFCLASS32:
        std::cout << "ELFCLASS32 header found" << std::endl;
        {
            ElfDumper<Elf32_Ehdr> ed {base};
            ed.dumpSignature(outfile);
        }
        break;
    case ELFCLASS64:
        std::cout << "ELFCLASS64 header found" << std::endl;
        {
            ElfDumper<Elf64_Ehdr> ed {base};
            ed.dumpSignature(outfile);
        }
        break;
    default:
        throw std::runtime_error("Bad ELF class!");
    }
}

int main(int ac, char **av) {
    if(ac < 3) {
        std::cerr << boost::format("Usage: %s <bsign-signed file> <file for signature>\n") % av[0];
        return EINVAL;
    }

    try {
        FileMapper f {av[1]};
        extract_sig(f.addr(), av[2]);
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EINVAL;
    }
    
    return 0;
}
