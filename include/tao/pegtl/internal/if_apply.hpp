// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_IF_APPLY_HPP
#define TAO_PEGTL_INTERNAL_IF_APPLY_HPP

#include "../config.hpp"

#include "apply_single.hpp"
#include "skip_control.hpp"

#include "../analysis/counted.hpp"
#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Rule, typename... Actions >
         struct if_apply
         {
            using analyze_t = typename Rule::analyze_t;

            template< apply_mode A,
                      rewind_mode M,
                      template< typename... > class Action,
                      template< typename... > class Control,
                      typename Input,
                      typename... States >
            static bool match( Input& in, States&&... st )
            {
               if constexpr( ( A == apply_mode::ACTION ) && ( sizeof...( Actions ) != 0 ) ) {
                  using action_t = typename Input::action_t;
                  auto m = in.template mark< rewind_mode::REQUIRED >();
                  if( Control< Rule >::template match< apply_mode::ACTION, rewind_mode::ACTIVE, Action, Control >( in, st... ) ) {
                     const action_t i2( m.iterator(), in );
                     return m( ( apply_single< Actions >::match( i2, st... ) && ... ) );
                  }
                  return false;
               }
               else {
                  return Control< Rule >::template match< A, M, Action, Control >( in, st... );
               }
            }
         };

         template< typename Rule, typename... Actions >
         inline constexpr bool skip_control< if_apply< Rule, Actions... > > = true;

      }  // namespace internal

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
