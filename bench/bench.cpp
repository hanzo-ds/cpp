#include <benchmark/benchmark.h>

#include <datastore/client.h>
#include <ut/utils.h>

namespace datastore {

Client g_client(ClientOptions()
        .SetHost(           getEnvOrDefault("DATASTORE_HOST",     "localhost"))
        .SetPort( std::stoi(getEnvOrDefault("DATASTORE_PORT",     "9000")))
        .SetUser(           getEnvOrDefault("DATASTORE_USER",     "default"))
        .SetPassword(       getEnvOrDefault("DATASTORE_PASSWORD", ""))
        .SetDefaultDatabase(getEnvOrDefault("DATASTORE_DB",       "default"))
        .SetPingBeforeQuery(false));

static void SelectNumber(benchmark::State& state) {
    while (state.KeepRunning()) {
        g_client.Select("SELECT number, number, number FROM system.numbers LIMIT 1000",
            [](const Block& block) { block.GetRowCount(); }
        );
    }
}
BENCHMARK(SelectNumber);

static void SelectNumberMoreColumns(benchmark::State& state) {
    // Mainly test performance on type name parsing.
    while (state.KeepRunning()) {
        g_client.Select("SELECT "
                "number, number, number, number, number, number, number, number, number, number "
                "FROM system.numbers LIMIT 100",
            [](const Block& block) { block.GetRowCount(); }
        );
    }
}
BENCHMARK(SelectNumberMoreColumns);

}

BENCHMARK_MAIN();
