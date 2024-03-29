#pragma once

#include <deque>
#include <estd/BigInteger.h>
#include <exception>
#include <iomanip>
#include <map>
#include <sstream>

namespace estd {
    class BigDecimal {
        friend class BigInteger;

    protected:
        estd::BigInteger parent = 0;
        size_t index = 0; // this defines how many digits in base 10 are after the decimal point.

        inline void addTrailZeros(size_t z) {
            while (z % 9 != 0) {
                z -= 1;
                index += 1;
                parent.operator*=(10);
            }
            while (z > 0) {
                z -= 9;
                index += 9;
                parent.number.push_back(0);
            }
        }

        inline BigDecimal& removeAllDecimals() {
            while (index >= 9 && parent.number.size() > 0) {
                parent.number.pop_back();
                index -= 9;
            }
            while (index > 0 && parent.number.size() > 0) {
                parent.number = parent.operator*(1000000000 / 10).number;
                parent.number.pop_back();
                index--;
            }
            if (parent.number.size() == 0) {
                index = 0;
                parent.number.push_back(0);
            }

            return *this;
        }

        inline BigDecimal& trimTrailingZeros() {
            while (index >= 9 && parent.number.size() > 1 && parent.number[parent.number.size() - 1] == 0) {
                parent.number.pop_back();
                index -= 9;
            }
            while (index > 0 && parent.number.size() > 0 && parent.number[parent.number.size() - 1] % 10 == 0) {
                if (parent.number.size() == 1 &&
                    parent.number[0] == 0) { //special case, this is a zero, return to avoid removing everything
                    index = 0;
                    break;
                }
                parent.number = parent.operator*(1000000000 / 10).number; // great way to divide by 10
                parent.number.pop_back();
                index--;
            }
            return *this;
        }

        inline size_t alignDecimals(BigDecimal& left, BigDecimal& right) const {
            if (left.index < right.index) {
                left.addTrailZeros(right.index - left.index);
            } else if (left.index > right.index) {
                right.addTrailZeros(left.index - right.index);
            }
            return right.index;
        }

    public:
        inline BigDecimal() { this->operator=(int64_t(0)); };
        inline BigDecimal(std::nullptr_t) : parent(nullptr){};
        inline BigDecimal(std::string val) { this->operator=(val); }
        inline BigDecimal(const char* val) { this->operator=(val); }
        inline BigDecimal(float val) { this->operator=(val); }
        inline BigDecimal(double val) { this->operator=(val); }
        // template for integer types
        template <class T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
        inline BigDecimal(T val) {
            this->operator=(val);
        };

        inline BigDecimal& operator=(const char* strNum) { return operator=(std::string(strNum)); }

        inline void parsePositiveBase10(std::string strNum) {
            if (strNum.size() == 0) throw std::invalid_argument("Cannot parse BigDecimal");

            size_t pointIdx = strNum.length();
            for (size_t i = 0; i < strNum.length(); i++) {
                if (strNum[i] == '.') {
                    // if there is more than 1 point
                    if (pointIdx != strNum.length()) throw std::invalid_argument("Cannot parse BigDecimal");
                    pointIdx = i;
                }
            }

            if (pointIdx != strNum.length()) {
                strNum = strNum.substr(0, pointIdx) + strNum.substr(pointIdx + 1);
                index = strNum.length() - pointIdx;
            }

            if (strNum.size() == 0) throw std::invalid_argument("Cannot parse BigDecimal");

            parent.operator=(strNum);
        }

        inline void parsePositiveBaseN(const uint8_t base, std::string strNum) {
            if (strNum.size() <= 2) throw std::invalid_argument("Cannot parse BigDecimal");

            std::string prefix = strNum.substr(0, 2);

            size_t pointIdx = strNum.length();
            for (size_t i = 0; i < strNum.length(); i++) {
                if (strNum[i] == '.') {
                    // if there is more than 1 point
                    if (pointIdx != strNum.length()) throw std::invalid_argument("Cannot parse BigDecimal");
                    pointIdx = i;
                }
            }

            std::string leftNum = strNum.substr(0, pointIdx);

            parent.operator=(leftNum);

            std::string rightNum = "";
            if (strNum.length() != pointIdx) {
                rightNum = prefix + strNum.substr(pointIdx + 1);

                BigDecimal dec = BigInteger(rightNum);
                BigDecimal numDiv2 = rightNum.size() - 2;
                if (base == 8) numDiv2 *= 3;
                else if (base == 16)
                    numDiv2 *= 4;
                *this += dec * BigDecimal("0.5").power(numDiv2);
            }

            if (leftNum.size() == 0 && rightNum.size() == 0) throw std::invalid_argument("Cannot parse BigDecimal");
        }


        inline BigDecimal& operator=(std::string strNum) {
            if (strNum.size() == 0) throw std::invalid_argument("Cannot parse BigDecimal");

            bool isNegative = false;

            if (strNum[0] == '-') {
                isNegative = true;
                strNum = strNum.substr(1);
            }

            if (strNum.substr(0, 2) == "0x") {
                parsePositiveBaseN(16, strNum);
            } else if (strNum.substr(0, 2) == "0o") {
                parsePositiveBaseN(8, strNum);
            } else if (strNum.substr(0, 2) == "0b") {
                parsePositiveBaseN(2, strNum);
            } else {
                parsePositiveBase10(strNum);
            }

            parent.isNegative = isNegative;

            return trimTrailingZeros();
        }
        // template for integer types
        template <class T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
        inline BigDecimal& operator=(T n) {
            parent.operator=(n);
            index = 0;
            return *this;
        }

        inline BigDecimal& operator=(double n) {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(200) << n;
            return operator=(ss.str()).trimTrailingZeros();
        }

        inline BigDecimal& operator=(float n) {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(200) << n;
            return operator=(ss.str()).trimTrailingZeros();
        }

        //Operations
        inline BigDecimal operator+(BigDecimal right) const {
            BigDecimal left = *this;
            size_t idx = alignDecimals(left, right);
            BigDecimal result = left.parent.operator+(right.parent);
            result.index = idx;
            return result.trimTrailingZeros();
        }

        inline BigDecimal& operator+=(const BigDecimal& right) { return (*this) = (*this) + right; }

        inline BigDecimal operator-(BigDecimal right) const {
            BigDecimal left = *this;
            size_t idx = alignDecimals(left, right);
            BigDecimal result = left.parent.operator-(right.parent);
            result.index = idx;
            return result.trimTrailingZeros();
        }

        inline BigDecimal& operator-=(const BigDecimal& right) { return (*this) = (*this) - right; }

        inline BigDecimal operator*(BigDecimal right) const {
            const BigDecimal& left = *this;
            BigDecimal result = left.parent.operator*(right.parent);
            result.index = left.index + right.index;
            return result.trimTrailingZeros();
        }

        inline BigDecimal& operator*=(const BigDecimal& right) { return (*this) = (*this) * right; }

        inline BigDecimal operator/(BigDecimal right) {
            BigDecimal left = *this;
            alignDecimals(left, right);
            BigDecimal result = left.parent.operator/(right.parent);
            result.index = 0;
            return result.trimTrailingZeros();
        }

        inline BigDecimal& operator/=(const BigDecimal& right) { return (*this) = (*this) / right; }

        inline BigDecimal operator%(BigDecimal right) {
            BigDecimal left = *this;
            size_t idx = alignDecimals(left, right);
            BigDecimal result = left.parent.operator%(right.parent);
            result.index = idx;
            return result.trimTrailingZeros();
        }

        inline BigDecimal& operator%=(const BigDecimal& right) { return (*this) = (*this) % right; }

        inline BigDecimal power(BigDecimal p) {
            if (p.index != 0) throw std::invalid_argument("Power cannot be a fraction");
            const BigDecimal& left = *this;
            BigDecimal result = left.parent.power(p.parent);
            result.index = left.index * BigInteger(p).toUint();
            return result.trimTrailingZeros();
        }

        inline BigDecimal operator++(int) {
            BigDecimal oldThis = *this;
            *this = *this + 1;
            return oldThis;
        };
        inline BigDecimal& operator++() {
            *this = *this + 1;
            return *this;
        };
        inline BigDecimal operator--(int) {
            BigDecimal oldThis = *this;
            *this = *this - 1;
            return oldThis;
        };
        inline BigDecimal& operator--() {
            *this = *this - 1;
            return *this;
        };

        inline bool operator==(BigDecimal right) const {
            BigDecimal left = *this;
            alignDecimals(left, right);
            return left.parent.operator==(right.parent);
        }

        inline bool operator!=(BigDecimal right) const {
            BigDecimal left = *this;
            alignDecimals(left, right);
            return left.parent.operator!=(right.parent);
        }

        inline bool operator>(BigDecimal right) const {
            BigDecimal left = *this;
            alignDecimals(left, right);
            return left.parent.operator>(right.parent);
        }

        inline bool operator>=(BigDecimal right) const {
            BigDecimal left = *this;
            alignDecimals(left, right);
            return left.parent.operator>=(right.parent);
        }

        inline bool operator<(BigDecimal right) const {
            BigDecimal left = *this;
            alignDecimals(left, right);
            return left.parent.operator<(right.parent);
        }

        inline bool operator<=(BigDecimal right) const {
            BigDecimal left = *this;
            alignDecimals(left, right);
            return left.parent.operator<=(right.parent);
        }

        // Im not sure if these are relevant
        // just divide and multiply by 2 if needed
        // BigDecimal& operator<<=(const BigDecimal& right);
        // BigDecimal& operator>>=(const BigDecimal& right);
        // BigDecimal operator<<(const BigDecimal& right) const;
        // BigDecimal operator>>(const BigDecimal& right) const;

        inline friend std::ostream& operator<<(std::ostream& out, const BigDecimal& right) {
            return out << right.toString();
        }
        inline friend std::istream& operator>>(std::istream& in, BigDecimal& right); //TODO

        inline uintmax_t toUint() const { return toBigInt().toUint(); }
        inline intmax_t toInt() const { return toBigInt().toInt(); }
        // template for integer types
        template <class T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
        inline explicit operator T() {
            if constexpr (std::is_unsigned<T>::value) {
                return toUint();
            } else {
                return toInt();
            }
        }

        inline std::string toString() const {
            if (parent.number.size() == 0) return "nan";
            std::string str = parent.toString();

            if (index != 0) {
                while (str.length() <= index) { str = std::string("0") + str; }
                str = str.substr(0, str.length() - index) + "." + str.substr(str.length() - index);
            }
            return str;
        }

        inline BigInteger toBigInt() const {
            if (index == 0) {
                return parent;
            } else {
                // throw std::runtime_error("Not implementd, must truncate");
                BigDecimal r = (*this);
                r.removeAllDecimals();
                return r.parent;
            }
        }

        inline explicit operator BigInteger() const { return toBigInt(); }

        // TODO:
        // DEFINE_BIN_OP(^) // makes no sense for long ints, will just be all 1s  if we assume leading zeros
        // DEFINE_BIN_OP(&) // can be done, but hard for the stored format, will be slow
        // DEFINE_BIN_OP(|) // can be done, but hard for the stored format, will be slow
        // DEFINE_UNARY_OP(~) // makes no sense for long ints, will just be all 1s if we assume leading zeros
        // DEFINE_BIN_OP(^=) // makes no sense for long ints, will just be all 1s  if we assume leading zeros
        // DEFINE_BIN_OP(&=) // can be done, but hard for the stored format, will be slow
        // DEFINE_BIN_OP(|=) // can be done, but hard for the stored format, will be slow
    };

    typedef BigDecimal BigDec;

    inline BigInteger::operator BigDecimal() {
        BigDecimal r;
        r.parent.number = number;
        r.parent.isNegative = isNegative;
        return r;
    }
} // namespace estd