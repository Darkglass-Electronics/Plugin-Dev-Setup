# Audio Plugin Documentation for Darkglass Anagram

This document contains information on how plugins should handle multi-threading when on the [Darkglass Anagram](https://www.darkglass.com/products/anagram/).  
A basic set of rules to follow in order to ensure consistency within the platform.

NOTE: This assumes the Anagram is running version 1.8 or above.

## CPU core separation

Anagram has 6 CPU cores in a Big.Little fashion, 4 "little" cores and 2 "big" cores.
The "big" cores are reserved for audio processing, and the "little" cores handle everything else, potentially some background audio processing too.
The 2 support the same ARMv8.2 feature-set, so plugin binaries are always compatible with both.

Ideally plugins will make use of LV2 Worker extension for doing thread-safe non-realtime tasks through the host, but sometimes that is not possible.
In such cases, consider using threads following these restrictions:

 1. If the thread is a direct dependency of the audio processing (e.g. multi-threaded processing) use the "big" cores with RT priority, but try to keep maximum 1 extra thread for processing so that half runs directly on the audio thread, other half on the 2nd "big" core.

 2. If the thread is doing background audio processing (e.g. reverb tail that is not needed in the current audio cycle) use the "little" cores with RT priority.

 3. If the thread is doing background non-audio processing (e.g. loading files) use the "little" cores without RT priority.

When targeting "little" cores use indexes 0 to 3.

When targeting "big" cores use indexes 4 to 7.
We use 4 cores here for possible future compatibility, as the RK3582 SoC used in Anagram has 2 "big" cores but is socket-compatible with RK3588 which has 4 "big" cores.

**NOTE: Do not create extra threads with RT priority without pinning to specific CPU cores, as then it is random which cores will be running it which creates unstable/unreliable DSP time consumption.**

## RT priority

The plugin host used in Anagram reports to plugins the RT priority they should use for new threads through [threads:schedPriority](http://ardour.org/lv2/threads/#schedPriority) and [threads:schedPolicy](http://ardour.org/lv2/threads/#schedPolicy).

If you do not have access to this LV2 extension value (e.g. using a framework that does not expose it), you can use `MOD_PLUGIN_THREAD_PRIORITY` environment variable, which is hardcoded to `57`.

## Example

Creating a thread for audio tasks, on either "big" or "little" cores separately:

```c
#ifdef _DARKGLASS_DEVICE_PABLITO
    // initialize thread attributes for RT usage
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    struct sched_param param = {
      .sched_priority = 57;
    };
    pthread_attr_setschedparam(&attr, &param);

    // pin thread to specific CPU cores
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    if (usingBigCores)
    {
        for (int i = 4; i < 8; ++i)
            CPU_SET(i, &cpuset);
    }
    else
    {
        for (int i = 0; i < 4; ++i)
            CPU_SET(i, &cpuset);
    }
    pthread_attr_setaffinity_np(&attr, sizeof(cpuset), &cpuset);

    // create the thread
    pthread_t handle;
    pthread_create(&handle, &attr, _threadEntryPoint, _threadArg);

    // don't forget to do cleanup!
    pthread_attr_destroy(&attr);
#endif
```
