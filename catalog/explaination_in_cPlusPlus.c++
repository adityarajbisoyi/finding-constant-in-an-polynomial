#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>

// Function to convert a value from a given base to base 10
int convertToBase10(const std::string& value, int base) {
    int result = 0;  // Resulting base 10 value
    int power = 1;   // Power of the base

    // Iterate over each digit in reverse order
    for (int i = value.size() - 1; i >= 0; --i) {
        // Convert character digit to integer and calculate its base 10 equivalent
        result += (value[i] - '0') * power;
        power *= base;  // Increase power of the base for the next digit
    }
    
    return result;  // Return the final base 10 result
}

// Function to perform Lagrange interpolation and find the constant term (c)
double lagrangeInterpolation(const std::vector<std::pair<int, int>>& points, int k) {
    // Lambda function to calculate the Lagrange basis polynomial L_i
    auto li = [&](int i) -> double {
        double basis = 1.0;   // Initialize the basis polynomial
        int xi = points[i].first;  // x-coordinate for the ith point

        // Calculate L_i for the current point
        for (int j = 0; j < k; ++j) {
            if (i != j) {  // Skip the current point
                int xj = points[j].first;  // x-coordinate of the jth point
                // Calculate the basis polynomial L_i
                basis *= -xj / static_cast<double>(xi - xj);
            }
        }
        return basis;  // Return the computed basis
    };

    double f0 = 0.0;  // Initialize the result of the polynomial at x=0
    // Calculate the polynomial value at x=0 using Lagrange interpolation
    for (int i = 0; i < k; ++i) {
        int yi = points[i].second;  // y-coordinate for the ith point
        f0 += yi * li(i);  // Accumulate the contribution of each point
    }

    return f0;  // Return the constant term (c)
}

// Function to process the shares and decode them into (x, y) points
std::vector<std::pair<int, int>> processShares(const std::map<std::string, std::map<std::string, std::string>>& data, int n, int k) {
    std::vector<std::pair<int, int>> points;  // Vector to store the (x, y) points

    // Iterate through the shares based on the total number of shares (n)
    for (int i = 1; i <= n; ++i) {
        std::string key = std::to_string(i);  // Convert the index to string for lookup
        if (data.find(key) != data.end()) {  // Check if the key exists in the data map
            // Retrieve the base and value, and convert value to base 10
            int base = std::stoi(data.at(key).at("base"));  // Convert base to int
            std::string value = data.at(key).at("value");   // Get the value in string format
            int decoded_value = convertToBase10(value, base);  // Decode the value
            points.push_back({i, decoded_value});  // Add (x, y) point to the vector
            
            // Break the loop if we have collected enough points (k)
            if (points.size() == k) break;
        }
    }

    return points;  // Return the vector of points
}

int main() {
    // Example input in the provided format
    std::map<std::string, std::map<std::string, std::string>> data = {
        {"keys", {{"n", "4"}, {"k", "3"}}},
        {"1", {{"base", "10"}, {"value", "4"}}},
        {"2", {{"base", "2"}, {"value", "111"}}},
        {"3", {{"base", "10"}, {"value", "12"}}},
        {"6", {{"base", "4"}, {"value", "213"}}}
    };

    // Extract the number of shares (n) and the minimum number of shares required (k)
    int n = std::stoi(data["keys"]["n"]);
    int k = std::stoi(data["keys"]["k"]);

    // Process the shares to get the (x, y) points
    std::vector<std::pair<int, int>> points = processShares(data, n, k);

    // Perform Lagrange interpolation to find the constant term (c)
    double constant_term = lagrangeInterpolation(points, k);

    // Output the result
    std::cout << "Constant term (c): " << constant_term << std::endl;

    return 0;
}
