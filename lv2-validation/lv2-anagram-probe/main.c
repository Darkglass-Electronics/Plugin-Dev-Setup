#include <lilv/lilv.h>
#include <lv2/urid/urid.h>
#include <lv2/worker/worker.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_URIDS 256
static char* urid_map[MAX_URIDS];
static LV2_URID urid_count = 0;

static LV2_URID urid_map_func(LV2_URID_Map_Handle handle, const char* uri) {
    for (LV2_URID i = 0; i < urid_count; i++) {
        if (strcmp(urid_map[i], uri) == 0) return i + 1;
    }
    urid_map[urid_count++] = strdup(uri);
    return urid_count;
}

static LV2_Worker_Status worker_schedule_func(LV2_Worker_Schedule_Handle handle,
        uint32_t size, const void* data) {
    return LV2_WORKER_SUCCESS;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <bundle-path> <plugin-uri>\n", argv[0]);
        return 1;
    }

    const char* bundle_path = argv[1];
    const char* plugin_uri_str = argv[2];

    LilvWorld* world = lilv_world_new();
    LilvNode* bundle_uri = lilv_new_file_uri(world, NULL, bundle_path);
    lilv_world_load_bundle(world, bundle_uri);
    lilv_world_load_specifications(world);

    LilvNode* plugin_uri = lilv_new_uri(world, plugin_uri_str);
    const LilvPlugins* plugins = lilv_world_get_all_plugins(world);
    const LilvPlugin* plugin = lilv_plugins_get_by_uri(plugins, plugin_uri);

    if (!plugin) {
        fprintf(stderr, "Plugin not found\n");
        lilv_node_free(plugin_uri);
        lilv_node_free(bundle_uri);
        lilv_world_free(world);
        return 1;
    }

    LV2_URID_Map map = { NULL, urid_map_func };
    LV2_Feature map_feature = { LV2_URID__map, &map };
    LV2_Worker_Schedule worker_schedule = { NULL, worker_schedule_func };
    LV2_Feature worker_feature = { LV2_WORKER__schedule, &worker_schedule };
    const LV2_Feature* features[] = { &map_feature, &worker_feature, NULL };

    LilvInstance* instance = lilv_plugin_instantiate(plugin, 48000.0, features);
    if (!instance) {
        fprintf(stderr, "Failed to instantiate plugin\n");
        lilv_node_free(plugin_uri);
        lilv_node_free(bundle_uri);
        lilv_world_free(world);
        return 1;
    }

    LilvNodes* extensions = lilv_plugin_get_extension_data(plugin);
    LILV_FOREACH(nodes, i, extensions) {
        const LilvNode* ext = lilv_nodes_get(extensions, i);
        printf("Declared extension: %s\n", lilv_node_as_uri(ext));
        const void* data = lilv_instance_get_extension_data(instance, lilv_node_as_uri(ext));
        printf("  - Binary provides it: %s\n", data ? "yes" : "NO (mismatch!)");
    }

    float dummy[256] = {0};
    uint32_t num_ports = lilv_plugin_get_num_ports(plugin);
    for (uint32_t i = 0; i < num_ports; i++) {
        printf("checking port %d\n", i);
        lilv_instance_connect_port(instance, i, dummy);
    }

    lilv_instance_activate(instance);
    lilv_instance_run(instance, 256);
    lilv_instance_deactivate(instance);
    lilv_instance_free(instance);

    for (LV2_URID i = 0; i < urid_count; i++) free(urid_map[i]);

    lilv_nodes_free(extensions);
    lilv_node_free(plugin_uri);
    lilv_node_free(bundle_uri);
    lilv_world_free(world);
    return 0;
}
