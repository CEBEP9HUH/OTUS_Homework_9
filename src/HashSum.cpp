#include "HashSum.hpp"


using namespace DF;


HashType HashSum::stringToHashType(const std::string& hashType) {
    if(hashType == "crc32") {
        return HashType::CRC32;
    } else if (hashType == "md5") {
        return HashType::MD5;
    }
    return HashType::UNDEFINED;
}


CRC32Sum::CRC32Sum(const char* data, const size_t size) {
    boost::crc_32_type res;
    res.process_bytes(data, size);
    _hashsum = res.checksum();
}


bool CRC32Sum::isEqualTo(const HashSum& other) const {
    if(const CRC32Sum* tmp = static_cast<const CRC32Sum*>(&other)){
        return _hashsum == tmp->_hashsum;
    }
    return false;
}

bool CRC32Sum::isLessThan(const HashSum& other) const {
    if(const CRC32Sum* tmp = static_cast<const CRC32Sum*>(&other)){
        return _hashsum < tmp->_hashsum;
    }
    return false;
}


MD5Sum::MD5Sum(const char* data, const size_t size) {
    boost::uuids::detail::md5 hash;
    hash.process_bytes(data, size);
    hash.get_digest(_hashsum);
}


bool MD5Sum::isEqualTo(const HashSum& other) const {
    if(const MD5Sum* tmp = static_cast<const MD5Sum*>(&other)){
        return  std::equal(_hashsum, _hashsum+4, tmp->_hashsum);
    }
    return false;
}

#include <iostream>

bool MD5Sum::isLessThan(const HashSum& other) const {
    if(const MD5Sum* tmp = static_cast<const MD5Sum*>(&other)){
        return  std::lexicographical_compare(_hashsum, _hashsum+4, tmp->_hashsum, tmp->_hashsum+4);
    }
    return false;
}