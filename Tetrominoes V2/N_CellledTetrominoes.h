#pragma once
#include "KDContainerTree.h"
#include "Tetromino.h"
#include <vector>
#include <string>
#include "VectorMethods.h"
using std::vector;
using std::string;

// class that handles the logic of creating all the tetrominoes of size N
class N_CellledTetrominoes
{
private:
	int level;
	KDContainerTree<Tetromino, int> Tetrominoes;

public:
	N_CellledTetrominoes(int level) {
		this->level = level;
		Tetrominoes = KDContainerTree<Tetromino, int>(2).
			balancedPopulation(possibleDimensions(level));
	}

	void addTetromino(Tetromino T) {
		Tetrominoes.insertItem(T, T.getDimensions());
	}

	void addTetromino(vector<vector<int>> matrix, int n) {
		Tetromino T(matrix, n);
		Tetrominoes.insertItem(T, T.getDimensions());
	}

	bool search(Tetromino T) {
		auto temp = Tetrominoes.getItemsWithValue(T.getDimensions());
		return getPos(temp, T) != -1;
	}

	bool searchMatrix(vector<vector<int>> matrix) {
		int height = matrix.size();
		int width = matrix[0].size();

		auto temp = Tetrominoes.getItemsWithValue(
			vector<int>{height, width});
	
		for (auto i : temp) {
			if (i.isSameTetrominoe(matrix)) {
				return true;
			}
		}

		if (width == height) {
			return false;
		}

		temp = Tetrominoes.getItemsWithValue(
			vector<int>{width, height});

		for (auto i : temp) {
			if (i.isSameTetrominoe(matrix)) {
				return true;
			}
		}

		return false;
	}

	N_CellledTetrominoes nextLevel() {
		auto iter = Tetrominoes.iterator();
		N_CellledTetrominoes next(level + 1);

		while (iter.hasNext()) {
			auto temp = iter.next();
			for (auto temp2 : temp) {
				for (auto i : temp2.generateChildren()) {

					if (!next.searchMatrix(i)) {
						next.addTetromino(i, level + 1);
					}
				}
			}
		}

		return next;
	}

	int amount() { return Tetrominoes.getAmount(); }

	int getLevel() { return level; }

	string toDot() { return Tetrominoes.toDot(); }


private:
	static vector<vector<int>> possibleDimensions(int n) {
		vector<vector<int>> temp;

		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				if ((i * j) >= n and (i + j - 1) <= n) {
					temp.push_back(vector<int>{i, j});
				}
			}
		}

		return temp;
	}

};

