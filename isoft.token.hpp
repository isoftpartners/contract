/*

    isoft token contract. 

*/

#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/singleton.hpp>

using namespace eosio;
using namespace std; 

namespace isoft
{



    CONTRACT isoftToken: public contract {
    public:
        isoftToken( name receiver, name code, datastream<const char*> ds ) : contract(receiver, code, ds) {};

        ACTION create( name issuer, asset maximum_supply );
        ACTION issue ( name to, asset quantity, string memo );
        ACTION retire( asset quantity, string memo );
        ACTION transfer( name from, name to, asset quantity, string memo );
        ACTION open( name owner, const symbol& symbol, name ram_payer );
        ACTION close( name owner, const symbol& symbol );
        ACTION decms( asset quantity, string memo );
        ACTION lockup( name account, asset quantity, time_point expire_time, string memo );

        ACTION planunlock( name account, asset quantity, time_point time, uint32_t span_hour, string memo );
        ACTION claimunlock( name account, const symbol& symbol );

        static asset get_supply( name token_contract_account, symbol_code sym_code )
        {
            stats_t statstable( token_contract_account, sym_code.raw() );
            const auto& st = statstable.get( sym_code.raw() );
            return st.supply;
        }

        static asset get_balance( name token_contract_account, name owner, symbol_code sym_code )
        {
            accounts_t accountstable( token_contract_account, owner.value );
            const auto& ac = accountstable.get( sym_code.raw() );
            return ac.balance;
        }
    private:
        // scope: account
        TABLE account {
            asset       balance;

            uint64_t primary_key() const { return balance.symbol.code().raw(); }
        };
        typedef eosio::multi_index< "accounts"_n, account > accounts_t;


        // scope: code
        TABLE currency_stats {
            asset       supply;
            asset       max_supply;
            name	    issuer;

            uint64_t primary_key() const { return supply.symbol.code().raw(); }
        };
        typedef eosio::multi_index< "stat"_n, currency_stats > stats_t;

        // scope: account
        TABLE lockup_info {
            asset       balance;
            time_point  expire_time;

            uint64_t primary_key() const { return balance.symbol.code().raw(); }
        };
        typedef eosio::multi_index< "lockups"_n, lockup_info > lockups_t;

        // scope: account
        // Lock free plan info
        TABLE plan4unlock_info {
            asset       quantity;
            time_point  time;
            uint32_t    span_hour;

            uint64_t primary_key() const { return quantity.symbol.code().raw(); }
        };
        typedef eosio::multi_index< "plan4unlock"_n, plan4unlock_info > plan4unlock_t;

    private:
        bool check_lockup( name owner, asset value );

        void sub_balance( name owner, asset value );
        void add_balance( name owner, asset value, name ram_payer );
    };
    


} // name space of isoft
