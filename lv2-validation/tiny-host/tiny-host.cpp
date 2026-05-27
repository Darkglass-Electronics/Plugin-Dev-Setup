#include <lilv/lilv.h>
#include <lv2/buf-size/buf-size.h>
#include <lv2/options/options.h>
#include <lv2/urid/urid.h>
#include <lv2/options/options.h>
#include <lv2/worker/worker.h>
#include <cstdio>
#include <string>
#include <vector>

#define LV2_ATOM_URI    "http://lv2plug.in/ns/ext/atom"  ///< http://lv2plug.in/ns/ext/atom
#define LV2_ATOM_PREFIX LV2_ATOM_URI "#"                 ///< http://lv2plug.in/ns/ext/atom#

#define LV2_ATOM__Atom          LV2_ATOM_PREFIX "Atom"           ///< http://lv2plug.in/ns/ext/atom#Atom
#define LV2_ATOM__AtomPort      LV2_ATOM_PREFIX "AtomPort"       ///< http://lv2plug.in/ns/ext/atom#AtomPort
#define LV2_ATOM__Blank         LV2_ATOM_PREFIX "Blank"          ///< http://lv2plug.in/ns/ext/atom#Blank
#define LV2_ATOM__Bool          LV2_ATOM_PREFIX "Bool"           ///< http://lv2plug.in/ns/ext/atom#Bool
#define LV2_ATOM__Chunk         LV2_ATOM_PREFIX "Chunk"          ///< http://lv2plug.in/ns/ext/atom#Chunk
#define LV2_ATOM__Double        LV2_ATOM_PREFIX "Double"         ///< http://lv2plug.in/ns/ext/atom#Double
#define LV2_ATOM__Event         LV2_ATOM_PREFIX "Event"          ///< http://lv2plug.in/ns/ext/atom#Event
#define LV2_ATOM__Float         LV2_ATOM_PREFIX "Float"          ///< http://lv2plug.in/ns/ext/atom#Float
#define LV2_ATOM__Int           LV2_ATOM_PREFIX "Int"            ///< http://lv2plug.in/ns/ext/atom#Int
#define LV2_ATOM__Literal       LV2_ATOM_PREFIX "Literal"        ///< http://lv2plug.in/ns/ext/atom#Literal
#define LV2_ATOM__Long          LV2_ATOM_PREFIX "Long"           ///< http://lv2plug.in/ns/ext/atom#Long
#define LV2_ATOM__Number        LV2_ATOM_PREFIX "Number"         ///< http://lv2plug.in/ns/ext/atom#Number
#define LV2_ATOM__Object        LV2_ATOM_PREFIX "Object"         ///< http://lv2plug.in/ns/ext/atom#Object
#define LV2_ATOM__Path          LV2_ATOM_PREFIX "Path"           ///< http://lv2plug.in/ns/ext/atom#Path
#define LV2_ATOM__Property      LV2_ATOM_PREFIX "Property"       ///< http://lv2plug.in/ns/ext/atom#Property
#define LV2_ATOM__Resource      LV2_ATOM_PREFIX "Resource"       ///< http://lv2plug.in/ns/ext/atom#Resource
#define LV2_ATOM__Sequence      LV2_ATOM_PREFIX "Sequence"       ///< http://lv2plug.in/ns/ext/atom#Sequence
#define LV2_ATOM__Sound         LV2_ATOM_PREFIX "Sound"          ///< http://lv2plug.in/ns/ext/atom#Sound
#define LV2_ATOM__String        LV2_ATOM_PREFIX "String"         ///< http://lv2plug.in/ns/ext/atom#String
#define LV2_ATOM__Tuple         LV2_ATOM_PREFIX "Tuple"          ///< http://lv2plug.in/ns/ext/atom#Tuple
#define LV2_ATOM__URI           LV2_ATOM_PREFIX "URI"            ///< http://lv2plug.in/ns/ext/atom#URI
#define LV2_ATOM__URID          LV2_ATOM_PREFIX "URID"           ///< http://lv2plug.in/ns/ext/atom#URID
#define LV2_ATOM__Vector        LV2_ATOM_PREFIX "Vector"         ///< http://lv2plug.in/ns/ext/atom#Vector
#define LV2_ATOM__atomTransfer  LV2_ATOM_PREFIX "atomTransfer"   ///< http://lv2plug.in/ns/ext/atom#atomTransfer
#define LV2_ATOM__beatTime      LV2_ATOM_PREFIX "beatTime"       ///< http://lv2plug.in/ns/ext/atom#beatTime
#define LV2_ATOM__bufferType    LV2_ATOM_PREFIX "bufferType"     ///< http://lv2plug.in/ns/ext/atom#bufferType
#define LV2_ATOM__childType     LV2_ATOM_PREFIX "childType"      ///< http://lv2plug.in/ns/ext/atom#childType
#define LV2_ATOM__eventTransfer LV2_ATOM_PREFIX "eventTransfer"  ///< http://lv2plug.in/ns/ext/atom#eventTransfer
#define LV2_ATOM__frameTime     LV2_ATOM_PREFIX "frameTime"      ///< http://lv2plug.in/ns/ext/atom#frameTime
#define LV2_ATOM__supports      LV2_ATOM_PREFIX "supports"       ///< http://lv2plug.in/ns/ext/atom#supports
#define LV2_ATOM__timeUnit      LV2_ATOM_PREFIX "timeUnit"       ///< http://lv2plug.in/ns/ext/atom#timeUnit

enum {
    k_urid_null,
    k_urid_atom_bool,
    k_urid_atom_int,
    k_urid_atom_long,
    k_urid_atom_float,
    k_urid_atom_double,
};

static LV2_URID mapfn(LV2_URID_Map_Handle, const char* const uri)
{
    if (uri == nullptr || uri[0] == '\0')
        return k_urid_null;

    static std::vector<std::string> mapping = {
        LV2_ATOM__Bool,
        LV2_ATOM__Int,
        LV2_ATOM__Long,
        LV2_ATOM__Float,
        LV2_ATOM__Double,
    };

    LV2_URID urid = 1;
    for (const std::string& uri2 : mapping)
    {
        if (uri2 == uri)
            return urid;
        ++urid;
    }

    mapping.emplace_back(uri);
    return urid;
}

static const int32_t block_length = 256;

LV2_URID urid_buf_max  = mapfn(nullptr, LV2_BUF_SIZE__maxBlockLength);
LV2_URID urid_buf_nom  = mapfn(nullptr, LV2_BUF_SIZE__nominalBlockLength);
LV2_URID urid_atom_int = mapfn(nullptr, LV2_ATOM__Int);

static const LV2_Options_Option options[] = {
    {
        LV2_OPTIONS_INSTANCE, 0,
        urid_buf_max,
        sizeof(int32_t), urid_atom_int, &block_length
    },
    {
        LV2_OPTIONS_INSTANCE, 0,
        urid_buf_nom,
        sizeof(int32_t), urid_atom_int, &block_length
    },
    { LV2_OPTIONS_INSTANCE, 0, 0, 0, 0, nullptr }
};

LV2_Feature options_feature = { LV2_OPTIONS__options, const_cast<LV2_Options_Option*>(options) };

static LV2_Worker_Status worker_schedule_func(LV2_Worker_Schedule_Handle,
        uint32_t, const void*)
{
    return LV2_WORKER_SUCCESS;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <bundle-path> <plugin-uri>\n", argv[0]);
        return 1;
    }

    const char* bundle_path   = argv[1];
    const char* plugin_uri_str = argv[2];

    LilvWorld* world = lilv_world_new();
    LilvNode* bundle_uri = lilv_new_file_uri(world, nullptr, bundle_path);
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

    LV2_URID_Map map = { nullptr, mapfn };
    LV2_Feature map_feature = { LV2_URID__map, &map };

    LV2_Worker_Schedule worker_schedule = { nullptr, worker_schedule_func };
    LV2_Feature worker_feature = { LV2_WORKER__schedule, &worker_schedule };

    const LV2_Feature* features[] = { &map_feature, &worker_feature, &options_feature, nullptr };

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

    lilv_nodes_free(extensions);
    lilv_node_free(plugin_uri);
    lilv_node_free(bundle_uri);
    lilv_world_free(world);

    return 0;
}
