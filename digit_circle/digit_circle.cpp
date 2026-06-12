#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>
#include <locale>
class BigNumber{
private:

    std::string value;
    bool valid;

    bool isValidNumber() const {
        if (value.empty()) {
            return false;
        }

        if (value.size() > 1 && value[0] == '0') {
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

bool isPossibleLength(int aLen, int bLen, int cLen) {
    int maxLen = std::max(aLen, bLen);

    return cLen == maxLen || cLen == maxLen + 1;
}

Equation createEquation(const std::string& ring, int aLen, int bLen) {
    int cLen = ring.size() - aLen - bLen;

    BigNumber a(ring.substr(0, aLen));
    BigNumber b(ring.substr(aLen, bLen));
    BigNumber c(ring.substr(aLen + bLen, cLen));

    return Equation(a, b, c);
}

std::string solveRing(const std::string& ring) {
    int n = ring.size();

    std::string doubled = ring + ring;


    for (int start = 0; start < n; start++) {

        std::string current = doubled.substr(start, n);


        for (int aLen = 1; aLen < n - 1; aLen++) {

            for (int bLen = 1; bLen < n - aLen; bLen++) {

                int cLen = n - aLen - bLen;


                if (!isPossibleLength(aLen, bLen, cLen)) {
                    continue;
                }


                Equation eq = createEquation(current, aLen, bLen);


                if (eq.check()) {
                    std::stringstream answer;

                    answer << eq;

                    return answer.str();
                }
            }
        }
    }

    return "No";
}

std::string getError(const std::string& ring) {
    if (ring.empty()) {
        return "Файл пуст";
    }


    if (ring.size() > 1000) {
        return "Слишком много знаков";
    }


    for (char c : ring) {

        if (!isdigit(c)) {
            return "В кольце есть не цифры";
        }
    }


    return "";
}

std::string readInput(const std::string& filename) {
    std::ifstream input(filename);

    if (!input.is_open()) {
        return "";
    }

    std::string ring;

    input >> ring;

    return ring;
}

bool writeOutput(const std::string& filename, const std::string& result) {
    std::ofstream output(filename);

    if (!output.is_open()) {
        return false;
    }

    output << result;

    return true;
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);

    return file.is_open();
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::string inputFile;
    std::string outputFile;


    std::cout << "Введите имя входного файла: ";
    std::cin >> inputFile;


    if (!fileExists(inputFile)) {
        std::cout << "Входной файл не найден";

        return 1;
    }


    std::cout << "Введите имя выходного файла: ";
    std::cin >> outputFile;


    std::string ring = readInput(inputFile);


    std::string error = getError(ring);


    if (!error.empty()) {

        writeOutput(outputFile, error);

        std::cout << "Ошибка: " << error;

        return 0;
    }


    std::string answer = solveRing(ring);


    if (!writeOutput(outputFile, answer)) {

        std::cout << "Не удалось создать выходной файл";

        return 1;
    }


    std::cout << "Программа завершена успешно";

    return 0;
}