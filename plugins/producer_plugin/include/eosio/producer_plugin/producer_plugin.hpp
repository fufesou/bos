/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */

#pragma once

#include <eosio/chain_plugin/chain_plugin.hpp>
#include <eosio/http_client_plugin/http_client_plugin.hpp>

#include <appbase/application.hpp>

namespace eosio {

using boost::signals2::signal;

class producer_plugin : public appbase::plugin<producer_plugin> {
public:
   APPBASE_PLUGIN_REQUIRES((chain_plugin)(http_client_plugin))

   struct runtime_options {
      fc::optional<int32_t> max_transaction_time;
      fc::optional<int32_t> max_irreversible_block_age;
      fc::optional<int32_t> produce_time_offset_us;
      fc::optional<int32_t> last_block_time_offset_us;
      fc::optional<int32_t> max_scheduled_transaction_time_per_block_ms;
      fc::optional<int32_t> subjective_cpu_leeway_us;
      fc::optional<double>  incoming_defer_ratio;
   };

   struct whitelist_blacklist {
      fc::optional< flat_set<account_name> > actor_whitelist;
      fc::optional< flat_set<account_name> > actor_blacklist;
      fc::optional< flat_set<account_name> > contract_whitelist;
      fc::optional< flat_set<account_name> > contract_blacklist;
      fc::optional< flat_set< std::pair<account_name, action_name> > > action_blacklist;
      fc::optional< flat_set<public_key_type> > key_blacklist;
   };

   struct greylist_params {
      std::vector<account_name> accounts;
   };

   struct integrity_hash_information {
      chain::block_id_type head_block_id;
      chain::digest_type   integrity_hash;
   };
////bos feature export table snapshot begin
   struct table_snapshot_params{
        std::string          table_name;
   };

   struct table_snapshot_information {
        std::string          table_snapshot_name;
   };
////bos feature export table snapshot end
   struct snapshot_information {
      chain::block_id_type head_block_id;
      std::string          snapshot_name;
   };

   producer_plugin();
   virtual ~producer_plugin();

   virtual void set_program_options(
      boost::program_options::options_description &command_line_options,
      boost::program_options::options_description &config_file_options
      ) override;

   bool                   is_producer_key(const chain::public_key_type& key) const;
   chain::signature_type  sign_compact(const chain::public_key_type& key, const fc::sha256& digest) const;

   virtual void plugin_initialize(const boost::program_options::variables_map& options);
   virtual void plugin_startup();
   virtual void plugin_shutdown();

   void pause();
   void resume();
   bool paused() const;
   void update_runtime_options(const runtime_options& options);
   runtime_options get_runtime_options() const;

   void add_greylist_accounts(const greylist_params& params);
   void remove_greylist_accounts(const greylist_params& params);
   greylist_params get_greylist() const;

   whitelist_blacklist get_whitelist_blacklist() const;
   void set_whitelist_blacklist(const whitelist_blacklist& params);

   integrity_hash_information get_integrity_hash() const;
   snapshot_information create_snapshot() const;
   table_snapshot_information export_table_snapshot(const table_snapshot_params& params) const;////bos feature export table snapshot 

   signal<void(const chain::producer_confirmation&)> confirmed_block;
private:
   std::shared_ptr<class producer_plugin_impl> my;
};

} //eosio

FC_REFLECT(eosio::producer_plugin::runtime_options, (max_transaction_time)(max_irreversible_block_age)(produce_time_offset_us)(last_block_time_offset_us)(subjective_cpu_leeway_us)(incoming_defer_ratio));
FC_REFLECT(eosio::producer_plugin::greylist_params, (accounts));
FC_REFLECT(eosio::producer_plugin::whitelist_blacklist, (actor_whitelist)(actor_blacklist)(contract_whitelist)(contract_blacklist)(action_blacklist)(key_blacklist) )
FC_REFLECT(eosio::producer_plugin::integrity_hash_information, (head_block_id)(integrity_hash))
FC_REFLECT(eosio::producer_plugin::snapshot_information, (head_block_id)(snapshot_name))
////bos feature export table snapshot begin
FC_REFLECT(eosio::producer_plugin::table_snapshot_information, (table_snapshot_name))
FC_REFLECT(eosio::producer_plugin::table_snapshot_params, (table_name))
////bos feature export table snapshot end