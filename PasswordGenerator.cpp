#include <iostream>
#include <string>
#include <random>

const std::string GREEN = "\033[1;32m";
const std::string BLUE  = "\033[1;34m";
const std::string RESET = "\033[0m";

std::string generate_password(
    int length,
    bool use_lower = true,
    bool use_upper = true,
    bool use_digits = true,
    bool use_symbols = true
) {
    const std::string lower   = "abcdefghijklmnopqrstuvwxyz";
    const std::string upper   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string digits  = "0123456789";
    const std::string symbols = "!@#$%^&*()-_=+[]{};:,.<>?";

    std::string charset;
    if (use_lower)   charset += lower;
    if (use_upper)   charset += upper;
    if (use_digits)  charset += digits;
    if (use_symbols) charset += symbols;

    if (charset.empty()) {
        throw std::runtime_error("No character set selected");
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> dist(0, charset.size() - 1);

    std::string password;
    for (int i = 0; i < length; ++i) {
        password += charset[dist(generator)];
    }

    return password;
}

int main() {
    int length = 0;

    std::cout << "How many characters do you want your password to be? ";
    std::cin >> length;

    if(length <= 0){
        std::cout << "Invalid length. Exiting." << std::endl;
        return 1;
    }

    try {
        std::string password = generate_password(length);
        std::cout << GREEN << "\nYour generated password:\n" << password << RESET << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << BLUE << "\nSupport me if you like this tool:" << std::endl;
    std::cout << "PayPal: https://www.paypal.com/paypalme/EnricoArama" << RESET << std::endl;

    std::cout << "\nPress ENTER to exit...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::cin.get(); 

    return 0;
}
