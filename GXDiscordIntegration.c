#ifdef BUILD_G10_WITH_DISCORD
#include <G10/GXDiscordIntegration.h>

#include <discord_game_sdk.h>
#include <time.h>

void DISCORD_CALLBACK OnUserUpdated(void* data)
{   
    GXInstance_t* p_instance = g_get_active_instance();

    struct DiscordUser user;
    p_instance->discord_integration.users->get_current_user(p_instance->discord_integration.users, &user);

    p_instance->discord_integration.user_id = user.id;

    printf("DISCORD USER ID: \"%s\"\n", user.username);
}


void init_discord_integration(void)
{
    struct IDiscordUserEvents  users_events = { 0 };
    struct DiscordCreateParams params       = { 0 };
    GXInstance_t* p_instance = g_get_active_instance();
    
    users_events.on_current_user_update = OnUserUpdated;

    DiscordCreateParamsSetDefault(&params);
    
    params.client_id = 1059248544785117244;
    params.flags = DiscordCreateFlags_Default;
    params.event_data = &p_instance->discord_integration;

    if ( DiscordCreate(DISCORD_VERSION, &params, &p_instance->discord_integration.core) != DiscordResult_Ok )
        printf("[G10] [Discord] Failed to initialize Discord API\n");

    p_instance->discord_integration.users = p_instance->discord_integration.core->get_user_manager(p_instance->discord_integration.core);
    p_instance->discord_integration.achievements = p_instance->discord_integration.core->get_achievement_manager(p_instance->discord_integration.core);
    p_instance->discord_integration.activities = p_instance->discord_integration.core->get_activity_manager(p_instance->discord_integration.core);
    p_instance->discord_integration.application = p_instance->discord_integration.core->get_application_manager(p_instance->discord_integration.core);
    p_instance->discord_integration.lobbies = p_instance->discord_integration.core->get_lobby_manager(p_instance->discord_integration.core);

    struct IDiscordUserManager* user_manager = p_instance->discord_integration.users;
    struct IDiscordActivityManager* activity_manager = p_instance->discord_integration.activities;
    struct DiscordActivity          activity = { 0 };

    activity.application_id = 1059248544785117244;
    snprintf(activity.details, 34, "Scene: %s", p_instance->context.scene->name);
    snprintf(activity.state, 10, "Debugging");
    activity.party.size.current_size = 0;
    activity.party.size.max_size = 0;
    activity.assets.large_image[0] = 'g';
    activity.assets.large_image[1] = '1';
    activity.assets.large_image[2] = '0';
    activity.assets.large_image[3] = '\0';
//    activity.type = DiscordActivityType_Playing;
    activity.timestamps.start = time(NULL);
    activity.timestamps.end = 0;
    user_manager->get_current_user(user_manager, &p_instance->discord_integration.user);
    activity_manager->update_activity(activity_manager, &activity, 0, 0);


    return 1;
}

int discord_callbacks ( GXInstance_t *instance )
{
	
	// Argument check
	{
		#ifndef NDEBUG
			if(instance == (void *)0 )
				goto no_instance;
		#endif
	}
    

    // Run callbacks
    if ( instance->discord_integration.core->run_callbacks(instance->discord_integration.core) != DiscordResult_Ok )
        return 0;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_instance:
				#ifndef NDEBUG
					g_print_error("[G10] [User code] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
				#endif	
				return 0;
		}
	}
}
#endif