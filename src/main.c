#include <stdio.h>

s21_ZOOM_LEVEL_MASK = 0xff0000; // 16711680 - десятичное / шеснадцатиричное - 0xff0000 / восьмиричное - 077600000 / двоичное - 0b111111110000000000000000
s21_NEGATE_MASK = 0x80000000; // 2147483648 - десятичное / шеснадцатиричное - 0x80000000 / восьмиричное - 020000000000 / двоичное - 0b10000000000000000000000000000000
s21_MANTISLENGTH = 31; // т.к. мантиса состоит из 3 unsigned по 10 символов и конца строки /0

typedef struct {
    unsigned bits[4];
} s21_decimal;

short s21_GetSignDecimal(s21_decimal decimal) {
    return decimal.bits[3] >> 31;
}

short s21_GetZoomLevelDecimal(s21_decimal decimal) {
    return (decimal.bits[3] & s21_ZOOM_LEVEL_MASK) >> 16;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
    result->bits[0] = value.bits[0];
    result->bits[1] = value.bits[1];
    result->bits[2] = value.bits[2];
    result->bits[3] = value.bits[3] ^ s21_NEGATE_MASK;
    return 0;
}

short s21_getMantisDecimal(s21_decimal decimal, char *mantis) {
    sprintf(mantis, "%010u%010u%010u", decimal.bits[2], decimal.bits[1], decimal.bits[0]);
    return 0;
}

short s21_getInsignificantZerosDecimal(s21_decimal decimal) {
    char mantis[s21_MANTISLENGTH];
    s21_getMantisDecimal(decimal, &mantis);
    short count = 0;
    for (short i = 0; i < 31; i++)
        if (mantis[i] == '0')
            count++;
        else
            break;
    return count;
}

short s21_secondNormalizationDecimal(s21_decimal *a, s21_decimal *b, short A, short B) {
    short sub = A - B;
    short zeroB = s21_getInsignificantZerosDecimal(*b);
    if (zeroB >= sub) {
        char mantis[s21_MANTISLENGTH];
        s21_getMantisDecimal(*b, &mantis);

    }
}

short s21_firstNormalizationDecimal(s21_decimal *a, s21_decimal *b) {
    short zoomLevelA = s21_GetZoomLevelDecimal(*a);
    short zoomLevelB = s21_GetZoomLevelDecimal(*b);
    if (zoomLevelA == zoomLevelB)
        return 0;
    else if (zoomLevelA > zoomLevelB)
        return s21_secondNormalizationDecimal(&a, &b, zoomLevelA, zoomLevelB);
    else
        return s21_secondNormalizationDecimal(&b, &a, zoomLevelB, zoomLevelA);
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    if (s21_GetSignDecimal(value_1) == s21_GetSignDecimal(value_2)) {
        result->bits[3] = value_1.bits[3];
        short overflow = 0;
        for (short i = 0; i < 3; i++) {
            result->bits[i] = value_1.bits[i] + value_2.bits[i] + overflow;
            if (result->bits[i] <= value_1.bits[i]) {
                overflow = 1;
                if (i == 2)
                    return 1;
            }
            else
                overflow = 0;
        }
    }
    return 0;
}

int main() {
	// your code goes here
	s21_decimal a = {{4294967295, 4294967295, 4294967295, 4294967295}};
	s21_decimal b = {{4294967295, 4294967295, 4294967295, 4294967295}};
	s21_decimal c = {{1, 1, 1, 1}};
	printf("%i\n", s21_getInsignificantZerosDecimal(c));
	printf("%u\n", s21_add(a, b, &c));
	char buffer[100];
	snprintf(buffer, sizeof(buffer), "%u %u %u %u", c.bits[0], c.bits[1], c.bits[2], c.bits[3]);
	printf("%s\n", buffer);
	s21_getInsignificantZerosDecimal(a);
	s21_getInsignificantZerosDecimal(c);
}

