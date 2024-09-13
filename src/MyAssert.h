#pragma once

#ifndef NDEBUG
    #define MyAssert(Expr, ...) \
        if (!(Expr))            \
            MyAssert_(#Expr, Expr, __FILE__, __LINE__, ##__VA_ARGS__);
#else
    #define MyAssert(Expr, ...) \
        if (!(Expr))            \
            abort();
#endif

void MyAssert_(const char* expr_str, bool expr, const char* file, int line, const char* msg) __attribute__((noreturn));
void MyAssert_(const char* expr_str, bool expr, const char* file, int line) __attribute__((noreturn));
