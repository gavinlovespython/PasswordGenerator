#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>

// ANSI colors
const std::string GREEN = "\033[1;32m";
const std::string BLUE  = "\033[1;34m";
const std::string RED   = "\033[1;31m";
const std::string RESET = "\033[0m";

// Global RNG (better practice)
std::mt19937& rng() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

bool get_yes_no(const std::string& question) {
    char choice;
    std::cout << question << " (y/n): ";
    std::cin >> choice;
    return choice == 'y' || choice == 'Y';
}

std::string generate_password(
    int length,
    bool use_lower,
    bool use_upper,
    bool use_digits,
    bool use_symbols
) {
    const std::string lower   = "abcdefghijklmnopqrstuvwxyz";
    const std::string upper   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string digits  = "0123456789";
    const std::string symbols = "!@#$%^&*()-_=+[]{};:,.<>?";

    std::vector<std::string> pools;
    std::string charset;

    if (use_lower)   pools.push_back(lower),   charset += lower;
    if (use_upper)   pools.push_back(upper),   charset += upper;
    if (use_digits)  pools.push_back(digits),  charset += digits;
    if (use_symbols) pools.push_back(symbols), charset += symbols;

    if (pools.empty())
        throw std::runtime_error("No character sets selected.");

    if (length < static_cast<int>(pools.size()))
        throw std::runtime_error("Password length too short for selected options.");

    std::uniform_int_distribution<> dist(0, charset.size() - 1);
    std::string password;

    // Guarantee one char per selected category
    for (const auto& pool : pools) {
        std::uniform_int_distribution<> pool_dist(0, pool.size() - 1);
        password += pool[pool_dist(rng())];
    }

    // Fill the rest
    while (password.size() < static_cast<size_t>(length)) {
        password += charset[dist(rng())];
    }

    // Shuffle password
    std::shuffle(password.begin(), password.end(), rng());

    return password;
}

int main() {
    int length;

    std::cout << BLUE << "=== Secure Password Generator v2.0 ===\n" << RESET;
    std::cout << "Password length: ";
    std::cin >> length;

    if (length <= 0) {
        std::cout << RED << "Invalid length.\n" << RESET;
        return 1;
    }

    bool lower   = get_yes_no("Include lowercase letters");
    bool upper   = get_yes_no("Include uppercase letters");
    bool digits  = get_yes_no("Include numbers");
    bool symbols = get_yes_no("Include symbols");

    try {
        std::string password = generate_password(
            length, lower, upper, digits, symbols
        );

        std::cout << GREEN << "\nGenerated password:\n"
                  << password << RESET << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << RED << "Error: " << e.what() << RESET << "\n";
        return 1;
    }

    std::cout << BLUE << "\nSupport me if you like this tool:\n"
              << "PayPal: https://www.paypal.com/paypalme/EnricoArama"
              << RESET << "\n";

    std::cout << "\nPress ENTER to exit...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return 0;
}
