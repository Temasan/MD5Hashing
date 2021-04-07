#include "md5hashing.h"
#include "boost/endian/conversion.hpp"
#include <math.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <bitset>

MD5Hashing::MD5Hashing(){
    initVectorOfConsts();
}

void MD5Hashing::initVectorOfConsts() noexcept{
    const double twoConst = std::pow(2,32);
    for(size_t i = 0; i < vectorOfConstsSize_; ++i){
        vectorOfConsts_.push_back(uint32_t(twoConst*std::abs(std::sin(float(i+1)))));
    }
}

std::string MD5Hashing::cypredMessage(const std::string & str){
    std::string result = str;
        // 1
    result.append(extendedAlignment(str));
        // 2
    result.append(extendedBitLength(str));
        // 3
        // 4
    while (result.size() % 4 != 0) result += '0';
    return processHash((strToUintArray(result)));
        // 5
}

const std::string MD5Hashing::extendedAlignment(const std::string & str) noexcept{
    std::string result = "";
    result += 128;
    int increasingZerros = 56 - (str.size() + result.size())%64;
    if(increasingZerros < 0) increasingZerros = 56 - increasingZerros;
    for(int i = 0; i < increasingZerros; ++i) result += '0';
    return result;
}

const std::string MD5Hashing::extendedBitLength(const std::string & str) noexcept{
    uint64_t bitLength = str.length()*8;
    uint64_t littleEnding = boost::endian::endian_reverse(bitLength);
    std::stringstream sstream;
    sstream << std::hex << littleEnding;
    std::string res = sstream.str();
    return res;
}

std::vector<uint32_t> MD5Hashing::strToUintArray(const std::string & str) const{
    std::size_t strSize = str.size();
    std::vector<uint32_t> result(strSize/4);
    for(uint64_t i = 0; i < str.size(); i+=4){
        uint32_t number = (uint32_t(str[i+0]) << 0)
        + (uint32_t(str[i+1]) << 8)
        + (uint32_t(str[i+2]) << 16)
        + (uint32_t(str[i+3]) << 24);
        result[i/4] = number;
    }
    size_t appendedValue = result.size()%16;
    result.insert(result.end(),16-appendedValue, 0);
    return std::move(result);
}

std::string MD5Hashing::processHash(const std::vector<uint32_t> vector){
    uint32_t A = 0x67452301;
    uint32_t B = 0xEFCDAB89;
    uint32_t C = 0x98BADCFE;
    uint32_t D = 0x10325476;
    for(size_t i = 0; i < vector.size(); i+=16){
        uint32_t AA = A,BB = B,CC = C,DD = D;
        // Round 1
        A = B + rotateToLeft(A + F(B,C,D)+vector[i+0]+vectorOfConsts_[0],7);
        D = A + rotateToLeft(D + F(A,B,C)+vector[i+1]+vectorOfConsts_[1],12);
        C = D + rotateToLeft(C + F(D,A,B)+vector[i+2]+vectorOfConsts_[2],17);
        B = C + rotateToLeft(B + F(C,D,A)+vector[i+3]+vectorOfConsts_[3],22);

        A = B + rotateToLeft(A + F(B,C,D)+vector[i+4]+vectorOfConsts_[4],7);
        D = A + rotateToLeft(D + F(A,B,C)+vector[i+5]+vectorOfConsts_[5],12);
        C = D + rotateToLeft(C + F(D,A,B)+vector[i+6]+vectorOfConsts_[6],17);
        B = C + rotateToLeft(B + F(C,D,A)+vector[i+7]+vectorOfConsts_[7],22);

        A = B + rotateToLeft(A + F(B,C,D)+vector[i+8]+vectorOfConsts_[8],7);
        D = A + rotateToLeft(D + F(A,B,C)+vector[i+9]+vectorOfConsts_[9],12);
        C = D + rotateToLeft(C + F(D,A,B)+vector[i+10]+vectorOfConsts_[10],17);
        B = C + rotateToLeft(B + F(C,D,A)+vector[i+11]+vectorOfConsts_[11],22);

        A = B + rotateToLeft(A + F(B,C,D)+vector[i+12]+vectorOfConsts_[12],7);
        D = A + rotateToLeft(D + F(A,B,C)+vector[i+13]+vectorOfConsts_[13],12);
        C = D + rotateToLeft(C + F(D,A,B)+vector[i+14]+vectorOfConsts_[14],17);
        B = C + rotateToLeft(B + F(C,D,A)+vector[i+15]+vectorOfConsts_[15],22);

        // Round 2
        A = B + rotateToLeft(A + G(B,C,D)+vector[i+1]+vectorOfConsts_[16],5);
        D = A + rotateToLeft(D + G(A,B,C)+vector[i+6]+vectorOfConsts_[17],9);
        C = D + rotateToLeft(C + G(D,A,B)+vector[i+11]+vectorOfConsts_[18],14);
        B = C + rotateToLeft(B + G(C,D,A)+vector[i+0]+vectorOfConsts_[19],20);

        A = B + rotateToLeft(A + G(B,C,D)+vector[i+5]+vectorOfConsts_[20],5);
        D = A + rotateToLeft(D + G(A,B,C)+vector[i+10]+vectorOfConsts_[21],9);
        C = D + rotateToLeft(C + G(D,A,B)+vector[i+15]+vectorOfConsts_[22],14);
        B = C + rotateToLeft(B + G(C,D,A)+vector[i+4]+vectorOfConsts_[23],20);

        A = B + rotateToLeft(A + G(B,C,D)+vector[i+9]+vectorOfConsts_[24],5);
        D = A + rotateToLeft(D + G(A,B,C)+vector[i+14]+vectorOfConsts_[25],9);
        C = D + rotateToLeft(C + G(D,A,B)+vector[i+3]+vectorOfConsts_[26],14);
        B = C + rotateToLeft(B + G(C,D,A)+vector[i+8]+vectorOfConsts_[27],20);

        A = B + rotateToLeft(A + G(B,C,D)+vector[i+13]+vectorOfConsts_[28],5);
        D = A + rotateToLeft(D + G(A,B,C)+vector[i+2]+vectorOfConsts_[29],9);
        C = D + rotateToLeft(C + G(D,A,B)+vector[i+7]+vectorOfConsts_[30],14);
        B = C + rotateToLeft(B + G(C,D,A)+vector[i+12]+vectorOfConsts_[31],20);

        // Round 3
        A = B + rotateToLeft(A + H(B,C,D)+vector[i+5]+vectorOfConsts_[32],4);
        D = A + rotateToLeft(D + H(A,B,C)+vector[i+8]+vectorOfConsts_[33],11);
        C = D + rotateToLeft(C + H(D,A,B)+vector[i+11]+vectorOfConsts_[34],16);
        B = C + rotateToLeft(B + H(C,D,A)+vector[i+4]+vectorOfConsts_[35],23);

        A = B + rotateToLeft(A + H(B,C,D)+vector[i+1]+vectorOfConsts_[36],4);
        D = A + rotateToLeft(D + H(A,B,C)+vector[i+4]+vectorOfConsts_[37],11);
        C = D + rotateToLeft(C + H(D,A,B)+vector[i+7]+vectorOfConsts_[38],16);
        B = C + rotateToLeft(B + H(C,D,A)+vector[i+10]+vectorOfConsts_[39],23);

        A = B + rotateToLeft(A + H(B,C,D)+vector[i+13]+vectorOfConsts_[40],4);
        D = A + rotateToLeft(D + H(A,B,C)+vector[i+0]+vectorOfConsts_[41],11);
        C = D + rotateToLeft(C + H(D,A,B)+vector[i+3]+vectorOfConsts_[42],16);
        B = C + rotateToLeft(B + H(C,D,A)+vector[i+6]+vectorOfConsts_[43],23);

        A = B + rotateToLeft(A + H(B,C,D)+vector[i+9]+vectorOfConsts_[44],4);
        D = A + rotateToLeft(D + H(A,B,C)+vector[i+12]+vectorOfConsts_[45],11);
        C = D + rotateToLeft(C + H(D,A,B)+vector[i+15]+vectorOfConsts_[46],16);
        B = C + rotateToLeft(B + H(C,D,A)+vector[i+2]+vectorOfConsts_[47],23);

        // Round 4
        A = B + rotateToLeft(A + I(B,C,D)+vector[i+0]+vectorOfConsts_[48],6);
        D = A + rotateToLeft(D + I(A,B,C)+vector[i+7]+vectorOfConsts_[49],10);
        C = D + rotateToLeft(C + I(D,A,B)+vector[i+14]+vectorOfConsts_[50],15);
        B = C + rotateToLeft(B + I(C,D,A)+vector[i+5]+vectorOfConsts_[51],21);

        A = B + rotateToLeft(A + I(B,C,D)+vector[i+12]+vectorOfConsts_[52],6);
        D = A + rotateToLeft(D + I(A,B,C)+vector[i+3]+vectorOfConsts_[53],10);
        C = D + rotateToLeft(C + I(D,A,B)+vector[i+10]+vectorOfConsts_[54],15);
        B = C + rotateToLeft(B + I(C,D,A)+vector[i+1]+vectorOfConsts_[55],21);

        A = B + rotateToLeft(A + I(B,C,D)+vector[i+8]+vectorOfConsts_[56],6);
        D = A + rotateToLeft(D + I(A,B,C)+vector[i+15]+vectorOfConsts_[57],10);
        C = D + rotateToLeft(C + I(D,A,B)+vector[i+6]+vectorOfConsts_[58],15);
        B = C + rotateToLeft(B + I(C,D,A)+vector[i+13]+vectorOfConsts_[59],21);

        A = B + rotateToLeft(A + I(B,C,D)+vector[i+4]+vectorOfConsts_[60],6);
        D = A + rotateToLeft(D + I(A,B,C)+vector[i+11]+vectorOfConsts_[61],10);
        C = D + rotateToLeft(C + I(D,A,B)+vector[i+2]+vectorOfConsts_[62],15);
        B = C + rotateToLeft(B + I(C,D,A)+vector[i+9]+vectorOfConsts_[63],21);

        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }
    return md5PartToString(A)+md5PartToString(B)+md5PartToString(C)+md5PartToString(D);
}

std::string MD5Hashing::md5PartToString(uint32_t value) const{
    std::string result = "";
    for(size_t i = 0; i < 4; ++i){
        std::stringstream stream;
        stream << std::hex << '0' << value%256;
        result += stream.str();
        value /= 256;
    }
    return result;
}




