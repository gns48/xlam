#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <utility>
#include <cctype>
#include <locale>
#include <codecvt>
#include <list>
#include <map>
#include <cmath>
#include "boost/lexical_cast.hpp"

int read_input(std::string& input, const char *fname) {
    std::ifstream ifs(fname);
    if(!ifs) {
        std::wcerr << "Can not open  "  << fname << std::endl;
        return EINVAL;
    }
    while(!ifs.eof()) {
        std::string line;
        getline(ifs, line);
        input.append(line);
    }
    ifs.close();
    
    return 0;
}


int main(int ac, char **av) {
    std::string raw_input;
    std::wstring cipher;
    int max_substr;
    int kwlen;
    
    const char* lang = getenv("LANG");
    
    std::setlocale(LC_ALL, lang);
    std::wcout.imbue(std::locale(lang));
    std::wcerr.imbue(std::locale(lang));
    
    if(ac < 3) {
        std::wcerr << "Usage: " << av[0]
                   << " <file with cipher> <max substring length>" << std::endl;
        return EINVAL;
    }

    try {
        max_substr = boost::lexical_cast<int>(av[2]);
        if(max_substr < 3) {
            std::wcerr << "<max sbstring length> == 2..." << std::endl;
            return EINVAL;
        }
    }
    catch(std::exception& e) {
        std::wcerr << e.what() << std::endl;
        return EINVAL;
    }
    
    int rv = read_input(raw_input, av[1]);
    if(rv) return rv;

    //erase whitespace
    raw_input.erase(std::remove_if(raw_input.begin(), raw_input.end(), isspace),
                    raw_input.end());

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    cipher = converter.from_bytes(raw_input);

    std::list<std::wstring> found_patterns;
    std::vector<int> distances;
    
    for(int substr_len = max_substr; substr_len >= 2; substr_len--) {
        for(ssize_t pos = 0; pos < cipher.length(); pos++) {
            std::wstring pattern = cipher.substr(pos, substr_len);
            // skip already found patterns
            bool pattern_found = false;
            for(const auto& fp : found_patterns) {
                if(fp.find(pattern) != std::wstring::npos) {
                    pattern_found = true;
                    break;
                }
            }
            if(pattern_found) continue;
            
            std::vector<ssize_t> positions;
            std::wstring::size_type found = pos + substr_len;
            positions.clear();
            positions.push_back(pos);
            while(1) {
                found = cipher.find(pattern, found);
                if(found == std::wstring::npos) break;
                positions.push_back(found);
                found += substr_len;
            }
            if(positions.size() > 2 || (pattern.length() > 3 && positions.size() == 2)) {
                found_patterns.push_back(pattern);
                std::string u8pattern = converter.to_bytes(pattern);
                std::wcout << pattern << ":";
                for(int i = 1; i < positions.size(); i++) {
                    int dist = positions[i] - positions[i-1];
                    distances.push_back(dist);
                    std::wcout << " " << dist;
                }
                std::wcout << std::endl;
            }
        }
    }

    for(int j = 3; j <= max_substr; j++) {
        int mod_count = 0;
        for(int i = 0; i < distances.size(); i++) 
            if(distances[i] % j == 0) mod_count++;
        std::wcout << "keyword length: " << j << ", "
                   << mod_count << " zeros out of " << distances.size() << std::endl;
    }

    std::map<wchar_t, int> freq;
    for(int j = 1; j <= max_substr; j++) {
        std::wcout << "kwlen: " << j << std::endl;
        for(int i = 0; i < j; i++) {
            for(int pos = 0; pos < cipher.length(); pos+=(j+i)) {
                if(freq.count(cipher[pos])) freq[cipher[pos]]++;
                else freq[cipher[pos]] = 1;
            }
            double enthropy = 0.0;
            for(const auto& it : freq) {
                double p = (double)(it.second)/(double)(cipher.length());
                enthropy += p * (-log2(p));
            }
            std::wcout << "enthropy:" << i << ": " << enthropy << std::endl;
            freq.clear();
        }
    }
    
    return 0;    
}




