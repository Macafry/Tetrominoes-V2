#pragma once
#include <vector>
#include <string>
#include "VectorMethods.h"
#include "MatrixMethods.h"

using std::vector;
using std::string;

class Tetromino
{
private:
	static const int pos = 2;
	vector<vector<vector<int>>> shapes, flipshapes;
	bool has_mirror;
	string id;
	int size, width, height, variations;
	vector<string> parent_ids, child_ids;
	vector<vector<int>> possible_children;

public:
	Tetromino(vector<vector<int>> base, int level) {
		shapes.push_back(base);
		height = base.size();
		width = base[0].size();
		size = level;
		this->doRotations();
		this->generatePossibleChildren();
	}

	void doRotations() {
		vector<vector<int>> aux;
		for (int i = 0; i < 3; i++) {
			aux = rotate90CW(shapes.back());
			if (getPos(shapes, aux) == -1) {
				shapes.push_back(aux);
			}
		}

		for (auto tet : shapes) {
			aux = flipHor(tet);
			if (getPos(shapes, aux) == -1) {
				flipshapes.push_back(aux);
			}

		}

		has_mirror = flipshapes.size() != 0;
		variations = flipshapes.size() + shapes.size();
	}

	bool isSameTetrominoe(vector<vector<int>> challenger) {
		return has_mirror ?
			getPos(shapes, challenger) != -1 or
			getPos(flipshapes, challenger) != -1 :
			getPos(shapes, challenger) != -1;
	}

	// attempt 1 at generating possible childs, failed for unkown reason
	/*void notgeneratePossibleChilds() {



		//generate larger canvas
		vector<int> aux_line;
		for (int i = 0; i < shapes[0][0].size()+2; i++){
			aux_line.push_back(0);
		}
		possible_childs.push_back(aux_line);

		for (int i = 0; i < shapes[0].size(); i++) {
			aux_line.clear();
			aux_line.push_back(0);
			for (int j = 0; j < shapes[0][0].size(); j++){
				aux_line.push_back(shapes[0][i][j]);
			}
			aux_line.push_back(0);
			possible_childs.push_back(aux_line);
		}

		aux_line.clear();
		for (int i = 0; i < shapes[0][0].size() + 2; i++) {
			aux_line.push_back(0);
		}
		possible_childs.push_back(aux_line);


		bool condition;

		//top
		for (int i = 1; i < possible_childs[0].size() - 1; i++) {
			condition = possible_childs[1][i] == 1;
			if (condition) { possible_childs[0][i] == -1; }

		}

		//bottom
		for (int i = 1; i < possible_childs.size() - 1; i++) {
			condition = possible_childs[i][1] == 1;
			if (condition) { possible_childs[i][0] == -1; }

		}





	}*/

	void generatePossibleChildren() {
		vector<vector<int>> aux(height + 2, vector<int>(width + 2, 0));

		//populate a larger "canvas"
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				//aux[i + 1][j + 1] = shapes[0][i][j]
				aux[static_cast<std::vector<std::vector<int, std::allocator<int>>, std::allocator<std::vector<int, std::allocator<int>>>>::size_type>(i) + 1]
					[static_cast<std::vector<int, std::allocator<int>>::size_type>(j) + 1] = shapes[0][i][j];
			}
		}

		//mark locations for possible extra cell
		for (int i = 0; i < height + 2; i++) {
			for (int j = 0; j < width + 2; j++) {
				aux[i][j] = aux[i][j] == 1 ? 1 :
					(isSurroundend(aux, i, j) ? 2 : 0);
			}
		}

		possible_children = aux;
	}

	vector<vector<int>> cleanUp(vector<vector<int>> cleanee, int a, int b) {
		for (size_t i = 0; i < cleanee.size(); i++) {
			for (size_t j = 0; j < cleanee[0].size(); j++) {
				if ((a != i or b != j) and cleanee[i][j] == pos) {
					cleanee[i][j] = 0;
				}
			}
		}

		cleanee[a][b] = 1;

		if (b != cleanee[0].size() - 1) {
			removeColumn(&cleanee, cleanee[0].size() - 1);
		}

		if (b != 0) {
			removeColumn(&cleanee, 0);
		}

		if (a != cleanee.size() - 1) {
			cleanee.pop_back();
		}

		if (a != 0) {
			cleanee.erase(cleanee.begin());
		}



		return cleanee;
	}

	vector<vector<vector<int>>> generateChildren() {
		vector<vector<vector<int>>> children;

		for (int i = 0; i < possible_children.size(); i++) {
			for (int j = 0; j < possible_children[0].size(); j++) {
				if (possible_children[i][j] == pos) {
					children.push_back(cleanUp(possible_children, i, j));
				}
			}
		}

		return children;
	}

	void insertParent(string id) {
		parent_ids.push_back(id);
	}

	void insertChild(string id) {
		child_ids.push_back(id);
	}


	//necessary getters and setters
	bool getHasMirror() { return has_mirror; }

	vector<int> getDimensions() { return vector<int>{height, width}; }

	void setID(string s) { id = s; }

	string getID() { return id; }

	string getAllRotations() {
		string rots = "";
		for (int i = 0; i < shapes.size(); i++) {
			rots += matrixToString(shapes[i]) + "\n";
		}
		if (has_mirror) {
			for (int i = 0; i < flipshapes.size(); i++) {
				rots += matrixToString(flipshapes[i]) + "\n";
			}
		}
		return rots;
	}

	int getHeight() { return height; }
	int getWidth() { return width; }

	string getLargerCanvas() { return matrixToString(possible_children); }
	string getAllChildren() {
		string family = "";
		vector<vector<vector<int>>> children = this->generateChildren();
		for (int i = 0; i < children.size(); i++) {
			family += matrixToString(children[i]);
		}
		return family;
	}
	string getMainShape() { return matrixToString(shapes[0]); }
	vector<vector<int>> getMainShapeV() { return shapes[0]; }
	vector<vector<int>> getMirrorShapeV() { return flipshapes[0]; }

	bool operator == (Tetromino Tet) {
		return this->isSameTetrominoe(Tet.getMainShapeV());
	}

	string generateMatrixImage() {
		int padding = 1, border = 1, square_length = 6, canvas_height, canvas_width,
			tetromino_height, tetromino_width, height_offset, width_offset;
		tetromino_height = height * (border + square_length) + border;
		tetromino_width = width * (border + square_length) + border;
		vector<vector<int>> cell(square_length + 2 * border,
			vector<int>(3 * (square_length + 2 * border), 50));

		for (int i = border; i < square_length + border; i++) {
			for (int j = 3 * border; j < 3 * (square_length + border); j += 3) {
				cell[i][j] = 20;
				cell[i][j + 1] = 50;
				cell[i][j + 2] = 200;

			}
		}

		canvas_height = tetromino_height + 2 * padding;
		if (has_mirror) {
			canvas_width = 3 * (2 * tetromino_width + 3 * padding);
		}
		else {
			canvas_width = 3 * (tetromino_width + 2 * padding);
		}

		vector<vector<int>> canvas(canvas_height,
			vector<int>(canvas_width, 255));

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (shapes[0][i][j]) {
					height_offset = padding + i * (border + square_length);
					width_offset = 3 * padding + j * 3 * (border + square_length);

					for (int k = 0; k < cell.size(); k++) {
						for (int l = 0; l < cell[0].size(); l++) {
							canvas[height_offset + k]
								[width_offset + l] = cell[k][l];
						}
					}




				}
			}

		}

		if (has_mirror) {
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					if (flipshapes[0][i][j]) {
						height_offset = padding + i * (border + square_length);
						width_offset = 6 * padding + 3 * tetromino_width + j * 3 * (border + square_length);

						for (int k = 0; k < cell.size(); k++) {
							for (int l = 0; l < cell[0].size(); l++) {
								canvas[height_offset + k]
									[width_offset + l] = cell[k][l];
							}
						}




					}
				}

			}
		}


		return matrixToString(canvas);

	}
	/*
	pngwriter picture{ canvas_width, canvas_height, 0, filepath.data() };

	aux = List[i].getMainShapeV();
	for (int j = 0; j < aux.size(); j++) {
		for (int k = 0; k < aux[0].size(); k++) {
			if (aux[j][k]) {
				picture.filledsquare(padding + k * (square_length + border),
					padding + j * (square_length + border),
					padding + (k + 1) * (square_length + border) + border - 1,
					padding + (j + 1) * (square_length + border) + border - 1,
					0, 0, 4);

				picture.filledsquare(padding + k * (square_length + border) + border,
					padding + j * (square_length + border) + border,
					padding + (k + 1) * (square_length + border) - 1,
					padding + (j + 1) * (square_length + border) - 1,
					0, 0, 32000);
			}
		}

	}
	picture.close();


}


}*/
private:

	static bool isSurroundend(vector<vector<int>> matrix, int i, int j) {
		//bool Case_1 = i != 0 ? 
		//matrix[i - 1][j] == 1 : false;
		bool Case_1 = i ? matrix[static_cast<std::vector<std::vector<int, std::allocator<int>>, std::allocator<std::vector<int, std::allocator<int>>>>::size_type>(i) - 1][j] == 1 : false;
		//bool Case_2 = j != 0 ? 
		//matrix[i][j - 1] == 1 : false;
		bool Case_2 = j ? matrix[i][static_cast<std::vector<int, std::allocator<int>>::size_type>(j) - 1] == 1 : false;
		//bool Case_3 = i != matrix.size() ? 
		//matrix[i + 1][j] == 1 : false;
		bool Case_3 = i != matrix.size() - 1 ? matrix[static_cast<std::vector<std::vector<int, std::allocator<int>>, std::allocator<std::vector<int, std::allocator<int>>>>::size_type>(i) + 1][j] == 1 : false;
		//bool Case_4 = j != matrix[0].size() ? 
		//matrix[i][j + 1] == 1 : false;
		bool Case_4 = j != matrix[0].size() - 1 ? matrix[i][static_cast<std::vector<int, std::allocator<int>>::size_type>(j) + 1] == 1 : false;
		return Case_1 or Case_2 or Case_3 or Case_4;
	}

};

