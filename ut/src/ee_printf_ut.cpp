#include <stdint.h>
#include <gtest/gtest.h>
#include "ee_printf.h"
#include <iostream>

using std::cout;
using std::endl;

TEST(ee_printf, test_numbers)
{
    char str[100];

    int len = ee_printf(str, "%02X %02X", 0x52, 0x34);
    str[5] = '\0';
    cout << "ee_printf string len=" << len << endl;
    cout << str << endl;

    EXPECT_EQ(memcmp("52 34", str, strlen(str)), 0);
}
