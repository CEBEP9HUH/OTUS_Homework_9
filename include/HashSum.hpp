#pragma once


#include <string>
#include "boost/crc.hpp"
#include "boost/uuid/detail/md5.hpp"


namespace DF{
    enum class HashType : boost::crc_32_type::value_type {
        UNDEFINED   = 0,
        CRC32       = 1,
        MD5         = 2
    };

    class HashSum{
        protected:
            virtual bool isEqualTo(const HashSum&) const = 0;
            virtual bool isLessThan(const HashSum&) const = 0;
        public:
            virtual ~HashSum() = default;
            friend bool operator==(const HashSum& lhs, const HashSum&rhs) { return lhs.isEqualTo(rhs); }
            friend bool operator!=(const HashSum& lhs, const HashSum&rhs) { return !lhs.isEqualTo(rhs); }
            friend bool operator<(const HashSum& lhs, const HashSum&rhs) { return lhs.isLessThan(rhs); }
            static HashType stringToHashType(const std::string& hashType);
    };

    class CRC32Sum : public HashSum{
        private:
            boost::crc_32_type::value_type _hashsum;
        protected:
            virtual bool isEqualTo(const HashSum&) const override;
            virtual bool isLessThan(const HashSum&) const override;
        public:
            virtual ~CRC32Sum() = default;
            CRC32Sum(const char* data, const size_t size);
    };

    class MD5Sum : public HashSum{
        private:
            boost::uuids::detail::md5::digest_type _hashsum;
        protected:
            virtual bool isEqualTo(const HashSum&) const override;
            virtual bool isLessThan(const HashSum&) const override;
        public:
            virtual ~MD5Sum() = default;
            MD5Sum(const char* data, const size_t size);
    };
}