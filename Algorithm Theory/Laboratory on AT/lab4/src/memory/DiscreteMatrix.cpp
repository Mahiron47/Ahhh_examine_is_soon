#include "DiscreteMatrix.hpp"
#include <cmath>

uint32_t DiscreteMatrix::memory_size = 0;
DSCmatrix* DiscreteMatrix::memory = nullptr;

DiscreteMatrix::DiscreteMatrix(uint32_t horizontal_size, uint32_t vertical_size) :
	horizonstal_pointer(horizontal_size),
	vertical_pointer(vertical_size) {
	data = new bool*[vertical_size];
	
	do { data[(uint32_t) vertical_pointer] = new bool[horizontal_size];
	} while (++vertical_pointer);
}

DiscreteMatrix::DiscreteMatrix(std::initializer_list<bool> condition) :
	horizonstal_pointer((uint32_t) std::floor(std::sqrt(condition.size()))),
	vertical_pointer((uint32_t) std::floor(std::sqrt(condition.size()))) {
	data = new bool*[(uint32_t) std::floor(std::sqrt(condition.size()))];
	
	do { data[(uint32_t) vertical_pointer] = new bool[(uint32_t) std::floor(std::sqrt(condition.size()))];
	} while (++vertical_pointer);
}

DiscreteMatrix::DiscreteMatrix(const std::vector<bool>& condition) :
	horizonstal_pointer((uint32_t) std::floor(std::sqrt(condition.size()))),
	vertical_pointer((uint32_t) std::floor(std::sqrt(condition.size()))) {
	uint32_t size = (uint32_t) std::floor(std::sqrt(condition.size()));
	data = new bool*[size];
	
	do { data[(uint32_t) vertical_pointer] = new bool[size];
	} while (++vertical_pointer);
	
	uint32_t index = 0;
	do { do {
			if (index < condition.size()) {
				data[(uint32_t)vertical_pointer][(uint32_t)horizonstal_pointer] = condition[index++];
			}
		} while (++horizonstal_pointer);
	} while (++vertical_pointer);
}

DiscreteMatrix::~DiscreteMatrix() {
	do { delete[] data[(uint32_t) vertical_pointer];
	} while (--vertical_pointer);
	delete[] data;
}

DSCmatrix DiscreteMatrix::create(uint32_t horizontal_size, uint32_t vertical_size) {
    if (memory != nullptr) {
        memory = new DSCmatrix[++memory_size];
        memory[memory_size - 1] = new DiscreteMatrix(horizontal_size, vertical_size);
        return memory[memory_size - 1];
    }
    DSCmatrix* buffer = new DSCmatrix[memory_size + 1];
    for (uint32_t i = 0; i < memory_size; i++) {
        buffer[i] = memory[i];
    }
    memory = new DSCmatrix[memory_size + 1];
    buffer[memory_size] = new DiscreteMatrix(horizontal_size, vertical_size);
    memory_size++;
    return buffer[memory_size - 1];
}

DSCmatrix DiscreteMatrix::create(std::initializer_list<bool> condition) {
	if (memory != nullptr) {
		memory = new DSCmatrix[++memory_size];
		memory[memory_size - 1] = new DiscreteMatrix(condition);
		return memory[memory_size - 1];
	}
	DSCmatrix* buffer = new DSCmatrix[memory_size + 1];
	for (uint32_t i = 0; i < memory_size; i++) {
		buffer[i] = memory[i];
	}
	memory = new DSCmatrix[memory_size + 1];
	buffer[memory_size] = new DiscreteMatrix(condition);
	memory_size++;
	return buffer[memory_size - 1];
}

DSCmatrix DiscreteMatrix::create(const std::vector<bool>& condition) {
	if (memory != nullptr) {
		memory = new DSCmatrix[++memory_size];
		memory[memory_size - 1] = new DiscreteMatrix(condition);
		return memory[memory_size - 1];
	}
	DSCmatrix* buffer = new DSCmatrix[memory_size + 1];
	for (uint32_t i = 0; i < memory_size; i++) {
		buffer[i] = memory[i];
	}
	memory = new DSCmatrix[memory_size + 1];
	buffer[memory_size] = new DiscreteMatrix(condition);
	memory_size++;
	return buffer[memory_size - 1];
}

void DiscreteMatrix::destroy(DSCmatrix& dsc_matrix) {
	if (dsc_matrix != nullptr) {
		delete (DiscreteMatrix*)dsc_matrix;
		dsc_matrix = nullptr;
	} else {
		throw "DCS_MATRIX_DESTROY_NULLPTR";
	}
}

DSCneighbors DiscreteMatrix::get_neighbors(DSCposition pos) {
	return DSCneighbors {
		.top_left = data[(pos.vertical_position == 0 ? vertical_pointer.size() - 1 : pos.vertical_position - 1)]
			[(pos.horizontal_position == 0 ? horizonstal_pointer.size() - 1 : pos.horizontal_position - 1)],
		.top = data[(pos.vertical_position == 0 ? vertical_pointer.size() - 1 : pos.vertical_position - 1)]
			[pos.horizontal_position],
		.top_right = data[(pos.vertical_position == 0 ? vertical_pointer.size() - 1 : pos.vertical_position - 1)]
			[(pos.horizontal_position + 1) % horizonstal_pointer.size()],
		.left = data[pos.vertical_position]
			[(pos.horizontal_position == 0 ? horizonstal_pointer.size() - 1 : pos.horizontal_position - 1)],
		.right = data[pos.vertical_position]
			[(pos.horizontal_position + 1) % horizonstal_pointer.size()],
		.bottom_left = data[(pos.vertical_position + 1) % vertical_pointer.size()]
			[(pos.horizontal_position == 0 ? horizonstal_pointer.size() - 1 : pos.horizontal_position - 1)],
		.bottom = data[(pos.vertical_position + 1) % vertical_pointer.size()]
			[pos.horizontal_position],
		.bottom_right = data[(pos.vertical_position + 1) % vertical_pointer.size()]
			[(pos.horizontal_position + 1) % horizonstal_pointer.size()]
	};
}

void DiscreteMatrix::do_each_cell(bool(*func)(bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix)) {
    do { do { data[(uint32_t)vertical_pointer][(uint32_t)horizonstal_pointer] =
				func(data[(uint32_t)vertical_pointer][(uint32_t)horizonstal_pointer], 
					 DSCposition {(uint32_t)horizonstal_pointer, (uint32_t)vertical_pointer}, 
					 *this);
		} while (++horizonstal_pointer);
	} while (++vertical_pointer);
}

void DiscreteMatrix::do_each_cell(void* data, bool(*func)(bool cell, DSCposition pos, void* data, DiscreteMatrix& dsc_matrix)) {
	bool** new_states = new bool*[vertical_pointer.size()];
	do { new_states[(uint32_t) vertical_pointer] = new bool[horizonstal_pointer.size()];
	} while (++vertical_pointer);
	
	do { do {
			DSCposition pos = {(uint32_t)horizonstal_pointer, (uint32_t)vertical_pointer};
			bool current_cell = this->data[(uint32_t)vertical_pointer][(uint32_t)horizonstal_pointer];
			bool new_cell = func(current_cell, pos, data, *this);
			new_states[(uint32_t)vertical_pointer][(uint32_t)horizonstal_pointer] = new_cell;
		} while (++horizonstal_pointer);
	} while (++vertical_pointer);
	
	do { do {
			this->data[(uint32_t)vertical_pointer][(uint32_t)horizonstal_pointer] = 
				new_states[(uint32_t)vertical_pointer][(uint32_t)horizonstal_pointer];
		} while (++horizonstal_pointer);
	} while (++vertical_pointer);
	
	do {  delete[] new_states[(uint32_t) vertical_pointer];
	} while (--vertical_pointer);
	delete[] new_states;
}

bool& DiscreteMatrix::operator[](const DSCposition& pos) {
    return data[pos.vertical_position][pos.horizontal_position];
}

DSCmatrix::DSCmatrix(DiscreteMatrix* p) : ptr(p), dead_cells_count(0) {
	if (ptr != nullptr) {
		ptr->do_each_cell(&dead_cells_count, [](bool cell, DSCposition pos, void* data, DiscreteMatrix& dsc_matrix) -> bool {
			if (!cell) {
				(*static_cast<uint64_t*>(data))++;
			}
			return cell;
		});
	}
}

bool& DSCmatrix::operator[](const DSCposition& pos) const {
    return (*ptr)[pos];
}

DSCmatrix::operator string() const {
	if (ptr == nullptr) {
		return "<html><body><h3>Matrix is null</h3></body></html>";
	}
	
	DSCsize size = ptr->get_size();

	uint64_t dead_cells_count_iteration = 0;

	ptr->do_each_cell(&dead_cells_count_iteration, [](bool cell, DSCposition pos, void* data, DiscreteMatrix& dsc_matrix) -> bool {
		if (!cell) {
			(*static_cast<uint64_t*>(data))++;
		}
		return cell;
	});

	int64_t delta_dead_cells_count = dead_cells_count_iteration - dead_cells_count;
	const_cast<DSCmatrix*>(this)->dead_cells_count = dead_cells_count_iteration;
	
	string html = "<!DOCTYPE html>\n<html>\n<head>\n";
	html += "<style>\n";
	html += "table { border-collapse: collapse; margin: 20px; }\n";
	html += "td { width: 20px; height: 20px; border: 1px solid #ccc; text-align: center; }\n";
	html += ".alive { background-color: #ff0000; color: #fff; }\n";
	html += ".dead { background-color: #fff; color: #000; }\n";
	html += "</style>\n";
	html += "</head>\n<body>\n";
	
	html += "<h2>Game of Life - Matrix State</h2>\n";
	html += "<p><strong>Dead cells this step:</strong> " + std::to_string(delta_dead_cells_count) + "</p>\n";
	html += "<p><strong>Matrix size:</strong> " + std::to_string(size.horizontal_size) + " x " + std::to_string(size.vertical_size) + "</p>\n";
	
	uint32_t alive_count = 0;
	for (uint32_t row = 0; row < size.vertical_size; row++) {
		for (uint32_t col = 0; col < size.horizontal_size; col++) {
			if ((*ptr)[DSCposition{col, row}]) {
				alive_count++;
			}
		}
	}
	html += "<p><strong>Alive cells:</strong> " + std::to_string(alive_count) + "</p>\n";
	
	html += "<table>\n";
	for (uint32_t row = 0; row < size.vertical_size; row++) {
		html += "  <tr>\n";
		for (uint32_t col = 0; col < size.horizontal_size; col++) {
			bool cell = (*ptr)[DSCposition{col, row}];
			html += "    <td class=\"" + string(cell ? "alive" : "dead") + "\">";
			html += cell ? "&#9632;" : "&#9633;"; // ■ for alive, □ for dead
			html += "</td>\n";
		}
		html += "  </tr>\n";
	}
	html += "</table>\n";
	
	html += "</body>\n</html>";
	return html;
}
