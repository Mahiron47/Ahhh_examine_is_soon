#include <iostream>
#include <cstdint>
#include <thread>
#include <future>

std::mutex grid_mutex;
uint64_t counter = 0;

void print_grid(uint32_t* grid, size_t width, size_t height) {
	uint8_t max_exp = [&grid, width, height]() -> uint8_t {
		uint8_t max_exp = 0;
		uint32_t max_val = 0;
		for (size_t i = 0; i < width * height; i++) {
			max_val = [](uint32_t a, uint32_t b) -> uint32_t {
				if (a < b) {
					return b;
				} else {
					return a;
				}
			}(max_val, grid[i]);
		}
		do {
			max_exp++;
			max_val /= 10;
		} while (max_val != 0);
		return max_exp;
	}();
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			char* r;
			std::cout << grid[y * width + x] << [&grid, &r, width, max_exp](size_t x, size_t y) -> char* { 
				uint8_t size = 0;
				uint32_t temp = grid[y * width + x];
				do {
					size++;
					temp /= 10;
				} while (temp != 0);
				size = max_exp - size;
				r = new char[size + 1];
				r[size] = '\0';
				while (size != 0) {
					size--;
					r[size] = ' ';
				}
				return r;
			}(x, y) << " ";
			delete[] r;
		}
		std::cout << std::endl;
	}
}

void wave(uint32_t* grid, size_t x, size_t y, size_t width, uint32_t wave_num = 0) {
    {
        std::lock_guard<std::mutex> lock(grid_mutex);

        if (grid[x + y * width] == 0 || grid[x + y * width] <= wave_num) {
            return;
        }

        std::cout << ++counter << ": " << grid[x + y * width] << " wave: " << 
            wave_num << std::endl;
        grid[x + y * width] = wave_num;
    }
    
    std::thread right([&grid, x, y, width, wave_num]() -> void {
        wave(grid, x + 1, y, width, wave_num + 1);
    });
    std::thread down([&grid, x, y, width, wave_num]() -> void {
        wave(grid, x, y + 1, width, wave_num + 1);
    });
    std::thread up([&grid, x, y, width, wave_num]() -> void {
        wave(grid, x, y - 1, width, wave_num + 1);
    });
    std::thread left([&grid, x, y, width, wave_num]() -> void {
        wave(grid, x - 1, y, width, wave_num + 1);
    });

    right.join();
    down.join();
    up.join();
    left.join();
}

int main() {
	uint32_t height = 10;
	uint32_t width = 10;

	uint32_t* grid = new uint32_t[height * width] {
		0, 0,    0,    0,    0,    0,    0,    0,    0,    0,
		0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 0,
		0, 9000, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 0,
		0, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 0,
		0, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 0,
		0, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000, 0,
		0, 4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800, 0,
		0, 5100, 5200, 5300, 5400, 5500, 5600, 5700, 5800, 0,
		0, 6100, 6200, 6300, 6400, 6500, 6600, 6700, 6800, 0,
		0, 0,    0,    0,    0,    0,    0,    0,    0,    0
	};

	print_grid(grid, width, height);

	wave(grid, 2, 2, width);

	print_grid(grid, width, height);

	delete[] grid;

	return 0;
}