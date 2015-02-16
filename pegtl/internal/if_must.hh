// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_IF_MUST_HH
#define PEGTL_INTERNAL_IF_MUST_HH

#include "seq.hh"
#include "trivial.hh"
#include "utility.hh"
#include "rule_conjunction.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Cond, typename ... Thens >
      struct if_must
      {
         using analyze_t = analysis::generic< analysis::rule_type::IF, Cond, seq< Thens ... >, trivial< false > >;

         template< apply_mode A, error_mode Mode, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< error_mode::THROW >();

            if ( Control< Cond >::template match< A, Mode, Action, Control >( in, st ... ) ) {
               return m( rule_conjunction< Thens ... >::template match< A, error_mode::THROW, Action, Control >( in, st ... ) );
            }
            return false;
         }
      };

   } // internal

} // pegtl

#endif
