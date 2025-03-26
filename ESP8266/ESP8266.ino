#include <TM1637Display.h>

// Pins for TM1637 display
#define CLK D2
#define DIO D3

TM1637Display display(CLK, DIO);

// Function to convert characters to their corresponding patterns
uint8_t charToSegment(char character) {
    // Define patterns for each character
    switch (character) {
        case '0': return 0b00111111;
        case '1': return 0b00000110;
        case '2': return 0b01011011;
        case '3': return 0b01001111;
        case '4': return 0b01100110;
        case '5': return 0b01101101;
        case '6': return 0b01111101;
        case '7': return 0b00000111;
        case '8': return 0b01111111;
        case '9': return 0b01101111;
        case 'A': return 0b01110111;
        case 'B': return 0b01111100;
        case 'C': return 0b00111001;
        case 'D': return 0b01011110;
        case 'E': return 0b01111001;
        case 'F': return 0b01110001;
        case 'G': return 0b00111101;
        case 'H': return 0b01110110;
        case 'I': return 0b00000110;
        case 'J': return 0b00011110;
        case 'K': return 0b01110101;
        case 'L': return 0b00111000;
        case 'M': return 0b01010101;
        case 'N': return 0b01010100;
        case 'O': return 0b00111111;
        case 'P': return 0b01110011;
        case 'Q': return 0b01100111;
        case 'R': return 0b01010000;
        case 'S': return 0b01101101;
        case 'T': return 0b01111000;
        case 'U': return 0b00111110;
        case 'V': return 0b00111110; // Same as U
        case 'W': return 0b00101010;
        case 'X': return 0b01110110; // Same as H
        case 'Y': return 0b01101110;
        case 'Z': return 0b01011011;
        default: return 0; // For unsupported characters
    }
}

// Function to display text on the TM1637 display
void displayText(const char* text) {
    uint8_t segments[4] = {0}; // Array to hold segment patterns
    
    for (int i = 0; i < 4; ++i) {
        if (text[i] != '\0') {
            segments[i] = charToSegment(toupper(text[i]));
        }
    }
    display.setSegments(segments);
}

void setup() {
    // Initialize TM1637 display
    display.setBrightness(0x0a); // Set brightness level (0x00 to 0x0f)

    // Example text to display
    const char* text = "";

    // Display the text
    displayText(text);
}

void loop(){
  
}
