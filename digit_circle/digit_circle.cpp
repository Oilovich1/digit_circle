#include <iostream>
#include <string>
#include <cctype>
#include <fstream>

class BigNumber{
private:

    std::string value;
    bool valid;

    bool isValidNumber() const {
        if (value.empty()) {
            return false;
        }

        for (char c : value) {
            if (!isdigit(c)) {
                return false;
            }
        }

        return true;
    }
public:

    BigNumber(std::string number){
        value = number;
        valid = isValidNumber();
    }

    bool isValid() const{
        return valid;
	}

    std::string getValue() const{
        return value;
    }

    bool hasLeadingZero() const{
        return value.size() > 1 &&
            value[0] == '0';
    }

    BigNumber add(const BigNumber& other) const{
        std::string result;

        int i = value.size() - 1;
        int j = other.value.size() - 1;

        int carry = 0;

        while (i >= 0 || j >= 0 || carry){
            int sum = carry;

            if (i >= 0){
                sum += value[i] - '0';
                i--;
            }

            if (j >= 0){
                sum += other.value[j] - '0';
                j--;
            }

            result = char(sum % 10 + '0')
                + result;

            carry = sum / 10;
        }


        return BigNumber(result);
    }
};

class Equation {
private:
    BigNumber first;
    BigNumber second;
    BigNumber result;

public:

    Equation(
        BigNumber a,
        BigNumber b,
        BigNumber c
    ) : first(a), second(b), result(c) {
    }


    bool check() const {
        if (!first.isValid() ||
            !second.isValid() ||
            !result.isValid()) {
            return false;
        }

        BigNumber sum = first.add(second);

        return sum.getValue() == result.getValue();
    }

    friend std::ostream& operator<<(std::ostream& out, const Equation& eq);

};

std::ostream& operator<<(std::ostream& out, const Equation& eq) {
    out << eq.first.getValue() << "+"
        << eq.second.getValue() << "="
        << eq.result.getValue();
    return out;
}