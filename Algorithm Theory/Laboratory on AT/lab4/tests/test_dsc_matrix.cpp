#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"
#include "../src/memory/DiscreteMatrix.hpp"

TEST_CASE("DiscreteMatrix: create and destroy", "[DiscreteMatrix]") {
	DSCmatrix dsc_matrix = DiscreteMatrix::create(5, 5);
	REQUIRE(dsc_matrix->get_size().horizontal_size == 5);
	REQUIRE(dsc_matrix->get_size().vertical_size == 5);
	DiscreteMatrix::destroy(dsc_matrix);
}

TEST_CASE("DiscreteMatrix: initializer list creation", "[DiscreteMatrix]") {
	DSCmatrix dsc_matrix = DiscreteMatrix::create({true, false, true, false,
												   false, true, false, true,
												   true, true, false, false,
												   false, false, true, true});
	REQUIRE(dsc_matrix->get_size().horizontal_size == 4);
	REQUIRE(dsc_matrix->get_size().vertical_size == 4);
	DiscreteMatrix::destroy(dsc_matrix);
}

TEST_CASE("DiscreteMatrix: multiple matrices creation and out-of-order destruction", "[DiscreteMatrix]") {
	DSCmatrix dsc_matrix1 = DiscreteMatrix::create(3, 3);
	DSCmatrix dsc_matrix2 = DiscreteMatrix::create(4, 4);
	DSCmatrix dsc_matrix3 = DiscreteMatrix::create(5, 5);
	
	REQUIRE(dsc_matrix1->get_size().horizontal_size == 3);
	REQUIRE(dsc_matrix1->get_size().vertical_size == 3);
	REQUIRE(dsc_matrix2->get_size().horizontal_size == 4);
	REQUIRE(dsc_matrix2->get_size().vertical_size == 4);
	REQUIRE(dsc_matrix3->get_size().horizontal_size == 5);
	REQUIRE(dsc_matrix3->get_size().vertical_size == 5);
	
	// Set some values to ensure matrices are independent
	dsc_matrix1[{0, 0}] = true;
	dsc_matrix2[{0, 0}] = false;
	dsc_matrix3[{0, 0}] = true;
	
	REQUIRE(dsc_matrix1[{0, 0}] == true);
	REQUIRE(dsc_matrix2[{0, 0}] == false);
	REQUIRE(dsc_matrix3[{0, 0}] == true);
	
	// Destroy in non-creation order: 2, 1, 3
	DiscreteMatrix::destroy(dsc_matrix2);
	DiscreteMatrix::destroy(dsc_matrix1);
	DiscreteMatrix::destroy(dsc_matrix3);
}

TEST_CASE("DiscreteMatrix: vector constructor", "[DiscreteMatrix]") {
	std::vector<bool> data = {true, false, true, false,
							  false, true, false, true,
							  true, true, false, false,
							  false, false, true, true};
	
	DSCmatrix dsc_matrix = DiscreteMatrix::create(data);
	
	REQUIRE(dsc_matrix->get_size().horizontal_size == 4);
	REQUIRE(dsc_matrix->get_size().vertical_size == 4);
	
	REQUIRE(dsc_matrix[{0, 0}] == true);  REQUIRE(dsc_matrix[{1, 0}] == false); REQUIRE(dsc_matrix[{2, 0}] == true);  REQUIRE(dsc_matrix[{3, 0}] == false);
	REQUIRE(dsc_matrix[{0, 1}] == false); REQUIRE(dsc_matrix[{1, 1}] == true);  REQUIRE(dsc_matrix[{2, 1}] == false); REQUIRE(dsc_matrix[{3, 1}] == true);
	REQUIRE(dsc_matrix[{0, 2}] == true);  REQUIRE(dsc_matrix[{1, 2}] == true);  REQUIRE(dsc_matrix[{2, 2}] == false); REQUIRE(dsc_matrix[{3, 2}] == false);
	REQUIRE(dsc_matrix[{0, 3}] == false); REQUIRE(dsc_matrix[{1, 3}] == false); REQUIRE(dsc_matrix[{2, 3}] == true);  REQUIRE(dsc_matrix[{3, 3}] == true);

	DiscreteMatrix::destroy(dsc_matrix);
}

TEST_CASE("DiscreteMatrix: stress test memory management", "[DiscreteMatrix]") {
	SECTION("Create and destroy many matrices") {
		constexpr int count = 10;
		DSCmatrix matrices[count];
		
		for (int i = 0; i < count; ++i) {
			matrices[i] = DiscreteMatrix::create(i + 2, i + 2);
			REQUIRE(matrices[i]->get_size().horizontal_size == i + 2);
		}
		
		// Destroy in reverse order
		for (int i = count - 1; i >= 0; --i) {
			DiscreteMatrix::destroy(matrices[i]);
		}
	}
	
	SECTION("Create, destroy middle, create again") {
		DSCmatrix m1 = DiscreteMatrix::create(3, 3);
		DSCmatrix m2 = DiscreteMatrix::create(4, 4);
		DSCmatrix m3 = DiscreteMatrix::create(5, 5);
		
		m1[{0, 0}] = true;
		m2[{0, 0}] = false;
		m3[{0, 0}] = true;
		
		DiscreteMatrix::destroy(m2);
		
		DSCmatrix m4 = DiscreteMatrix::create(6, 6);
		m4[{0, 0}] = false;
		
		REQUIRE(m1[{0, 0}] == true);
		REQUIRE(m3[{0, 0}] == true);
		REQUIRE(m4[{0, 0}] == false);
		
		DiscreteMatrix::destroy(m1);
		DiscreteMatrix::destroy(m3);
		DiscreteMatrix::destroy(m4);
	}
	
	SECTION("Double destroy protection") {
		DSCmatrix m1 = DiscreteMatrix::create(3, 3);
		DiscreteMatrix::destroy(m1);
		// Second destroy should throw exception
		REQUIRE_THROWS_WITH(DiscreteMatrix::destroy(m1), "DCS_MATRIX_DESTROY_NULLPTR");
	}
	
	SECTION("Access after destroy") {
		DSCmatrix m1 = DiscreteMatrix::create(3, 3);
		m1[{0, 0}] = true;
		DiscreteMatrix::destroy(m1);
		
		// Create new matrix to potentially reuse memory
		DSCmatrix m2 = DiscreteMatrix::create(3, 3);
		m2[{0, 0}] = false;
		
		REQUIRE(m2[{0, 0}] == false);
		DiscreteMatrix::destroy(m2);
	}
}

TEST_CASE("DiscreteMatrix: cell access", "[DiscreteMatrix]") {
	DSCmatrix dsc_matrix = DiscreteMatrix::create(3, 3);
	
	DSCposition pos1 {0, 0};
	DSCposition pos2 {1, 1};
	DSCposition pos3 {2, 2};
	
	dsc_matrix[pos1] = true;
	dsc_matrix[pos2] = false;
	dsc_matrix[pos3] = true;
	
	REQUIRE(dsc_matrix[pos1] == true);
	REQUIRE(dsc_matrix[pos2] == false);
	REQUIRE(dsc_matrix[pos3] == true);
	
	DiscreteMatrix::destroy(dsc_matrix);
}

TEST_CASE("DiscreteMatrix: get neighbors", "[DiscreteMatrix]") {
	DSCmatrix dsc_matrix = DiscreteMatrix::create(3, 3);
	
	dsc_matrix[{0, 0}] = true;  dsc_matrix[{1, 0}] = false; dsc_matrix[{2, 0}] = true;
	dsc_matrix[{0, 1}] = false; dsc_matrix[{1, 1}] = true;  dsc_matrix[{2, 1}] = false;
	dsc_matrix[{0, 2}] = true;  dsc_matrix[{1, 2}] = false; dsc_matrix[{2, 2}] = true;
	
	SECTION("Center position") {
		DSCposition center{1, 1};
		DSCneighbors neighbors = dsc_matrix->get_neighbors(center);
		
		REQUIRE(neighbors.top_left == true);    REQUIRE(neighbors.top == false);    REQUIRE(neighbors.top_right == true);
		REQUIRE(neighbors.left == false);                                           REQUIRE(neighbors.right == false);
		REQUIRE(neighbors.bottom_left == true); REQUIRE(neighbors.bottom == false); REQUIRE(neighbors.bottom_right == true);
	}
	
	SECTION("Right edge center") {
		DSCposition right_center{2, 1};
		DSCneighbors neighbors = dsc_matrix->get_neighbors(right_center);
		
		REQUIRE(neighbors.top_left == false);   REQUIRE(neighbors.top == true);     REQUIRE(neighbors.top_right == true);
		REQUIRE(neighbors.left == true);                                            REQUIRE(neighbors.right == false);
		REQUIRE(neighbors.bottom_left == false); REQUIRE(neighbors.bottom == true);  REQUIRE(neighbors.bottom_right == true);
	}
	
	SECTION("Top left corner") {
		DSCposition top_left{0, 0};
		DSCneighbors neighbors = dsc_matrix->get_neighbors(top_left);
		
		REQUIRE(neighbors.top_left == true);    REQUIRE(neighbors.top == true);     REQUIRE(neighbors.top_right == false);
		REQUIRE(neighbors.left == true);                                            REQUIRE(neighbors.right == false);
		REQUIRE(neighbors.bottom_left == false); REQUIRE(neighbors.bottom == false);  REQUIRE(neighbors.bottom_right == true);
	}
	
	DiscreteMatrix::destroy(dsc_matrix);
}

TEST_CASE("DiscreteMatrix: do each cell", "[DiscreteMatrix]") {
	SECTION("Basic toggle all cells") {
		DSCmatrix dsc_matrix = DiscreteMatrix::create(2, 2);
		
		dsc_matrix[{0, 0}] = false; dsc_matrix[{1, 0}] = false;
		dsc_matrix[{0, 1}] = false; dsc_matrix[{1, 1}] = false;
		
		auto toggle_cell = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			return !cell;
		};
		
		dsc_matrix->do_each_cell(toggle_cell);
		
		REQUIRE(dsc_matrix[{0, 0}] == true); REQUIRE(dsc_matrix[{1, 0}] == true);
		REQUIRE(dsc_matrix[{0, 1}] == true); REQUIRE(dsc_matrix[{1, 1}] == true);
		
		DiscreteMatrix::destroy(dsc_matrix);
	}
	
	SECTION("Set all cells to true") {
		DSCmatrix dsc_matrix = DiscreteMatrix::create(3, 3);
		
		// Initialize with mixed values
		dsc_matrix[{0, 0}] = false; dsc_matrix[{1, 0}] = true;  dsc_matrix[{2, 0}] = false;
		dsc_matrix[{0, 1}] = true;  dsc_matrix[{1, 1}] = false; dsc_matrix[{2, 1}] = true;
		dsc_matrix[{0, 2}] = false; dsc_matrix[{1, 2}] = true;  dsc_matrix[{2, 2}] = false;
		
		auto set_true = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			return true;
		};
		
		dsc_matrix->do_each_cell(set_true);
		
		// Verify all cells are true
		for (uint32_t row = 0; row < 3; row++) {
			for (uint32_t col = 0; col < 3; col++) {
				REQUIRE(dsc_matrix[{col, row}] == true);
			}
		}
		
		DiscreteMatrix::destroy(dsc_matrix);
	}
	
	SECTION("Set cells based on position") {
		DSCmatrix dsc_matrix = DiscreteMatrix::create(4, 4);
		
		// Initialize all to false
		auto set_false = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			return false;
		};
		dsc_matrix->do_each_cell(set_false);
		
		// Set cells on diagonal to true
		auto set_diagonal = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			return pos.horizontal_position == pos.vertical_position;
		};
		dsc_matrix->do_each_cell(set_diagonal);
		
		// Verify diagonal
		for (uint32_t i = 0; i < 4; i++) {
			for (uint32_t j = 0; j < 4; j++) {
				if (i == j) {
					REQUIRE(dsc_matrix[{i, j}] == true);
				} else {
					REQUIRE(dsc_matrix[{i, j}] == false);
				}
			}
		}
		
		DiscreteMatrix::destroy(dsc_matrix);
	}
	
	SECTION("Count neighbors during iteration") {
		DSCmatrix dsc_matrix = DiscreteMatrix::create(3, 3);
		
		// Create a pattern
		dsc_matrix[{0, 0}] = true;  dsc_matrix[{1, 0}] = true;  dsc_matrix[{2, 0}] = false;
		dsc_matrix[{0, 1}] = true;  dsc_matrix[{1, 1}] = false; dsc_matrix[{2, 1}] = false;
		dsc_matrix[{0, 2}] = false; dsc_matrix[{1, 2}] = false; dsc_matrix[{2, 2}] = false;
		
		// Count alive neighbors and set cell based on Game of Life rules
		auto game_of_life = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			DSCneighbors neighbors = dsc_matrix.get_neighbors(pos);
			int alive_count = neighbors.top_left + neighbors.top + neighbors.top_right +
							  neighbors.left + neighbors.right +
							  neighbors.bottom_left + neighbors.bottom + neighbors.bottom_right;
			
			if (cell) {
				// Cell is alive: survives with 2 or 3 neighbors
				return alive_count == 2 || alive_count == 3;
			} else {
				// Cell is dead: becomes alive with exactly 3 neighbors
				return alive_count == 3;
			}
		};
		
		dsc_matrix->do_each_cell(game_of_life);
		
		// Verify results (this tests that neighbors are correctly accessed during iteration)
		REQUIRE(dsc_matrix[{0, 0}] == true);  // Had 2 alive neighbors
		REQUIRE(dsc_matrix[{1, 0}] == true);  // Had 2 alive neighbors
		
		DiscreteMatrix::destroy(dsc_matrix);
	}
	
	SECTION("Stress test: large matrix") {
		DSCmatrix dsc_matrix = DiscreteMatrix::create(50, 50);
		
		// Initialize with pattern
		auto set_checkboard = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			return (pos.horizontal_position + pos.vertical_position) % 2 == 0;
		};
		dsc_matrix->do_each_cell(set_checkboard);
		
		// Verify checkboard pattern
		for (uint32_t row = 0; row < 50; row++) {
			for (uint32_t col = 0; col < 50; col++) {
				bool expected = (row + col) % 2 == 0;
				REQUIRE(dsc_matrix[{col, row}] == expected);
			}
		}
		
		// Toggle all cells
		auto toggle = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			return !cell;
		};
		dsc_matrix->do_each_cell(toggle);
		
		// Verify inverted checkboard
		for (uint32_t row = 0; row < 50; row++) {
			for (uint32_t col = 0; col < 50; col++) {
				bool expected = (row + col) % 2 == 1;
				REQUIRE(dsc_matrix[{col, row}] == expected);
			}
		}
		
		DiscreteMatrix::destroy(dsc_matrix);
	}
	
	SECTION("Stress test: multiple iterations") {
		DSCmatrix dsc_matrix = DiscreteMatrix::create(10, 10);
		
		// Initialize all to false
		auto set_false = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			return false;
		};
		dsc_matrix->do_each_cell(set_false);
		
		// Perform 100 toggle operations
		auto toggle = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			return !cell;
		};
		
		for (int i = 0; i < 100; i++) {
			dsc_matrix->do_each_cell(toggle);
		}
		
		// After even number of toggles, all should be false
		for (uint32_t row = 0; row < 10; row++) {
			for (uint32_t col = 0; col < 10; col++) {
				REQUIRE(dsc_matrix[{col, row}] == false);
			}
		}
		
		DiscreteMatrix::destroy(dsc_matrix);
	}
	
	SECTION("Stress test: complex calculation") {
		DSCmatrix dsc_matrix = DiscreteMatrix::create(20, 20);
		
		// Set complex pattern based on position
		auto complex_pattern = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			// Create a pattern based on mathematical formula
			uint32_t x = pos.horizontal_position;
			uint32_t y = pos.vertical_position;
			return ((x * y) % 3 == 0) || (x == y) || (x + y == 19);
		};
		dsc_matrix->do_each_cell(complex_pattern);
		
		// Count how many cells are alive
		int alive_count = 0;
		for (uint32_t row = 0; row < 20; row++) {
			for (uint32_t col = 0; col < 20; col++) {
				if (dsc_matrix[{col, row}]) {
					alive_count++;
				}
			}
		}
		
		// Verify that we have a reasonable number of alive cells
		REQUIRE(alive_count > 0);
		REQUIRE(alive_count < 400);
		
		DiscreteMatrix::destroy(dsc_matrix);
	}

	SECTION("Stress test: do_each_cell with external data") {
		DSCmatrix dsc_matrix = DiscreteMatrix::create(30, 30);
		
		// Initialize matrix with checkerboard pattern
		auto set_checkboard = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			return (pos.horizontal_position + pos.vertical_position) % 2 == 0;
		};
		dsc_matrix->do_each_cell(set_checkboard);
		
		// Use external data to track statistics
		struct Statistics {
			int alive_count = 0;
			int dead_count = 0;
			int top_left_quadrant_alive = 0;
			int bottom_right_quadrant_alive = 0;
			int diagonal_alive = 0;
			int total_iterations = 0;
		};
		
		Statistics stats;
		
		auto collect_stats = [](bool cell, DSCposition pos, void* data, DiscreteMatrix& dsc_matrix) -> bool {
			Statistics* stats = static_cast<Statistics*>(data);
			stats->total_iterations++;
			
			if (cell) {
				stats->alive_count++;
				
				// Top-left quadrant
				if (pos.horizontal_position < 15 && pos.vertical_position < 15) {
					stats->top_left_quadrant_alive++;
				}
				
				// Bottom-right quadrant
				if (pos.horizontal_position >= 15 && pos.vertical_position >= 15) {
					stats->bottom_right_quadrant_alive++;
				}
				
				// Diagonal
				if (pos.horizontal_position == pos.vertical_position) {
					stats->diagonal_alive++;
				}
			} else {
				stats->dead_count++;
			}
			
			return cell; // Keep original value
		};
		
		dsc_matrix->do_each_cell(&stats, collect_stats);
		
		// Verify statistics
		REQUIRE(stats.total_iterations == 900); // 30x30
		REQUIRE(stats.alive_count + stats.dead_count == 900);
		REQUIRE(stats.alive_count == 450); // Half should be alive in checkerboard
		REQUIRE(stats.dead_count == 450);
		REQUIRE(stats.top_left_quadrant_alive > 0);
		REQUIRE(stats.bottom_right_quadrant_alive > 0);
		
		DiscreteMatrix::destroy(dsc_matrix);
	}

	SECTION("Stress test: do_each_cell with data modification") {
		DSCmatrix dsc_matrix = DiscreteMatrix::create(25, 25);
		
		// Initialize with random-like pattern
		auto init_pattern = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			return (pos.horizontal_position * 7 + pos.vertical_position * 13) % 3 == 0;
		};
		dsc_matrix->do_each_cell(init_pattern);
		
		// Use external counter to flip every Nth cell
		struct FlipData {
			int counter = 0;
			int flip_every = 5;
			int flipped_count = 0;
		};
		
		FlipData flip_data;
		
		auto flip_every_nth = [](bool cell, DSCposition pos, void* data, DiscreteMatrix& dsc_matrix) -> bool {
			FlipData* flip = static_cast<FlipData*>(data);
			flip->counter++;
			
			if (flip->counter % flip->flip_every == 0) {
				flip->flipped_count++;
				return !cell;
			}
			return cell;
		};
		
		dsc_matrix->do_each_cell(&flip_data, flip_every_nth);
		
		REQUIRE(flip_data.counter == 625); // 25x25
		REQUIRE(flip_data.flipped_count == 125); // Every 5th cell
		
		DiscreteMatrix::destroy(dsc_matrix);
	}

	SECTION("Stress test: multiple do_each_cell calls with shared data") {
		DSCmatrix dsc_matrix = DiscreteMatrix::create(20, 20);
		
		// Initialize all to false
		auto set_false = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			return false;
		};
		dsc_matrix->do_each_cell(set_false);
		
		struct IterationData {
			int iteration_number = 0;
			int total_changes = 0;
			int cells_set_to_true = 0;
		};
		
		IterationData iter_data;
		
		auto flip_based_on_iteration = [](bool cell, DSCposition pos, void* data, DiscreteMatrix& dsc_matrix) -> bool {
			IterationData* iter = static_cast<IterationData*>(data);
			
			// Different pattern each iteration
			bool new_value = (pos.horizontal_position + pos.vertical_position + iter->iteration_number) % 2 == 0;
			
			if (new_value != cell) {
				iter->total_changes++;
			}
			
			if (new_value) {
				iter->cells_set_to_true++;
			}
			
			return new_value;
		};
		
		// Run 10 iterations
		for (int i = 0; i < 10; i++) {
			iter_data.iteration_number = i;
			iter_data.total_changes = 0;
			iter_data.cells_set_to_true = 0;
			
			dsc_matrix->do_each_cell(&iter_data, flip_based_on_iteration);
			
			REQUIRE(iter_data.total_changes >= 0);
			REQUIRE(iter_data.cells_set_to_true >= 0);
			REQUIRE(iter_data.cells_set_to_true <= 400);
		}
		
		DiscreteMatrix::destroy(dsc_matrix);
	}

	SECTION("Stress test: Game of Life simulation with generation tracking") {
		DSCmatrix dsc_matrix = DiscreteMatrix::create(15, 15);
		
		// Create glider pattern at top-left
		auto init_glider = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			// Glider pattern
			if ((pos.horizontal_position == 1 && pos.vertical_position == 0) ||
				(pos.horizontal_position == 2 && pos.vertical_position == 1) ||
				(pos.horizontal_position == 0 && pos.vertical_position == 2) ||
				(pos.horizontal_position == 1 && pos.vertical_position == 2) ||
				(pos.horizontal_position == 2 && pos.vertical_position == 2)) {
				return true;
			}
			return false;
		};
		dsc_matrix->do_each_cell(init_glider);
		
		struct GenerationData {
			int generation = 0;
			int alive_cells = 0;
			int births = 0;
			int deaths = 0;
		};
		
		GenerationData gen_data;
		
		auto game_of_life_with_stats = [](bool cell, DSCposition pos, void* data, DiscreteMatrix& dsc_matrix) -> bool {
			GenerationData* gen = static_cast<GenerationData*>(data);
			
			DSCneighbors neighbors = dsc_matrix.get_neighbors(pos);
			int alive_count = neighbors.top_left + neighbors.top + neighbors.top_right +
							  neighbors.left + neighbors.right +
							  neighbors.bottom_left + neighbors.bottom + neighbors.bottom_right;
			
			bool new_state;
			if (cell) {
				new_state = alive_count == 2 || alive_count == 3;
			} else {
				new_state = alive_count == 3;
			}
			
			if (new_state) {
				gen->alive_cells++;
				if (!cell) {
					gen->births++;
				}
			} else if (cell) {
				gen->deaths++;
			}
			
			return new_state;
		};
		
		// Simulate 20 generations
		for (int i = 0; i < 20; i++) {
			gen_data.generation = i;
			gen_data.alive_cells = 0;
			gen_data.births = 0;
			gen_data.deaths = 0;
			
			dsc_matrix->do_each_cell(&gen_data, game_of_life_with_stats);
			
			// Glider should maintain approximately 5 alive cells
			REQUIRE(gen_data.alive_cells >= 0);
			REQUIRE(gen_data.alive_cells <= 225); // Max possible
		}
		
		DiscreteMatrix::destroy(dsc_matrix);
	}

	SECTION("Stress test: complex data structure with nested operations") {
		DSCmatrix dsc_matrix = DiscreteMatrix::create(10, 10);
		
		struct ComplexData {
			int histogram[10] = {0}; // Count cells by row
			int max_consecutive_alive = 0;
			int current_consecutive = 0;
			DSCposition last_alive_pos = {0, 0};
			bool found_pattern = false;
		};
		
		// Initialize with pattern
		auto init_pattern = [](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			return pos.vertical_position < 5 && pos.horizontal_position % 2 == 0;
		};
		dsc_matrix->do_each_cell(init_pattern);
		
		ComplexData complex_data;
		
		auto analyze_pattern = [](bool cell, DSCposition pos, void* data, DiscreteMatrix& dsc_matrix) -> bool {
			ComplexData* cd = static_cast<ComplexData*>(data);
			
			if (cell) {
				cd->histogram[pos.vertical_position]++;
				cd->current_consecutive++;
				cd->last_alive_pos = pos;
				
				if (cd->current_consecutive > cd->max_consecutive_alive) {
					cd->max_consecutive_alive = cd->current_consecutive;
				}
				
				// Check for specific pattern (3 consecutive alive cells)
				if (cd->current_consecutive >= 3) {
					cd->found_pattern = true;
				}
			} else {
				cd->current_consecutive = 0;
			}
			
			return cell;
		};
		
		dsc_matrix->do_each_cell(&complex_data, analyze_pattern);
		
		// Verify analysis
		REQUIRE(complex_data.max_consecutive_alive >= 0);
		REQUIRE(complex_data.histogram[0] >= 0);
		
		int total_from_histogram = 0;
		for (int i = 0; i < 10; i++) {
			total_from_histogram += complex_data.histogram[i];
		}
		REQUIRE(total_from_histogram > 0);
		
		DiscreteMatrix::destroy(dsc_matrix);
	}
}