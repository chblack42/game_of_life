#pragma once
#include <string>
#include <vector>
std::string test1 = 
R"((8, 0)
(8, 1)
(8, 2)
(8, 3)
(8, 4)
(8, 5)
(8, 6)
(8, 7))";

std::string test1_result =
R"(#Life 1.06
8 0
8 7
7 8
8 8
9 8
7 -1
8 -1
9 -1
)";

std::string test2 =
R"((0, -1)
(-1, 0)
(0, 0))";

std::string test2_result = 
R"(#Life 1.06
0 -1
-1 0
0 0
-1 -1
)";

std::string test3 =
R"((-3, -7)
(-2, -7)
(-1, -7)
(0, -7)
(-3, -6)
(1, -6)
(-3, -5)
(-12, -4)
(-11, -4)
(-2, -4)
(1, -4)
(-13, -3)
(-12, -3)
(-10, -3)
(-9, -3)
(-12, -2)
(-11, -2)
(-10, -2)
(-9, -2)
(-5, -2)
(-11, -1)
(-10, -1)
(-6, -1)
(-4, -1)
(-3, -1)
(6, -1)
(11, -1)
(12, -1)
(13, -1)
(-7, 0)
(-3, 0)
(6, 0)
(11, 0)
(13, 0)
(-11, 1)
(-10, 1)
(-6, 1)
(-4, 1)
(-3, 1)
(6, 1)
(11, 1)
(12, 1)
(13, 1)
(-12, 2)
(-11, 2)
(-10, 2)
(-9, 2)
(-5, 2)
(-13, 3)
(-12, 3)
(-10, 3)
(-9, 3)
(-12, 4)
(-11, 4)
(-2, 4)
(1, 4)
(-3, 5)
(-3, 6)
(1, 6)
(-3, 7)
(-2, 7)
(-1, 7)
(0, 7))";

std::string test3_result = R"(#Life 1.06
-16 -5
-15 -5
-16 -4
-15 -4
-14 -4
-15 -3
-14 -3
-16 -2
-11 -1
-9 -1
-7 -8
-4 -8
-8 -7
-8 -6
-4 -6
-8 -5
-7 -5
-6 -5
-5 -5
-3 -2
-3 -1
12 -4
12 -3
8 -2
9 -2
10 -2
11 -2
12 -2
8 -1
10 -1
-12 0
-11 0
-11 1
-9 1
-16 2
-15 3
-14 3
-16 4
-15 4
-14 4
-16 5
-15 5
-8 0
-3 1
-3 2
-8 5
-7 5
-6 5
-5 5
-8 6
-4 6
-8 7
11 0
14 0
15 0
8 1
10 1
8 2
9 2
10 2
11 2
12 2
12 3
12 4
0 0
5 0
0 1
6 1
7 1
7 2
7 -2
0 -1
6 -1
7 -1
-17 -4
-18 -3
-17 -3
-17 -2
-17 2
-18 3
-17 3
-17 4
-7 8
-4 8
16 0
)";


std::string test4 = 
R"((-9223372036854775808, 9223372036854775807)
(9223372036854775807, 9223372036854775807)
(-9223372036854775808, -9223372036854775808)
(9223372036854775807, -9223372036854775808))";

std::string test4_result = R"(#Life 1.06
)";

std::string test5 =
R"((-9223372036854775808, 9223372036854775807)
(-9223372036854775807, 9223372036854775807)
(-9223372036854775806, 9223372036854775807))";

std::string test5_result = R"(#Life 1.06
)";

std::string test6 =
R"((-7, -4)
(6, -4)
(-7, -3)
(-6, -3)
(-5, -3)
(0, -3)
(4, -3)
(5, -3)
(6, -3)
(-4, -2)
(-2, -2)
(0, -2)
(3, -2)
(-5, -1)
(2, -1)
(4, -1)
(-5, 0)
(-3, 0)
(4, 0)
(-4, 1)
(-1, 1)
(1, 1)
(3, 1)
(-7, 2)
(-6, 2)
(-5, 2)
(-1, 2)
(4, 2)
(5, 2)
(6, 2)
(-7, 3)
(6, 3))";

std::string test6_result = R"(#Life 1.06
-7 -4
-7 -3
-6 -3
-5 -3
-4 -2
-2 -2
-5 -1
6 -4
0 -3
4 -3
5 -3
6 -3
0 -2
3 -2
2 -1
4 -1
-5 0
-3 0
-4 1
-1 1
-7 2
-6 2
-5 2
-1 2
-7 3
4 0
1 1
3 1
4 2
5 2
6 2
6 3
)";

std::string test7 =
R"((9223372036854775806, 9223372036854775806)
(9223372036854775805, 9223372036854775806)
(9223372036854775804, 9223372036854775806)
(9223372036854775807, -9223372036854775808))";

std::string test7_result =
R"(#Life 1.06
9223372036854775804 9223372036854775806
9223372036854775806 9223372036854775806
9223372036854775805 9223372036854775806
)";



std::vector<std::string> test_vector = { test1,test2,test3,test4,test5,test6,test7 };
std::vector<std::string> test_result_vector = { test1_result,test2_result, test3_result,test4_result,test5_result,test6_result, test7_result };