// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

// This is a small experiment with a grammar that can recover from errors.
//
// Triggered by https://github.com/taocpp/PEGTL/issues/55
//
// The grammar will recognise simple expressions terminated by semicolons.
// When an expression fails to parse, it skips to the next expression
// by looking for the terminator.
//
// Try: build/src/example/pegtl/recover '1+2*3;1+2*(3-)-4;-5;6/;7*(8+9)'

#include <iostream>
#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/argv_input.hpp>

using namespace tao::TAOCPP_PEGTL_NAMESPACE;

// clang-format off

template< typename T >
struct skipping : until< T > {};

template< typename R, typename T >
struct recoverable : sor< try_catch< must< R, T > >, skipping< T > > {};

struct sum;
struct value : sor< plus< digit >, if_must< one< '(' >, sum, one< ')' > > > {};
struct prod : list< must< value >, one< '*', '/' > > {};
struct sum : list_must< prod, one< '+', '-' > > {};

struct term : sor< one< ';' >, eof > {};
struct expr : recoverable< sum, term > {};
struct my_grammar : star< expr > {};

// clang-format on

template< typename Rule >
struct my_action
   : nothing< Rule >
{
};

template< typename T >
struct my_action< skipping< T > >
{
   template< typename Input >
   static void apply( const Input& in, bool& error )
   {
      if( !error ) {
         std::cout << in.position() << ": Invalid expression \"" << in.string() << "\"" << std::endl;
      }
      error = true;
   }
};

template< typename R >
struct found
{
   template< typename Input >
   static void apply( const Input& in, bool& error )
   {
      if( !error ) {
         std::cout << in.position() << ": Found " << internal::demangle< R >() << ": \"" << in.string() << "\"" << std::endl;
      }
   }
};

// clang-format off
template<> struct my_action< value > : found< value > {};
template<> struct my_action< prod > : found< prod > {};
template<> struct my_action< sum > : found< sum > {};
// clang-format on

template<>
struct my_action< expr >
{
   template< typename Input >
   static void apply( const Input& in, bool& error )
   {
      found< expr >::apply( in, error );
      error = false;
      std::cout << std::string( 79, '-' ) << std::endl;
   }
};

template< typename Rule >
struct my_control
   : normal< Rule >
{
   template< typename Input, typename... States >
   static void raise( const Input& in, States&&... )
   {
      std::cout << in.position() << ": Parse error matching " << internal::demangle< Rule >() << std::endl;
      throw parse_error( "parse error matching " + internal::demangle< Rule >(), in );
   }
};

int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      argv_input<> in( argv, i );
      bool error = false;
      parse< my_grammar, my_action, my_control >( in, error );
   }
   return 0;
}