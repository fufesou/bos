/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include <appbase/channel.hpp>
#include <appbase/method.hpp>

#include <eosio/chain/block.hpp>
#include <eosio/chain/block_state.hpp>
#include <eosio/chain/transaction_metadata.hpp>
#include <eosio/chain/trace.hpp>
#include <eosio/chain/pbft_database.hpp>

namespace eosio { namespace chain { namespace plugin_interface {
   using namespace eosio::chain;
   using namespace appbase;

   template<typename T>
   using next_function = std::function<void(const fc::static_variant<fc::exception_ptr, T>&)>;

   struct chain_plugin_interface;

   namespace channels {
      using pre_accepted_block     = channel_decl<struct pre_accepted_block_tag,    signed_block_ptr>;
      using rejected_block         = channel_decl<struct rejected_block_tag,        signed_block_ptr>;
      using accepted_block_header  = channel_decl<struct accepted_block_header_tag, block_state_ptr>;
      using accepted_block         = channel_decl<struct accepted_block_tag,        block_state_ptr>;
      using irreversible_block     = channel_decl<struct irreversible_block_tag,    block_state_ptr>;
      using accepted_transaction   = channel_decl<struct accepted_transaction_tag,  transaction_metadata_ptr>;
      using applied_transaction    = channel_decl<struct applied_transaction_tag,   transaction_trace_ptr>;
      using accepted_confirmation  = channel_decl<struct accepted_confirmation_tag, header_confirmation>;

   }

   namespace methods {
      using get_block_by_number    = method_decl<chain_plugin_interface, signed_block_ptr(uint32_t block_num)>;
      using get_block_by_id        = method_decl<chain_plugin_interface, signed_block_ptr(const block_id_type& block_id)>;
      using get_head_block_id      = method_decl<chain_plugin_interface, block_id_type ()>;
      using get_lib_block_id       = method_decl<chain_plugin_interface, block_id_type ()>;

      using get_last_irreversible_block_number = method_decl<chain_plugin_interface, uint32_t ()>;
   }

   namespace incoming {
      namespace channels {
         using block                 = channel_decl<struct block_tag, signed_block_ptr>;
         using transaction           = channel_decl<struct transaction_tag, transaction_metadata_ptr>;
      }

      namespace methods {
         // synchronously push a block/trx to a single provider
         using block_sync            = method_decl<chain_plugin_interface, void(const signed_block_ptr&), first_provider_policy>;
         using transaction_async     = method_decl<chain_plugin_interface, void(const transaction_metadata_ptr&, bool, next_function<transaction_trace_ptr>), first_provider_policy>;
      }
   }

   namespace compat {
      namespace channels {
         using transaction_ack       = channel_decl<struct accepted_transaction_tag, std::pair<fc::exception_ptr, transaction_metadata_ptr>>;
      }
   }

   namespace pbft {
       namespace incoming {
           using prepare_channel = channel_decl<struct pbft_prepare_tag, pbft_metadata_ptr<pbft_prepare>>;
           using commit_channel = channel_decl<struct pbft_commit_tag, pbft_metadata_ptr<pbft_commit>>;
           using view_change_channel = channel_decl<struct pbft_view_change_tag, pbft_metadata_ptr<pbft_view_change>>;
           using new_view_channel = channel_decl<struct pbft_new_view_tag, pbft_metadata_ptr<pbft_new_view>>;
           using checkpoint_channel = channel_decl<struct pbft_checkpoint_tag, pbft_metadata_ptr<pbft_checkpoint>>;

       }

       namespace outgoing {
           using prepare_channel = channel_decl<struct pbft_prepare_tag, pbft_prepare_ptr>;
           using commit_channel = channel_decl<struct pbft_commit_tag, pbft_commit_ptr>;
           using view_change_channel = channel_decl<struct pbft_view_change_tag, pbft_view_change_ptr>;
           using new_view_channel = channel_decl<struct pbft_new_view_tag, pbft_new_view_ptr>;
           using checkpoint_channel = channel_decl<struct pbft_checkpoint_tag, pbft_checkpoint_ptr>;

       }

       using committed_transition_channel = channel_decl<struct pbft_comitted_tag, bool>;
       using prepared_transition_channel = channel_decl<struct pbft_prepared_tag, bool>;
   }

} } }
