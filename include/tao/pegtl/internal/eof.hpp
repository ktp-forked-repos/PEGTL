// Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_EOF_HPP
#define TAO_PEGTL_INTERNAL_EOF_HPP

#include "../config.hpp"

#include "skip_control.hpp"

#include "../analysis/generic.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         struct eof
         {
            using analyze_t = analysis::generic< analysis::rule_type::opt >;

            template< typename Input >
            [[nodiscard]] static bool match( Input& in ) noexcept( noexcept( in.empty() ) )
            {
               return in.empty();
            }
         };

         template<>
         inline constexpr bool skip_control< eof > = true;

      }  // namespace internal

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
