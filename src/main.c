#include <stdio.h>
#include <string.h>

// Bundles the three values extractIPv4 needs to hand back to main.
typedef struct {
    char address[16];   // "255.255.255.255" + null, or "-1" if invalid address
    int decimalValue;   // 32-bit decimal value of the address
    int port;   // port number, or -1 if port was not given
} IPv4Address;

// TEMP FUNCTION
IPv4Address extractIPv4(const char *input) {
    return;
}

int main(void) {
    char input[1024];

    while (1) {
        printf("Enter a string (or 'END' to quit): ");

        // Strip newline
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        // `END` case
        if (strcmp(input, "END") == 0) {
            printf("Program terminated\n");
            break;
        }

        IPv4Address result;

        // TEMP VALUES
        strcpy(result.address, "-1");
        result.decimalValue = 0;
        result.port = -1;

        // Invalid ip from user input
        if (strcmp(result.address, "-1") == 0) {
            printf("Invalid input: no valid IPv4 address found.\n");
        } else {
            // Valid ip, decimal value, with/without port value
            printf("Extracted IPv4 address: %s (decimal value: %d, port: %d)\n",
                   result.address, result.decimalValue, result.port);
        }
    }

    return 0;
}