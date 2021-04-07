#ifndef MD5HASHING_H
#define MD5HASHING_H
#include <string>
#include <vector>

/* 
* \class MD5Hashing returned hash by message
*/
class MD5Hashing
{
public:
    MD5Hashing();
    ~MD5Hashing() = default;
    /**
    * \brief cypredMessage return hash from message
    */
    std::string cypredMessage(const std::string & str);
private:
    /**
    * \brief init vector of conts 
    */
    void initVectorOfConsts() noexcept;
    /**
    * \brief extend message on 1 bit and zerros by 448 mod 512 (56 mod 64)
    */
    const std::string extendedAlignment(const std::string & str) noexcept;
    /**
    * \brief extend message by 64 bits number of it length
    */
    const std::string extendedBitLength(const std::string & str) noexcept;
    /**
    * \brief processed cycling operation
    */
    std::string processHash(const std::vector<uint32_t> vector);
    /**
    * \brief represent part of hash to message format
    */
    std::string md5PartToString(uint32_t value) const;
    /**
    * \brief represent message to 32 bit's array for cycling 
    */
    std::vector<uint32_t> strToUintArray(const std::string & str) const;
    inline constexpr uint32_t F (uint32_t X,uint32_t Y,uint32_t Z) noexcept{return (X & Y) | (!X & Z);}
    inline constexpr uint32_t G (uint32_t X,uint32_t Y,uint32_t Z) noexcept{return (X & Z) | (!Z & Y);}
    inline constexpr uint32_t H (uint32_t X,uint32_t Y,uint32_t Z) noexcept{return X ^ Y ^ Z;}
    inline constexpr uint32_t I (uint32_t X,uint32_t Y,uint32_t Z) noexcept{return Y ^ (X | !Z);}

    static constexpr size_t vectorOfConstsSize_ = 64;
    inline uint32_t rotateToLeft(uint32_t x,uint32_t n) const {return (x << n) | (x >> (32 - n));}
    std::vector<uint32_t> vectorOfConsts_;
};

#endif // MD5CYPRED_H
