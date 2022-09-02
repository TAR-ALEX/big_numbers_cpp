#pragma once

#include <deque>
#include <estd/BigInt.h>
#include <exception>
#include <iomanip>
#include <map>
#include <sstream>

namespace estd {
    class BigDecimal : public estd::BigInt {
        friend class BigInt;

    protected:
        size_t index = 0;

        void addTrailZeros(size_t z) {
            while (z % 9 != 0) {
                z -= 1;
                index += 1;
                BigInt::operator*=(10);
            }
            while (z > 0) {
                z -= 9;
                index += 9;
                number.push_back(0);
            }
        }

        BigDecimal& trimTrailingZeros() {
            while (index >= 9 && number.size() > 0 && number[number.size() - 1] == 0) {
                number.pop_back();
                index-=9;
            }
            while (index > 0 && number.size() > 0 && number[number.size() - 1] % 10 == 0) {
                number = BigInt::operator*(1000000000 / 10).number;
                number.pop_back();
                index--;
            }
            return *this;
        }

        size_t alignDecimals(BigDecimal& left, BigDecimal& right) const {
            if (left.index < right.index) {
                left.addTrailZeros(right.index - left.index);
            } else if (left.index > right.index) {
                right.addTrailZeros(left.index - right.index);
            }
            return right.index;
        }

    public:
        BigDecimal() { this->operator=(int64_t(0)); };

        template <typename T>
        BigDecimal(T val) {
            this->operator=(val);
        };

        BigDecimal& operator=(const char* strNum) { return operator=(std::string(strNum)); }

        BigDecimal& operator=(std::string strNum) {
            if (strNum.size() == 0) throw std::invalid_argument("Cannot parse BigDecimal");

            if (strNum[0] == '-') {
                isNegative = true;
                strNum = strNum.substr(1);
            }

            size_t pointIdx = strNum.length();
            for (size_t i = 0; i < strNum.length(); i++) {
                if (strNum[i] == '.') {
                    if (pointIdx != strNum.length()) throw std::invalid_argument("Cannot parse BigDecimal");
                    pointIdx = i;
                } else if (strNum[i] < '0' || strNum[i] > '9')
                    throw std::invalid_argument("Cannot parse BigDecimal");
            }

            if (pointIdx != strNum.length()) {
                strNum = strNum.substr(0, pointIdx) + strNum.substr(pointIdx + 1);
                index = strNum.length() - pointIdx;
            }

            if (strNum.size() == 0) throw std::invalid_argument("Cannot parse BigDecimal");

            BigInt::operator=(strNum);

            return trimTrailingZeros();
        }

        template <
            class T,
            typename std::enable_if<std::is_integral<T>::value>::type* = nullptr> // template for integer types
        BigInt& operator=(T n) {
            BigInt::operator=(n);
            index = 0;
            return *this;
        }

        BigDecimal& operator=(double n) {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(200) << n;
            return operator=(ss.str()).trimTrailingZeros();
        }

        BigDecimal& operator=(float n) {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(200) << n;
            return operator=(ss.str()).trimTrailingZeros();
        }

        //Operations
        BigDecimal operator+(BigDecimal right) const {
            BigDecimal left = *this;
            size_t idx = alignDecimals(left, right);
            BigDecimal result = left.BigInt::operator+(right);
            result.index = idx;
            return result.trimTrailingZeros();
        }

        BigDecimal& operator+=(const BigDecimal& right) { return (*this) = (*this) + right; }

        BigDecimal operator-(BigDecimal right) const {
            BigDecimal left = *this;
            size_t idx = alignDecimals(left, right);
            BigDecimal result = left.BigInt::operator-(right);
            result.index = idx;
            return result.trimTrailingZeros();
        }

        BigDecimal& operator-=(const BigDecimal& right) { return (*this) = (*this) - right; }

        BigDecimal operator*(BigDecimal right) const {
            const BigDecimal& left = *this;
            BigDecimal result = left.BigInt::operator*(right);
            result.index = left.index + right.index;
            return result.trimTrailingZeros();
        }

        BigDecimal& operator*=(const BigDecimal& right) { return (*this) = (*this) * right; }

        BigDecimal operator/(BigDecimal right) {
            BigDecimal left = *this;
            alignDecimals(left, right);
            BigDecimal result = left.BigInt::operator/(right);
            result.index = 0;
            return result.trimTrailingZeros();
        }

        BigDecimal& operator/=(const BigDecimal& right) { return (*this) = (*this) / right; }

        BigDecimal operator%(BigDecimal right) {
            BigDecimal left = *this;
            size_t idx = alignDecimals(left, right);
            BigDecimal result = left.BigInt::operator%(right);
            result.index = idx;
            return result.trimTrailingZeros();
        }

        BigDecimal& operator%=(const BigDecimal& right) { return (*this) = (*this) % right; }

        BigDecimal power(BigDecimal p) {
            if (p.index != 0) throw std::invalid_argument("Power cannot be a fraction");
            const BigDecimal& left = *this;
            BigDecimal result = left.BigInt::power(BigInt(p));
            result.index = left.index * p.toUint();
            return result.trimTrailingZeros();
        }

        BigDecimal operator++(int) {
            BigDecimal oldThis = *this;
            *this = *this + 1;
            return oldThis;
        };
        BigDecimal& operator++() {
            *this = *this + 1;
            return *this;
        };
        BigDecimal operator--(int) {
            BigDecimal oldThis = *this;
            *this = *this - 1;
            return oldThis;
        };
        BigDecimal& operator--() {
            *this = *this - 1;
            return *this;
        };

        bool operator==(BigDecimal right) const {
            BigDecimal left = *this;
            alignDecimals(left, right);
            return left.BigInt::operator==(right);
        }

        bool operator!=(BigDecimal right) const {
            BigDecimal left = *this;
            alignDecimals(left, right);
            return left.BigInt::operator!=(right);
        }

        bool operator>(BigDecimal right) const {
            BigDecimal left = *this;
            alignDecimals(left, right);
            return left.BigInt::operator>(right);
        }

        bool operator>=(BigDecimal right) const {
            BigDecimal left = *this;
            alignDecimals(left, right);
            return left.BigInt::operator>=(right);
        }

        bool operator<(BigDecimal right) const {
            BigDecimal left = *this;
            alignDecimals(left, right);
            return left.BigInt::operator<(right);
        }

        bool operator<=(BigDecimal right) const {
            BigDecimal left = *this;
            alignDecimals(left, right);
            return left.BigInt::operator<=(right);
        }

        // Im not sure if these are relevant 
        // just divide and multiply by 2 if needed
        // BigDecimal& operator<<=(const BigDecimal& right);
        // BigDecimal& operator>>=(const BigDecimal& right);
        // BigDecimal operator<<(const BigDecimal& right) const;
        // BigDecimal operator>>(const BigDecimal& right) const;

        friend std::ostream& operator<<(std::ostream& out, const BigDecimal& right) { return out << right.toString(); }
        friend std::istream& operator>>(std::istream& in, BigDecimal& right); //TODO

        // uint64_t toUint() const;
        // int64_t toInt() const;

        std::string toString() const {
            if (number.size() == 0) return "nan";
            std::string str = BigInt::toString();

            if (index != 0) {
                while (str.length() <= index) { str = std::string("0") + str; }
                str = str.substr(0, str.length() - index) + "." + str.substr(str.length() - index);
            }
            return str;
        }

        size_t getNumDigits();

        // TODO:
        // DEFINE_BIN_OP(^) // makes no sense for long ints, will just be all 1s  if we assume leading zeros
        // DEFINE_BIN_OP(&) // can be done, but hard for the stored format, will be slow
        // DEFINE_BIN_OP(|) // can be done, but hard for the stored format, will be slow
        // DEFINE_UNARY_OP(~) // makes no sense for long ints, will just be all 1s if we assume leading zeros
        // DEFINE_BIN_OP(^=) // makes no sense for long ints, will just be all 1s  if we assume leading zeros
        // DEFINE_BIN_OP(&=) // can be done, but hard for the stored format, will be slow
        // DEFINE_BIN_OP(|=) // can be done, but hard for the stored format, will be slow
    };

    BigInt::operator BigDecimal() {
        BigDecimal r;
        r.number = number;
        r.isNegative = isNegative;
        return r;
    }

    // BigInt BigInt::power(BigDecimal p) const {
    //     if (p.index != 0) throw std::invalid_argument("Power cannot be a fraction");
    //     return BigInt::power(BigInt(p));
    // }
} // namespace estd