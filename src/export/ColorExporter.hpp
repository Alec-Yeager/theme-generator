#pragma once
#include "../coloring/Palette.hpp"

class ColorExporter {

public:
    ColorExporter() = default;
    virtual ~ColorExporter() = default;

    virtual void exportPalette(const Palette &p) = 0;
    virtual std::string name() const = 0;
};
