#include <MorseGenerator.h>

char morse_Codes[40][10] =
    {
        ".-",
        "-...",
        "-.-.",
        "-..",
        ".",
        "..-.",
        "--.",
        "....",
        "..",
        ".---",
        ".-.-",
        ".-..",
        "--",
        "-.",
        "---",
        ".--.",
        "--.-",
        ".-.",
        "...",
        "-",
        "..-",
        "...-",
        ".--",
        "-..-",
        "-.--",
        "--..",
        " "};
int buzzerPin = 13;

void PlayMorse(char morse[])
{
    int i = 0;
    while (morse[i] != 0)
    {
        if (morse[i] == '.')
        {
            tone(buzzerPin, MORSE_FRQ, MORSE_DOT_DUR);
            delay(MORSE_DOT_DUR);
            noTone(buzzerPin);
        }
        else if (morse[i] == '-')
        {
            tone(buzzerPin, MORSE_FRQ, MORSE_DASH_DUR);
            delay(MORSE_DASH_DUR);
            noTone(buzzerPin);
        }
        else
        {
            //no tone
            delay(MORSE_PAUSE);
        }
        delay(MORSE_DOT_DUR);

        i++;
    }
    delay(MORSE_DOT_DUR);
}

void PlayMorseString(char morseStr[])
{
    int i = 0;
    while (morseStr[i] != 0)
    {
        PlayMorse(morse_Codes[morseStr[i] - 'A']);
        i++;
    }
}

