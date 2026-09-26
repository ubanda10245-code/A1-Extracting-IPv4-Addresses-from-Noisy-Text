#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Bundles the three values extractIPv4 needs to hand back to main.
typedef struct {
    char address[16];   // "255.255.255.255" + null, or "-1" if invalid address
    unsigned int decimalValue;   // 32-bit decimal value of the address
    unsigned int port;   // port number, or -1 if port was not given
} IPv4Address;

IPv4Address extractIPv4(const char *input) {

    // Set default values (ADDRESS NOT FOUND)
    IPv4Address result;
    strcpy(result.address, "-1");
    result.decimalValue = 0;
    result.port = -1;

    // --- Find the first octet of the address ---
    int pos = 0;
    int start = -1;

    while (input[pos] != '\0') {
        // Find the first digit [0-9]
        if (isdigit((unsigned char)input[pos])) {
            int octetStart = pos;
            // Search through more digits
            while (isdigit((unsigned char)input[pos])) {
                pos++;
            }
            // Find the end of the first octet
            if (input[pos] == '.') {
                start = octetStart;
                break; // Stop search
            }
        } else { // Not a digit, move to the next character and startover
            pos++;
        }
    }
    // CASE: Never found the first octet of the address
    if (start == -1) {
        return result; 
    }

    // --- Check all four octets for validity ---
    pos = start; // position of the first octet digit
    int octetValues[4]; // array to store all four octet values

    for (int octetIndex = 0; octetIndex < 4; octetIndex++) {
        // Mark the start of the current octet
        int digitStart = pos;
        // Count the number of digits in the octet
        while (isdigit((unsigned char)input[pos])) {
            pos++;
        }
        int digitCount = pos - digitStart;
 
        // CASE: Empty octet or too many digits
        if (digitCount == 0 || digitCount > 3) {
            return result; 
        }
 
        // CASE: Leading zero (ex. "01")
        if (digitCount > 1 && input[digitStart] == '0') {
            return result;
        }
 
        // Calculate the numeric value of the octet
        int value = 0;
        for (int k = 0; k < digitCount; k++) {
            value = value * 10 + (input[digitStart + k] - '0');
        }
        
        // CASE: Value is over 255
        if (value > 255) {
            return result;
        }
        
        // Current octet is valid, store its value
        octetValues[octetIndex] = value;
        
        // Check if the current octet ends in a dot
        // (only for the first three octets)
        if (octetIndex < 3) {
            if (input[pos] != '.') {
                // CASE: Octet not followed by a dot
                return result;
            }
            pos++;
        }
    }

    // --- Check for OPTIONAL port ---
    int port = -1;

    if (input[pos] == ':') {
        pos++;
 
        int portStart = pos;
        while (isdigit((unsigned char)input[pos])) {
            pos++;
        }
        int digitCount = pos - portStart;
 
        // CASE: Empty port or too many digits
        if (digitCount == 0 || digitCount > 5) {
            return result;
        }
        
        // CASE: Leading zero (ex. "01")
        if (digitCount > 1 && input[portStart] == '0') {
            return result;
        }
        
        // Calculate the numeric value of the port
        int value = 0;
        for (int k = 0; k < digitCount; k++) {
            value = value * 10 + (input[portStart + k] - '0');
        }
        
        // CASE: Value is over 65535
        if (value > 65535) {
            return result;
        }

        port = value; // Port value is valid, store its value
    }
 
    // --- Build the final address string and its decimal value ---
    // Build the final address string from the validated octet values
    snprintf(result.address, sizeof(result.address), "%d.%d.%d.%d",
             octetValues[0], octetValues[1], octetValues[2], octetValues[3]);
    
    // Calculate the decimal value by shifting and combining each octet
    unsigned int decimalValue =
        ((unsigned int)octetValues[0] << 24) |
        ((unsigned int)octetValues[1] << 16) |
        ((unsigned int)octetValues[2] << 8)  |
        ((unsigned int)octetValues[3]);
    result.decimalValue = decimalValue;

    result.port = port;
    
    // CASE: Extracted IPv4 address from the input string
    return result;
}

int main(void) {
    char input[1024];

    while (1) {
        printf("Enter a string (or 'END' to quit): ");
        
        // Read input from the user
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Case: EOF or empty input
            break;
        }

        // Strip newline from the input
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        // Case: `END`, stop the program
        if (strcmp(input, "END") == 0) {
            printf("Program terminated\n");
            break;
        }

        IPv4Address result = extractIPv4(input);

        // Invalid ip from user input
        if (strcmp(result.address, "-1") == 0) {
            printf("Invalid input: no valid IPv4 address found.\n");
        } else {
            // Valid ip, decimal value, with/without port value
            printf("Extracted IPv4 address: %s (decimal value: %u, port: %d)\n",
                   result.address, result.decimalValue, result.port);
        }
    }

    return 0;
}