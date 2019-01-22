// Copyright (c) 2017-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <type_traits>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

using namespace tao::TAO_PEGTL_NAMESPACE;  // NOLINT

template< typename Rule >
struct selector : std::true_type
{};

struct user_state
{};

template< typename Rule >
struct work : nothing< Rule >
{};

template<>
struct work< success >
{
   template< typename Input, typename... States >
   static void apply( const Input& /*unused*/, user_state& /*unused*/, States&&... /*unused*/ )
   {}
};

int main()
{
   memory_input input( "", "dummy" );

   user_state state;
   auto root = parse_tree::parse< success, selector, work >( input, state );

   return 0;
}
