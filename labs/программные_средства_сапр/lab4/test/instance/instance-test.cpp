#include <catch2/catch_all.hpp>
#include "app/app.hpp"

static std::filesystem::path find_solution_dir() {
    std::filesystem::path initial_dir = std::filesystem::current_path();
    for (int i = 0; i < 6; ++i) {
        if (std::filesystem::exists(initial_dir / "04_real")) {
            return initial_dir;
        }
        if (std::filesystem::exists(initial_dir / "instance" / "solution-maximum" / "04_real")) {
            return initial_dir / "instance" / "solution-maximum";
        }
        if (std::filesystem::exists(initial_dir / "labs")) {
            for (const auto& entry : std::filesystem::directory_iterator(initial_dir / "labs")) {
                std::filesystem::path cand = entry.path() / "lab4" / "instance" / "solution-maximum";
                if (std::filesystem::exists(cand)) {
                    return cand;
                }
            }
        }
        if (initial_dir.has_parent_path() && initial_dir.parent_path() != initial_dir) {
            initial_dir = initial_dir.parent_path();
        }
    }
    return "";
}

TEST_CASE("Parse 01_simple LEF and DEF", "[simple]") {
    std::filesystem::path base = find_solution_dir();
    REQUIRE(!base.empty());

    FileHandler fh = FileHandler_T::init(base / "01_simple");
    ChipData chip = fh->get_chip_data();
    REQUIRE(chip != nullptr);

    bool ok_lef = fh->parse_lef(base / "01_simple" / "merged.nom.lef", chip);
    REQUIRE(ok_lef);
    CHECK(chip->get_lef().macros.size() == 6);
    for (const auto& [name, macro] : chip->get_lef().macros) {
        CHECK(macro.width > 0.0);
        CHECK(macro.height > 0.0);
        CHECK(macro.pins.size() > 0);
    }

    bool ok_def = fh->parse_def(base / "01_simple" / "counter.def", chip);
    REQUIRE(ok_def);
    CHECK(chip->get_die_area().width() == 80670);
    CHECK(chip->get_die_area().height() == 91390);
    CHECK(chip->get_components().size() == 18);
    CHECK(chip->get_nets().size() == 33);

    FileHandler_T::destroy(fh);
}

TEST_CASE("Parse 02_complex coordinates with asterisks", "[complex]") {
    std::filesystem::path base = find_solution_dir();
    REQUIRE(!base.empty());

    FileHandler fh = FileHandler_T::init(base / "02_complex");
    ChipData chip = fh->get_chip_data();
    REQUIRE(chip != nullptr);

    bool ok_lef = fh->parse_lef(base / "02_complex" / "merged.nom.lef", chip);
    REQUIRE(ok_lef);

    bool ok_def = fh->parse_def(base / "02_complex" / "counter.def", chip);
    REQUIRE(ok_def);
    CHECK(chip->get_components().size() == 18);
    CHECK(chip->get_nets().size() == 33);

    size_t seg_count = 0;
    for (const auto& net : chip->get_nets()) {
        seg_count += net.segments.size();
    }
    CHECK(seg_count == 127);

    FileHandler_T::destroy(fh);
}

TEST_CASE("Parse 03_all spm and majorityof5", "[all]") {
    std::filesystem::path base = find_solution_dir();
    REQUIRE(!base.empty());

    FileHandler fh = FileHandler_T::init(base / "03_all");
    ChipData chip = fh->get_chip_data();
    REQUIRE(chip != nullptr);

    bool ok_lef = fh->parse_lef(base / "03_all" / "merged.nom.lef", chip);
    REQUIRE(ok_lef);
    CHECK(chip->get_lef().macros.size() == 437);

    bool ok_maj = fh->parse_def(base / "03_all" / "majorityof5.def", chip);
    REQUIRE(ok_maj);
    CHECK(chip->get_components().size() == 4);
    CHECK(chip->get_nets().size() == 15);

    bool ok_spm = fh->parse_def(base / "03_all" / "spm.def", chip);
    REQUIRE(ok_spm);
    CHECK(chip->get_components().size() == 230);
    CHECK(chip->get_nets().size() == 309);

    FileHandler_T::destroy(fh);
}

TEST_CASE("Parse 04_real full LEF and DEF", "[real]") {
    std::filesystem::path base = find_solution_dir();
    REQUIRE(!base.empty());

    FileHandler fh = FileHandler_T::init(base / "04_real");
    ChipData chip = fh->get_chip_data();
    REQUIRE(chip != nullptr);

    bool ok_lef = fh->parse_lef(base / "04_real" / "merged.nom.lef", chip);
    REQUIRE(ok_lef);
    CHECK(chip->get_lef().macros.size() == 441);

    // Verify routing layers have correct dimensions
    auto& layers = chip->get_lef().layers;
    REQUIRE(layers.contains("met1"));
    CHECK(layers["met1"].width > 0.0);
    CHECK(layers["met1"].thickness > 0.0);
    REQUIRE(layers.contains("met5"));
    CHECK(layers["met5"].width >= 1.6);
    CHECK(layers["met5"].thickness >= 1.2);

    // Verify all 441 macros have valid dimensions
    for (const auto& [name, macro] : chip->get_lef().macros) {
        CHECK(macro.width > 0.0);
        CHECK(macro.height > 0.0);
    }

    bool ok_def = fh->parse_def(base / "04_real" / "counter.def", chip);
    REQUIRE(ok_def);
    CHECK(chip->get_components().size() == 542);
    CHECK(chip->get_nets().size() == 35);
    CHECK(chip->get_die_area().width() == 80670);
    CHECK(chip->get_die_area().height() == 91390);

    // Verify all placed components match a macro in LEF
    for (const auto& comp : chip->get_components()) {
        if (comp.is_placed) {
            CHECK(chip->get_lef().macros.contains(comp.macro_name));
        }
    }

    FileHandler_T::destroy(fh);
}
