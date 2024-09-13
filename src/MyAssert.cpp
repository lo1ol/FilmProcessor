#include "MyAssert.h"

#include <Arduino.h>

void MyAssert_(const char* expr_str, bool expr, const char* file, int line, const char* msg) {
    Serial.print("Assert failed:\t");
    Serial.println(msg);
    Serial.print("Expected:\t");
    Serial.println(expr_str);
    Serial.print("Source:\t");
    Serial.print(file);
    Serial.print(":");
    Serial.println(line);
    Serial.flush();
    abort();
}

void MyAssert_(const char* expr_str, bool expr, const char* file, int line) {
    Serial.println("Assert failed");
    Serial.print("Expected:\t");
    Serial.println(expr_str);
    Serial.print("Source:\t");
    Serial.print(file);
    Serial.print(":");
    Serial.println(line);
    Serial.flush();
    abort();
}
