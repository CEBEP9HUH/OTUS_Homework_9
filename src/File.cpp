#include "File.hpp"


#include <iterator>
#include <iostream>


using namespace DF;


File::File(const fs::path& filePath, const HashType& hashType, const std::streamsize blockSize) :    
                                                                    _filePath{filePath}, 
                                                                    _blockSize{blockSize},
                                                                    _eof{false} {
    switch(hashType){
        case HashType::CRC32:
            insertHashIntoList = [](const char* block, std::streamsize size, hashList_t& list) {
                list.emplace_back(std::make_unique<CRC32Sum>(block, size));
            };
            break;
        case HashType::MD5:
            insertHashIntoList = [](const char* block, std::streamsize size, hashList_t& list) {
                list.emplace_back(std::make_unique<MD5Sum>(block, size));
            };
            break;
        default:
            std::cerr << "Hash type is unknown. Default hash type is selected (MD5)\n";
            insertHashIntoList = [](const char* block, std::streamsize size, hashList_t& list) {
                list.emplace_back(std::make_unique<MD5Sum>(block, size));
            };
            break;
    }
}



File::~File() {
    if(_input.is_open()){
        _input.close();
    }
}


File::hashList_t::iterator File::getIterFromFile() const {
    std::string block;
    block.reserve(_blockSize);
    if(!_input.is_open()){
        _input.open(_filePath, std::ios::in);
    }
    if(_input.readsome(&block[0], _blockSize)){
        if(_input.gcount() != _blockSize) {
            auto offset = _input.gcount();
            std::fill_n(&block[0]+offset, _blockSize-offset, 0);
        }
        insertHashIntoList(block.c_str(), _blockSize, _hashList);
        return std::prev(_hashList.end());
    } 
    _input.close();
    block.clear();
    block.shrink_to_fit();
    _eof = true;
    return _hashList.end();
}


File::hashList_t::iterator File::getFirstBlock() const {
    _pos = 0;
    return getNextBlock();
}


File::hashList_t::iterator File::getNextBlock() const {
    auto it = _hashList.begin();
    if(_pos < _hashList.size()){
        std::advance(it, _pos);
    } else if(!_eof) {
        it = getIterFromFile();
    } else {
        it = _hashList.end();
    }
    ++_pos;
    return it;
}


bool File::operator==(File& other){
    auto iter = getFirstBlock();
    auto oIter = other.getFirstBlock();
    while(iter!=_hashList.end() && oIter!=other._hashList.end()){
        if(**iter!=**oIter){
            return false;
        }
        iter = getNextBlock();
        oIter = other.getNextBlock();
    }
    return true;
}

bool File::operator<(const File& other) const {
    auto iter = getFirstBlock();
    auto oIter = other.getFirstBlock();
    while(iter!=_hashList.end() && oIter!=other._hashList.end()){
        if(**iter<**oIter){
            return true;
        }
        iter = getNextBlock();
        oIter = other.getNextBlock();
    }
    return false;
}