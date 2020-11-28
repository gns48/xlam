#include <unistd.h>
#include <cstdlib>
#include "extract-sect.hpp"
#include "fmapper.hpp"

static void extract_sig(const void *base, const char* sname, const char* outfile) {
    const char *magic = reinterpret_cast<const char *>(base);
    
    if(memcmp(magic, ELFMAG, SELFMAG) != 0) 
        throw std::runtime_error("Bad ELF header!");
    
    switch(magic[EI_CLASS]) {
    case ELFCLASS32:
        std::cout << "ELFCLASS32 header found" << std::endl;
        {
            ElfDumper<Elf32_Ehdr> ed {base};
            ed.dumpSignature(sname, outfile);
        }
        break;
    case ELFCLASS64:
        std::cout << "ELFCLASS64 header found" << std::endl;
        {
            ElfDumper<Elf64_Ehdr> ed {base};
            ed.dumpSignature(sname, outfile);
        }
        break;
    default:
        throw std::runtime_error("Bad ELF class!");
    }
}

int main(int ac, char **av) {
    const char* section_name = nullptr;
    const char* output_file = nullptr;
    const char* elf_image = nullptr;
    static boost::format usage {"Usage: %s -s <section name> -o <output file> <elf image>\n"};
    int opt;
    
    while ((opt = getopt(ac, av, "hs:o:")) != -1) {
        switch (opt) {
        case 'o':
            output_file = optarg;
            break;
        case 's':
            section_name = optarg;
            break;
        case 'h':
        default: /* '?' */
            std::cerr << usage % av[0];
            return 0;
        }
    }

    bool good = true;
    if(!section_name)
        std::cerr << "No section name specified!" << std::endl, good = false;
    else if(!output_file) 
        std::cerr << "No output file specified!" << std::endl, good = false;
    else if(optind >= ac) 
        std::cerr << "No elf image given!" << std::endl, good = false;
    if(good)
        elf_image = av[optind];
    else {
        std::cerr << usage % av[0];
        return EINVAL;
    }

    try {
        FileMapper f {elf_image};
        extract_sig(f.addr(), section_name, output_file);
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EINVAL;
    }
    
    return 0;
}
