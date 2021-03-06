#pragma once

#if !defined(ARCADIA_BUILD)
#   include "config_core.h"
#endif

#include <Access/LDAPParams.h>
#include <common/types.h>

#if USE_LDAP
#   include <ldap.h>
#   define MAYBE_NORETURN
#else
#   define MAYBE_NORETURN [[noreturn]]
#endif


namespace DB
{

class LDAPClient
{
public:
    explicit LDAPClient(const LDAPServerParams & params_);
    ~LDAPClient();

    LDAPClient(const LDAPClient &) = delete;
    LDAPClient(LDAPClient &&) = delete;
    LDAPClient & operator= (const LDAPClient &) = delete;
    LDAPClient & operator= (LDAPClient &&) = delete;

protected:
    MAYBE_NORETURN void diag(const int rc, String text = "");
    MAYBE_NORETURN void openConnection();
    void closeConnection() noexcept;
    LDAPSearchResults search(const LDAPSearchParams & search_params);

protected:
    const LDAPServerParams params;
#if USE_LDAP
    LDAP * handle = nullptr;
#endif
};

class LDAPSimpleAuthClient
    : private LDAPClient
{
public:
    using LDAPClient::LDAPClient;
    bool authenticate(const LDAPSearchParamsList * search_params, LDAPSearchResultsList * search_results);
};

}

#undef MAYBE_NORETURN
