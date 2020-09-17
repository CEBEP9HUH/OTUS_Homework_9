#pragma once


#include <type_traits>
#include <vector>
#include <unordered_set>
#include <map>
#include <string>
#include <queue>


#include "File.hpp"


namespace DF{
    class DuplicateFinder{
    private:
        using inc_dirs_t = std::vector<std::string>;
        using exc_dirs_t = std::unordered_set<std::string>;
        using files_t = std::map<size_t, File>;
        using duplicate_list_t = std::map<File, std::vector<fs::path> >;
        using masks_t = std::vector<std::string>;
        using this_t = DuplicateFinder;
        struct Options{
            inc_dirs_t includeDirs;
            exc_dirs_t excludeDirs;
            masks_t masks;
            size_t depth;
            size_t documentSize;
            std::streamsize blockSize;
            HashType hashType;
        } _options;
        duplicate_list_t _duplicateList;
        files_t _filesList;
        void parseOptions(int argc, char* argv[]);
        void findFiles(const std::string& path);
    public:
        DuplicateFinder() = delete;
        DuplicateFinder(const this_t&) = delete;
        DuplicateFinder(this_t&&) = delete;
        this_t& operator=(const this_t&) = delete;
        this_t& operator=(this_t&&) = delete;

        DuplicateFinder(int argc, char* argv[]);
        ~DuplicateFinder()  = default;

        void run();
        void printResults();
    };
}

