/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Counter-Strike:Source Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$

*/

#include "extension.h"

Curl_Ext g_extension;  /**< Global singleton for extension's main interface */
SMEXT_LINK(&g_extension);

const size_t kThreadNumber = 2;

ThreadPool* g_thread_pool = NULL;
HandleType_t g_curl_type = 0;
HandleType_t g_file_type = 0;
CurlTypeHandler g_curl_type_handler;


bool Curl_Ext::SDK_OnLoad(char* error, size_t err_max, bool late) {  
#if defined PLATFORM_WINDOWS
    long flags = CURL_GLOBAL_WIN32;
#else
    long flags = CURL_GLOBAL_NOTHING;
#endif

    CURLcode code = curl_global_init(flags);

    if (code != CURLE_OK) {
        smutils->Format(error, err_max, "%s", curl_easy_strerror(code));
        return false;
    }

    HandleAccess rules;
    handlesys->InitAccessDefaults(NULL, &rules);

    rules.access[HandleAccess_Delete] = HANDLE_RESTRICT_IDENTITY;
    rules.access[HandleAccess_Read] = 0;

    g_curl_type = handlesys->CreateType(
            "Curl", 
            &g_curl_type_handler, 
            0, 
            NULL, 
            &rules, 
            myself->GetIdentity(), 
            NULL);

    if (!handlesys->FindHandleType("File", &g_file_type)) {
        smutils->Format(error, err_max, "%s", "Cannot find HandleType_t File");
        return false;
    }

    sharesys->AddNatives(myself, kExtensionNatives);

    g_thread_pool = new ThreadPool(kThreadNumber);

    return true; 
}

void Curl_Ext::SDK_OnUnload() {  
    if (g_thread_pool) {  
        delete g_thread_pool;
    }

    curl_global_cleanup();

    handlesys->RemoveType(g_curl_type, NULL);
}
