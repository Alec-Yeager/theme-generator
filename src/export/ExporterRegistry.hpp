#pragma once

#include "../coloring/Palette.hpp"
#include "ColorExporter.hpp"

class ExporterRegistry {

public:
    ExporterRegistry() = default;
    ~ExporterRegistry() = default;

    void registerExporter(std::unique_ptr<ColorExporter> exporter);
    void runExporters(const Palette &p, std::vector<std::string> exporters);

private:
    std::map<std::string, std::unique_ptr<ColorExporter>> exporter_map_;
};