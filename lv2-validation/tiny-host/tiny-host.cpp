#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <lilv/lilv.h>
#include <lv2/atom/atom.h>
#include <lv2/buf-size/buf-size.h>
#include <lv2/options/options.h>
#include <lv2/urid/urid.h>
#include <lv2/worker/worker.h>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

enum {
    k_urid_null,
    k_urid_atom_bool,
    k_urid_atom_int,
    k_urid_atom_long,
    k_urid_atom_float,
    k_urid_atom_double,
};

// Helper to hash uri, to create a unique but safe filename
static std::string hash_uri(const std::string& s) {
    uint64_t h = 0xcbf29ce484222325ULL;
    for (unsigned char c : s) {
        h ^= c;
        h *= 0x100000001b3ULL;
    }
    std::ostringstream out;
    out << std::hex << std::setw(16) << std::setfill('0') << h;
    return out.str();
}

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

    std::string msg;

    const char* bundle_path    = argv[1];
    const char* plugin_uri_str = argv[2];

    // If we actually have warnings / pedantic info later, we can write those to a different file.
    // So that the python script has an easy way to differentiate.
    const std::string filename_err = "/tmp/" + hash_uri(plugin_uri_str)
        + "_tinyhost_testresult_err.txt";

    LilvWorld* world = lilv_world_new();
    LilvNode* bundle_uri = lilv_new_file_uri(world, nullptr, bundle_path);
    lilv_world_load_bundle(world, bundle_uri);
    lilv_world_load_specifications(world);

    LilvNode* plugin_uri = lilv_new_uri(world, plugin_uri_str);
    const LilvPlugins* plugins = lilv_world_get_all_plugins(world);
    const LilvPlugin* plugin = lilv_plugins_get_by_uri(plugins, plugin_uri);
    LilvInstance* instance = nullptr;

    auto cleanup = [&] {
        if (instance) lilv_instance_free(instance);
        lilv_node_free(plugin_uri);
        lilv_node_free(bundle_uri);
        lilv_world_free(world);
    };

    auto write_err_msg = [&]() {
        std::ofstream out(filename_err);
        if (out) out << msg;
    };

    if (!plugin) {
        msg += "Plugin not found\n";
        cleanup();
        return 1;
    }

    LV2_URID_Map map = { nullptr, mapfn };

    // TODO, are any of these actually not supported on Anagram? If so maybe best to remove it
    LV2_Feature map_feature = {
        LV2_URID__map,
        &map
    };
    LV2_Feature bounded_block_length_feature = {
        LV2_BUF_SIZE__boundedBlockLength, nullptr
    };
    LV2_Feature coarsed_block_length_feature = {
        LV2_BUF_SIZE__coarseBlockLength, nullptr
    };
    LV2_Feature fixed_block_length_feature = {
        LV2_BUF_SIZE__fixedBlockLength, nullptr
    };
    LV2_Feature max_block_length_feature = {
        LV2_BUF_SIZE__maxBlockLength, nullptr
    };
    LV2_Feature min_block_length_feature = {
        LV2_BUF_SIZE__minBlockLength, nullptr
    };
    LV2_Feature nominal_block_length_feature = {
        LV2_BUF_SIZE__nominalBlockLength, nullptr
    };
    LV2_Feature powerof2_block_length_feature = {
        LV2_BUF_SIZE__powerOf2BlockLength, nullptr
    };
    LV2_Feature sequenceSize_block_length_feature = {
        LV2_BUF_SIZE__sequenceSize, nullptr
    };
    LV2_Feature license_feature = {
        "http://www.darkglass.com/lv2/ns/lv2ext/license#feature", nullptr
    };
    LV2_Feature license_feature_old = {
        "http://moddevices.com/ns/ext/license#feature", nullptr
    };

    LV2_Feature in_place_broken_feature = {
        LV2_CORE__inPlaceBroken, nullptr
    };

    LV2_Worker_Schedule worker_schedule = { nullptr, worker_schedule_func };
    LV2_Feature worker_feature = { LV2_WORKER__schedule, &worker_schedule };

    const LV2_Feature* features[] = {
        &map_feature,
        &worker_feature,
        &options_feature,
        &bounded_block_length_feature,
        &coarsed_block_length_feature,
        &fixed_block_length_feature,
        &max_block_length_feature,
        &min_block_length_feature,
        &nominal_block_length_feature,
        &powerof2_block_length_feature,
        &sequenceSize_block_length_feature,
        &in_place_broken_feature,
        &license_feature,
        &license_feature_old,
        nullptr
    };

    bool features_missing = false;
    // Check required features before instantiate, so a missing feature gives
    // an informative error rather than a silent NULL.
    LilvNodes* required = lilv_plugin_get_required_features(plugin);
    LILV_FOREACH(nodes, i, required) {
        const char* uri = lilv_node_as_uri(lilv_nodes_get(required, i));
        if (!uri) continue;

        bool supplied = false;
        for (const LV2_Feature* f : features) {
            if (f && std::strcmp(f->URI, uri) == 0) { supplied = true; break; }
        }
        if (!supplied) {
            msg += "Missing required feature: ";
            msg += uri;
            msg += "\n";
            features_missing = true;
        }
    }
    lilv_nodes_free(required);

    if (features_missing != 0) {
        cleanup();
        write_err_msg();
        return 1;
    }

    instance = lilv_plugin_instantiate(plugin, 48000.0, features);
    if (!instance) {
        msg += "Failed to instantiate plugin\n";
        cleanup();
        write_err_msg();
        return 1;
    }

    // Check if the extension declaration matches between binary and TTL
    const char* const license_interface = "http://www.darkglass.com/lv2/ns/lv2ext/license#interface";
    const char* const license_interface_legacy = "http://moddevices.com/ns/ext/license#interface";
    bool licenseInTtl = false;
    bool licenseLegacyInTtl = false;

    LilvNodes* extensions = lilv_plugin_get_extension_data(plugin);
    LILV_FOREACH(nodes, i, extensions) {
        const LilvNode* ext = lilv_nodes_get(extensions, i);
        if (const char* uri = lilv_node_as_uri(ext)) {
            if (std::strcmp(uri, license_interface) == 0)
                licenseInTtl = true;
            else if (std::strcmp(uri, license_interface_legacy) == 0)
                licenseLegacyInTtl = true;
        }
    }
    lilv_nodes_free(extensions);

    if (licenseInTtl != !!lilv_instance_get_extension_data(instance, license_interface)) {
        msg += "Declared extension: ";
        msg += license_interface;
        msg += "\n  - ";
        msg += licenseInTtl
             ? "In ttl but binary does NOT provide it (mismatch!)\n"
             : "Binary provides it but ttl does NOT declare it (mismatch!)\n";
    }
    if (licenseLegacyInTtl != !!lilv_instance_get_extension_data(instance, license_interface_legacy)) {
        msg += "Declared extension: ";
        msg += license_interface_legacy;
        msg += "\n  - ";
        msg += licenseLegacyInTtl
             ? "In ttl but binary does NOT provide it (mismatch!)\n"
             : "Binary provides it but ttl does NOT declare it (mismatch!)\n";
    }

    // Connect ports, so we can see with a tool like valgrind if there is a memory issue here.
    // No per-port validation here, since TTL validity is handled upstream by lv2-anagram-validate script.
    uint32_t num_ports = lilv_plugin_get_num_ports(plugin);
    LilvNode* input_class = lilv_new_uri(world, LILV_URI_INPUT_PORT);
    LilvNode* atom_class  = lilv_new_uri(world, LV2_ATOM__AtomPort);
    const LV2_URID urid_atom_seq = mapfn(nullptr, LV2_ATOM__Sequence);

    std::vector<std::vector<uint8_t>> port_buffers(num_ports);

    for (uint32_t i = 0; i < num_ports; ++i) {
        const LilvPort* port = lilv_plugin_get_port_by_index(plugin, i);
        if (!port) continue;

        const bool is_input = lilv_port_is_a(plugin, port, input_class);

        if (lilv_port_is_a(plugin, port, atom_class)) {
            constexpr size_t atom_buf_size = 1024;
            port_buffers[i].resize(atom_buf_size, 0);
            auto* seq = reinterpret_cast<LV2_Atom_Sequence*>(port_buffers[i].data());
            seq->atom.type = urid_atom_seq;
            seq->atom.size = is_input ? sizeof(LV2_Atom_Sequence_Body)
                                       : atom_buf_size - sizeof(LV2_Atom);
            if (is_input) { seq->body.unit = 0; seq->body.pad = 0; }
        } else {
            port_buffers[i].resize(block_length * sizeof(float), 0);
        }

        lilv_instance_connect_port(instance, i, port_buffers[i].data());
    }

    lilv_node_free(input_class);
    lilv_node_free(atom_class);

    write_err_msg();

    lilv_instance_activate(instance);
    lilv_instance_run(instance, 256);
    lilv_instance_deactivate(instance);

    cleanup();
    return 0;
}
