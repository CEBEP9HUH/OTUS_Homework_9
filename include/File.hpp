#pragma once


#include <experimental/filesystem>
#include <memory>
#include <fstream>
#include <functional>


#include "HashSum.hpp"


namespace DF{
    namespace fs = std::experimental::filesystem;
    class File{
    private:
        using hashList_t = std::vector<std::unique_ptr<HashSum> >;
        const fs::path _filePath;
        const std::streamsize _blockSize;
        mutable bool _eof;
        mutable hashList_t _hashList;
        mutable size_t _pos;
        mutable std::ifstream _input;
        std::function<void(const char*, std::streamsize, hashList_t&)> insertHashIntoList;
        hashList_t::iterator getFirstBlock() const;
        hashList_t::iterator getNextBlock() const;
        hashList_t::iterator getIterFromFile() const;
    public:
        File() = delete;
        File(const File&) = delete;
        File(File&&) = delete;
        File& operator=(const File&) = delete;
        File& operator=(File&&) = delete;

        File(const fs::path& filePath, const HashType& hashType, const std::streamsize blockSize);
        bool operator==(File& other);
        bool operator<(const File& other) const;
        ~File();
    };


}