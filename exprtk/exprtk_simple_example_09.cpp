/*
 **************************************************************
 *         C++ Mathematical Expression Toolkit Library        *
 *                                                            *
 * Simple Example 09                                          *
 * Author: Arash Partow (1999-2024)                           *
 * URL: https://www.partow.net/programming/exprtk/index.html  *
 *                                                            *
 * Copyright notice:                                          *
 * Free use of the Mathematical Expression Toolkit Library is *
 * permitted under the guidelines and in accordance with the  *
 * most current version of the MIT License.                   *
 * https://www.opensource.org/licenses/MIT                    *
 * SPDX-License-Identifier: MIT                               *
 *                                                            *
 **************************************************************
*/


#include <cstdio>
#include <string>

#include "exprtk.hpp"


template <typename T>
void primes()
{
   typedef exprtk::symbol_table<T>         symbol_table_t;
   typedef exprtk::expression<T>           expression_t;
   typedef exprtk::parser<T>               parser_t;
   typedef exprtk::function_compositor<T>  compositor_t;
   typedef typename compositor_t::function function_t;

   T x = T(0);

   symbol_table_t symbol_table;

   symbol_table.add_constants();
   symbol_table.add_variable("x",x);

   compositor_t compositor(symbol_table);

   //Mode 1 - if statement based
   compositor.add(
      function_t("is_prime_impl1")
      .vars("x", "y")
      .expression
      (
         " if (y == 1,true,                "
         "    if (0 == (x % y),false,      "
         "       is_prime_impl1(x,y - 1))) "
      ));

   compositor.add(
      function_t("is_prime1")
      .var("x")
      .expression
      (
         " if (frac(x) != 0, false,                                "
         "    if (x <= 0, false,                                   "
         "       is_prime_impl1(x,min(x - 1,trunc(sqrt(x)) + 1)))) "
      ));

   //Mode 2 - switch statement based
   compositor.add(
      function_t("is_prime_impl2")
      .vars("x", "y")
      .expression
      (
         " switch                                          "
         " {                                               "
         "    case y == 1       : true;                    "
         "    case (x % y) == 0 : false;                   "
         "    default           : is_prime_impl2(x,y - 1); "
         " }                                               "
      ));

   compositor.add(
      function_t("is_prime2")
      .var("x")
      .expression
      (
         " switch                                                                  "
         " {                                                                       "
         "    case x <= 0       : false;                                           "
         "    case frac(x) != 0 : false;                                           "
         "    default           : is_prime_impl2(x,min(x - 1,trunc(sqrt(x)) + 1)); "
         " }                                                                       "
      ));

   //Mode 3 - switch statement and while-loop based
   compositor.add(
      function_t("is_prime_impl3")
      .vars("x", "y")
      .expression
      (
         " while (y > 0)                              "
         " {                                          "
         "    switch                                  "
         "    {                                       "
         "       case y == 1       : ~(y := 0,true);  "
         "       case (x % y) == 0 : ~(y := 0,false); "
         "       default           : y := y - 1;      "
         "    }                                       "
         " }                                          "
      ));

   compositor.add(
      function_t("is_prime3")
      .var("x")
      .expression
      (
         " switch                                                                  "
         " {                                                                       "
         "    case x <= 0       : false;                                           "
         "    case frac(x) != 0 : false;                                           "
         "    default           : is_prime_impl3(x,min(x - 1,trunc(sqrt(x)) + 1)); "
         " }                                                                       "
      ));

   std::string expression_str1 = "is_prime1(x)";
   std::string expression_str2 = "is_prime2(x)";
   std::string expression_str3 = "is_prime3(x)";

   expression_t expression1;
   expression_t expression2;
   expression_t expression3;
   expression1.register_symbol_table(symbol_table);
   expression2.register_symbol_table(symbol_table);
   expression3.register_symbol_table(symbol_table);

   parser_t parser;

   parser.compile(expression_str1, expression1);
   parser.compile(expression_str2, expression2);
   parser.compile(expression_str3, expression3);

   for (std::size_t i = 0; i < 100; ++i)
   {
      x = static_cast<T>(i);

      const T result1 = expression1.value();
      const T result2 = expression2.value();
      const T result3 = expression3.value();

      printf("%03d  Result1: %c  Result2: %c  Result3: %c\n",
             static_cast<unsigned int>(i),
             (result1 == T(1)) ? 'T' : 'F',
             (result2 == T(1)) ? 'T' : 'F',
             (result3 == T(1)) ? 'T' : 'F');
   }
}

int main()
{
   primes<double>();
   return 0;
}
