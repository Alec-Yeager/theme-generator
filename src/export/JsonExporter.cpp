#include "JsonExporter.hpp"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <ostream>
#include <pwd.h>
#include <spdlog/spdlog.h>
#include <unistd.h>

namespace fs = std::filesystem;
using json = nlohmann::json;

std::string getHomeDirectory() {
    const char *home = std::getenv("HOME");
    if (!home) {
        home = std::getenv("USERPROFILE");
    }
    if (!home) {
        struct passwd *pw = getpwuid(getuid());
        home = pw ? pw->pw_dir : nullptr;
    }
    return home ? std::string(home) : "";
}

void JsonExporter::exportPalette(const Palette &p) {
    auto savepath = fs::path(getHomeDirectory());
    savepath = savepath / ".config" / "theme-generator";
    fs::create_directories(savepath);
    savepath = savepath / "palette.json";

    json pallete_json;
    // Recall cv is BGR.
    SPDLOG_DEBUG("FG: ({}, {}, {})", p.foreground()[2], p.foreground()[1], p.foreground()[0]);
    pallete_json["foreground"]["rgb"]["r"] = p.foreground()[2];
    pallete_json["foreground"]["rgb"]["g"] = p.foreground()[1];
    pallete_json["foreground"]["rgb"]["b"] = p.foreground()[0];
    pallete_json["foreground"]["hex"] = bgrToHex(p.foreground());
    SPDLOG_DEBUG("HEX: {}", bgrToHex(p.foreground()));

    SPDLOG_DEBUG("BG: ({}, {}, {})", p.background()[2], p.background()[1], p.background()[0]);
    pallete_json["background"]["rgb"]["r"] = p.background()[2];
    pallete_json["background"]["rgb"]["g"] = p.background()[1];
    pallete_json["background"]["rgb"]["b"] = p.background()[0];
    pallete_json["background"]["hex"] = bgrToHex(p.background());

    for (int i = 0; i < p.colors().size(); ++i) {
        auto &color = p.colors()[i];
        pallete_json["colors"][i]["rgb"]["r"] = color[2];
        pallete_json["colors"][i]["rgb"]["g"] = color[1];
        pallete_json["colors"][i]["rgb"]["b"] = color[0];
        pallete_json["colors"][i]["hex"] = bgrToHex(color);
    }

    std::ofstream outfile(savepath);
    outfile << pallete_json.dump(2);
    return;
}