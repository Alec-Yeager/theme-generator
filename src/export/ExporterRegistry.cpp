#include "ExporterRegistry.hpp"
#include <spdlog/spdlog.h>

void ExporterRegistry::registerExporter(std::unique_ptr<ColorExporter> exporter) {
    exporter_map_[exporter->name()] = std::move(exporter);
}

void ExporterRegistry::runExporters(const Palette &p, std::vector<std::string> exporters) {
    for (auto &name : exporters) {
        auto exp = exporter_map_.find(name);
        if (exp == exporter_map_.end()) {
            SPDLOG_WARN("Unable to find exporter named {}, is it registered?", name);
            continue;
        }
        exp->second->exportPalette(p);
    }
}
