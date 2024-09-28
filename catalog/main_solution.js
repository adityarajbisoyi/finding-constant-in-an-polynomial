// Function to convert a value from a given base to base 10
function convertToBase10(value, base) {
    let result = 0;
    let power = 1;

    for (let i = value.length - 1; i >= 0; --i) {
        let digit = parseInt(value[i], base);
        result += digit * power;
        power *= base;
    }

    return result;
}

// Function to perform Lagrange interpolation and find the constant term (c)
function lagrangeInterpolation(points, k) {
    const li = (i) => {
        let basis = 1.0;
        let xi = points[i][0];
        
        for (let j = 0; j < k; ++j) {
            if (i !== j) {
                let xj = points[j][0];
                basis *= -xj / (xi - xj);
            }
        }
        return basis;
    };

    let f0 = 0.0;
    for (let i = 0; i < k; ++i) {
        let yi = points[i][1];
        f0 += yi * li(i);
    }

    return f0;
}

// Function to process the shares and decode them
function processShares(data, n, k) {
    let points = [];

    for (let i = 1; i <= n; ++i) {
        let key = i.toString();
        if (data[key]) {
            let base = parseInt(data[key]["base"]);
            let value = data[key]["value"];
            let decodedValue = convertToBase10(value, base);
            points.push([i, decodedValue]);

            if (points.length === k) break;
        }
    }

    return points;
}

// Main function to run the example
function main() {
    // Example input in the provided format
    const data = {
        "keys": { "n": "4", "k": "3" },
        "1": { "base": "10", "value": "4" },
        "2": { "base": "2", "value": "111" },
        "3": { "base": "10", "value": "12" },
        "6": { "base": "4", "value": "213" }
    };

    // Extract n and k
    const n = parseInt(data["keys"]["n"]);
    const k = parseInt(data["keys"]["k"]);

    // Process the shares to get the (x, y) points
    const points = processShares(data, n, k);

    // Perform Lagrange interpolation to find the constant term c
    const constantTerm = lagrangeInterpolation(points, k);

    // Output the result
    console.log("Constant term (c):", constantTerm);
}

// Run the main function
main();
