#pragma once
#include "ColorExporter.hpp"

class JsonExporter : public ColorExporter {
public:
    JsonExporter() = default;
    ~JsonExporter() = default;

    std::string name() const override { return "JSON"; };
    void exportPalette(const Palette &p) override;
};
