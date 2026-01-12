#ifndef DISCRETEMATRIX_HPP
#define DISCRETEMATRIX_HPP
#include "Pointer.hpp"

#include <string>
#include <cstdint>
#include <vector>
#include <initializer_list>

using std::string;

class DiscreteMatrix;

class DSCmatrix {
	DiscreteMatrix* ptr;

public:
	DSCmatrix(DiscreteMatrix* p = nullptr);

	DiscreteMatrix* operator->() const { return ptr; }
	DiscreteMatrix& operator*() const { return *ptr; }
	DiscreteMatrix& operator=(DiscreteMatrix* p) {
		ptr = p;
		return *ptr;
	}

	bool& operator[](const struct DSCposition& pos) const;
	
	operator DiscreteMatrix*() const { return ptr; }
	operator bool() const { return ptr != nullptr; }
	operator string() const;
};

struct DSCsize {
	uint32_t horizontal_size;
	uint32_t vertical_size;
};

struct DSCposition {
	// From 0 to size - 1
	uint32_t horizontal_position;
	// From 0 to size - 1
	uint32_t vertical_position;
};

struct DSCneighbors { 
	bool top_left;    bool top;    bool top_right;
	bool left; 				 	   bool right; 
	bool bottom_left; bool bottom; bool bottom_right;
};

class DiscreteMatrix {
	static uint32_t memory_size;
	static DSCmatrix* memory;

	Pointer horizonstal_pointer;
	Pointer vertical_pointer;
	bool** data;
	
	DiscreteMatrix(uint32_t horizontal_size, uint32_t vertical_size);
	DiscreteMatrix(std::initializer_list<bool> condition);
	DiscreteMatrix(const std::vector<bool>& condition);
	~DiscreteMatrix();
	
public:
	uint64_t dead_cells_count = 0;
	DSCsize get_size() const {
		return DSCsize {horizonstal_pointer.size(), vertical_pointer.size()};
	}

	static DSCmatrix create(uint32_t horizontal_size, uint32_t vertical_size);
	static DSCmatrix create(std::initializer_list<bool> condition);
	static DSCmatrix create(const std::vector<bool>& condition);
	static void destroy(DSCmatrix& dsc_matrix);


	DSCneighbors get_neighbors(DSCposition pos);

	void do_each_cell(bool(*func)(bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix));
	void do_each_cell(void* data, bool(*func)(bool cell, DSCposition pos, void* data, DiscreteMatrix& dsc_matrix));

	bool& operator[](const DSCposition& pos);
};

#endif // DISCRETEMATRIX_HPP