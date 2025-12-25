#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"
#include "../src/Life.hpp"

// Testable Life class that exposes protected methods
class TestableLife : public Life {
public:
	TestableLife(uint32_t size) : Life(size) {}
	
	// Expose protected methods for testing
	string test_iterate_generations(uint32_t count) {
		return iterate_generations(count);
	}
	
	DSCmatrix test_generate_matrix(const string& input_file) {
		return generate_matrix(input_file);
	}
	
	// Get access to the matrix
	DSCmatrix& get_matrix() {
		return dsc_matrix;
	}
};

// Helper function to set pattern on matrix
void set_pattern(DSCmatrix& matrix, const std::vector<std::pair<uint32_t, uint32_t>>& alive_cells) {
	DSCsize size = matrix->get_size();
	// First set all to false
	for (uint32_t row = 0; row < size.vertical_size; row++) {
		for (uint32_t col = 0; col < size.horizontal_size; col++) {
			matrix[{col, row}] = false;
		}
	}
	// Then set alive cells
	for (const auto& cell : alive_cells) {
		matrix[{cell.first, cell.second}] = true;
	}
}

// Helper function to check if pattern matches
bool check_pattern(DSCmatrix& matrix, const std::vector<std::pair<uint32_t, uint32_t>>& alive_cells) {
	DSCsize size = matrix->get_size();
	for (uint32_t row = 0; row < size.vertical_size; row++) {
		for (uint32_t col = 0; col < size.horizontal_size; col++) {
			bool should_be_alive = false;
			for (const auto& cell : alive_cells) {
				if (cell.first == col && cell.second == row) {
					should_be_alive = true;
					break;
				}
			}
			if (matrix[{col, row}] != should_be_alive) {
				return false;
			}
		}
	}
	return true;
}

TEST_CASE("Life: Empty pattern stays empty", "[Life][patterns]") {
	TestableLife life(5);
	DSCmatrix& matrix = life.get_matrix();
	
	// Set all cells to false
	set_pattern(matrix, {});
	
	// After any number of iterations, it should remain empty
	life.test_iterate_generations(5);
	
	REQUIRE(check_pattern(matrix, {}));
}

TEST_CASE("Life: Single cell dies", "[Life][patterns]") {
	TestableLife life(5);
	DSCmatrix& matrix = life.get_matrix();
	
	// Set single cell alive
	set_pattern(matrix, {{2, 2}});
	
	// After one generation, it should die (underpopulation)
	life.test_iterate_generations(1);
	
	REQUIRE(check_pattern(matrix, {}));
}

TEST_CASE("Life: Block pattern (still life)", "[Life][patterns]") {
	TestableLife life(6);
	DSCmatrix& matrix = life.get_matrix();
	
	// Block pattern (2x2 square) - stable
	std::vector<std::pair<uint32_t, uint32_t>> block = {
		{2, 2}, {3, 2},
		{2, 3}, {3, 3}
	};
	
	set_pattern(matrix, block);
	
	SECTION("Block remains stable after 1 generation") {
		life.test_iterate_generations(1);
		REQUIRE(check_pattern(matrix, block));
	}
	
	SECTION("Block remains stable after 10 generations") {
		life.test_iterate_generations(10);
		REQUIRE(check_pattern(matrix, block));
	}
}

TEST_CASE("Life: Beehive pattern (still life)", "[Life][patterns]") {
	TestableLife life(8);
	DSCmatrix& matrix = life.get_matrix();
	
	// Beehive pattern - stable
	std::vector<std::pair<uint32_t, uint32_t>> beehive = {
		        {3, 2}, {4, 2},
		{2, 3},               {5, 3},
		        {3, 4}, {4, 4}
	};
	
	set_pattern(matrix, beehive);
	life.test_iterate_generations(1);
	
	REQUIRE(check_pattern(matrix, beehive));
}

TEST_CASE("Life: Blinker pattern (oscillator period 2)", "[Life][patterns]") {
	TestableLife life(5);
	DSCmatrix& matrix = life.get_matrix();
	
	// Blinker horizontal
	std::vector<std::pair<uint32_t, uint32_t>> blinker_h = {
		{1, 2}, {2, 2}, {3, 2}
	};
	
	// Blinker vertical
	std::vector<std::pair<uint32_t, uint32_t>> blinker_v = {
		        {2, 1},
		        {2, 2},
		        {2, 3}
	};
	
	set_pattern(matrix, blinker_h);
	REQUIRE(check_pattern(matrix, blinker_h));
	
	// After one generation should be vertical
	life.test_iterate_generations(1);
	REQUIRE(check_pattern(matrix, blinker_v));
	
	// After two generations should be horizontal again
	life.test_iterate_generations(1);
	REQUIRE(check_pattern(matrix, blinker_h));
}

TEST_CASE("Life: Toad pattern (oscillator period 2)", "[Life][patterns]") {
	TestableLife life(6);
	DSCmatrix& matrix = life.get_matrix();
	
	// Toad phase 1
	std::vector<std::pair<uint32_t, uint32_t>> toad_p1 = {
		        {2, 2}, {3, 2}, {4, 2},
		{1, 3}, {2, 3}, {3, 3}
	};
	
	// Toad phase 2
	std::vector<std::pair<uint32_t, uint32_t>> toad_p2 = {
		                {3, 1},
		{1, 2},                 {4, 2},
		{1, 3},                 {4, 3},
		{2, 4}
	};
	
	set_pattern(matrix, toad_p1);
	REQUIRE(check_pattern(matrix, toad_p1));
	
	life.test_iterate_generations(1);
	REQUIRE(check_pattern(matrix, toad_p2));
	
	life.test_iterate_generations(1);
	REQUIRE(check_pattern(matrix, toad_p1));
}

TEST_CASE("Life: Glider pattern (spaceship)", "[Life][patterns]") {
	TestableLife life(10);
	DSCmatrix& matrix = life.get_matrix();
	
	// Glider initial position
	std::vector<std::pair<uint32_t, uint32_t>> glider_gen0 = {
		        {2, 1},
		                {3, 2},
		{1, 3}, {2, 3}, {3, 3}
	};
	
	set_pattern(matrix, glider_gen0);
	REQUIRE(check_pattern(matrix, glider_gen0));
	
	// Glider after 4 generations should move diagonally
	std::vector<std::pair<uint32_t, uint32_t>> glider_gen4 = {
		                {3, 2},
		                        {4, 3},
		        {2, 4}, {3, 4}, {4, 4}
	};
	
	life.test_iterate_generations(4);
	REQUIRE(check_pattern(matrix, glider_gen4));
}

TEST_CASE("Life: Beacon pattern (oscillator period 2)", "[Life][patterns]") {
	TestableLife life(6);
	DSCmatrix& matrix = life.get_matrix();
	
	// Beacon phase 1
	std::vector<std::pair<uint32_t, uint32_t>> beacon_p1 = {
		{1, 1}, {2, 1},
		{1, 2},
		                {4, 3},
		        {3, 4}, {4, 4}
	};
	
	// Beacon phase 2
	std::vector<std::pair<uint32_t, uint32_t>> beacon_p2 = {
		{1, 1}, {2, 1},
		{1, 2}, {2, 2},
		        {3, 3}, {4, 3},
		        {3, 4}, {4, 4}
	};
	
	set_pattern(matrix, beacon_p1);
	REQUIRE(check_pattern(matrix, beacon_p1));
	
	life.test_iterate_generations(1);
	REQUIRE(check_pattern(matrix, beacon_p2));
	
	life.test_iterate_generations(1);
	REQUIRE(check_pattern(matrix, beacon_p1));
}

TEST_CASE("Life: Boat pattern (still life)", "[Life][patterns]") {
	TestableLife life(6);
	DSCmatrix& matrix = life.get_matrix();
	
	// Boat pattern - stable
	std::vector<std::pair<uint32_t, uint32_t>> boat = {
		{2, 2}, {3, 2},
		{2, 3},         {4, 3},
		        {3, 4}
	};
	
	set_pattern(matrix, boat);
	life.test_iterate_generations(1);
	
	REQUIRE(check_pattern(matrix, boat));
}

TEST_CASE("Life: Loaf pattern (still life)", "[Life][patterns]") {
	TestableLife life(7);
	DSCmatrix& matrix = life.get_matrix();
	
	// Loaf pattern - stable
	std::vector<std::pair<uint32_t, uint32_t>> loaf = {
		        {3, 2}, {4, 2},
		{2, 3},                 {5, 3},
		        {3, 4},         {5, 4},
		                {4, 5}
	};
	
	set_pattern(matrix, loaf);
	life.test_iterate_generations(1);
	
	REQUIRE(check_pattern(matrix, loaf));
}

TEST_CASE("Life: Three cells in a row oscillates (blinker)", "[Life][patterns]") {
	TestableLife life(5);
	DSCmatrix& matrix = life.get_matrix();
	
	// Three cells in a row horizontal
	std::vector<std::pair<uint32_t, uint32_t>> horizontal = {
		{1, 2}, {2, 2}, {3, 2}
	};
	
	std::vector<std::pair<uint32_t, uint32_t>> vertical = {
		        {2, 1},
		        {2, 2},
		        {2, 3}
	};
	
	set_pattern(matrix, horizontal);
	REQUIRE(check_pattern(matrix, horizontal));
	
	life.test_iterate_generations(1);
	REQUIRE(check_pattern(matrix, vertical));
}

TEST_CASE("Life: Four cells in a square (block) is stable", "[Life][patterns]") {
	TestableLife life(5);
	DSCmatrix& matrix = life.get_matrix();
	
	// 2x2 block
	std::vector<std::pair<uint32_t, uint32_t>> block = {
		{1, 1}, {2, 1},
		{1, 2}, {2, 2}
	};
	
	set_pattern(matrix, block);
	REQUIRE(check_pattern(matrix, block));
	
	// Block should remain stable after any number of generations
	life.test_iterate_generations(5);
	REQUIRE(check_pattern(matrix, block));
}

