// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

// Check for __VA_OPT__ support
// Apdated from cpplearner's StackOverflow answer: https://stackoverflow.com/a/48045656
#define PP_THIRD_ARG(a,b,c,...) c
#define VA_OPT_SUPPORTED_I(...) PP_THIRD_ARG(__VA_OPT__(,),true,false,)
#define VA_OPT_SUPPORTED VA_OPT_SUPPORTED_I(?)

// VA_ARGS_COMMAPREFIX(): VA_ARGS_COMMAPREFIX(__VA_ARGS__) expands to __VA_ARGS__ with a comma in
// front if more than one argument, else nothing.
// If __VA_OPT__ supported, use that. Else, use GCC's ,## hack
#if VA_OPT_SUPPORTED
#    define VA_ARGS_COMMAPREFIX(...) __VA_OPT__(,) __VA_ARGS__
#else
#    define VA_ARGS_COMMAPREFIX(...) , ## __VA_ARGS__
#endif

