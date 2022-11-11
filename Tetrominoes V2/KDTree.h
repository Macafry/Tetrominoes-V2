#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "VectorMethods.h"
#include "MatrixMethods.h"


using std::string;
using std::vector;
 

template <typename NumericType>
class KDTree {
	template<typename T> friend class KDTreeIterator;
private:
	KDTree *left;
	KDTree *right;
	vector<NumericType> value;
	int dim;
	
	
public:

	KDTree() {
	}

	KDTree(int dimensions) {
		value = vector<NumericType>(NULL,dim);
		left = nullptr;
		right = nullptr;
		this->dim = dimensions;
	}
	/*
	~KDTree() {
		if (left) {
			left->~KDTree();
		}
		if (right) {
			right->~KDTree();
		}
	}
	*/

	void free() {
		if (left) {
			left->free();
			delete right;
		}
		if (right) {
			right->free();
			delete right;
		}
	}

	bool isEmpty() {
		return !left and !right and value.empty();
	}

	void addItem(vector<NumericType> item) {
		if (item.size() != dim) {
			throw std::invalid_argument("Dimensions don't match");
		}

		addItem(item, 0);
		
	}

	vector<NumericType> getItem() {
		return value;
	}

	KDTree leftChild() {
		return left ? *left : NULL;
	}

	KDTree rightChild() {
		return right ? *right : NULL;
	}

	bool hasRightChild() {
		return right;
	}

	bool hasLeftChild() {
		return left;
	}


	string preOrder() {
		if (isEmpty()) {
			return "";
		}
		string str = vectorToString(value);
		if (left != nullptr) {
			str += left->preOrder();
		}
		if (right != nullptr) {
			str += right->preOrder();
		}
		return str;
	}

	bool search(vector<NumericType> item) {
		if (item.size() != dim) {
			throw std::invalid_argument("Dimensions don't match");
		}
		search(item, 0);
	}

	KDTree balancedInsertion(vector<vector<NumericType>> items) {
		if (!isRectangular(items)) {
			throw std::invalid_argument("Incoherent dimensions");
		}
		
		KDTree<NumericType> KDT(items[0].size());
		KDT.balancedInsertion(items, 0);
		return KDT;
	}

	vector<vector<NumericType>> toMatrix() {
		vector<vector<NumericType>> matrix;
		toMatrix(&matrix);
		return matrix;
	}

	void remove(vector<NumericType> item) {
		if (item.size() != dim) {
			throw std::invalid_argument("Dimensions don't match");
		}
		privRemove(item);

	}

	int getAmount() {
		return isEmpty() ? 0 :
			(1 + (left ? left->getAmount() : 0)
			+ (right ? right->getAmount() : 0));
	}

	string toDot() {
		string str = "digraph G {\n\tlabel = \"Total: ";
		str += std::to_string(getAmount());
		str += "\"\n\t labelloc = b\n\tnode [shape = record,height=.1];\n";
		str += dotDeclarations("ROOT") + dotRelations("ROOT") + "}";
		
		return str;
	}

	
private:
	void addItem(vector<NumericType> item, int depth) {
		if (isEmpty()) {
			value = item;
			return;
		}

		int pos = depth % dim;
		if (item[pos] <= value[pos]) {
			if (!left) {
				left = new KDTree<NumericType>(dim);
			}
			left->addItem(item, depth + 1);
			return;
		}

		if (!right) {
			right = new KDTree<NumericType>(dim);
		}
		right->addItem(item, depth + 1);
		return;


	}

	bool search(vector<NumericType> item, int depth) {
		if (isEmpty()) {
			return false;
		}

		if (item == value) {
			return true;
		}

		int pos = depth % dim;

		if (item[pos] <= value[pos]) {
			if (!left) {
				return false;
			}

			return left->search(item, depth + 1);
		}

		if (!right) {
			return false;
		}
		
		return right->search(item, depth + 1);
	}

	void balancedInsertion(vector<vector<NumericType>> items, int depth) {
		int col = depth % dim, medpos = (items.size() - 1) / 2;
		vector<vector<NumericType>> low;
		vector<vector<NumericType>> high;
		auto med = medianByColumn(items, col);
		value = med;

		std::copy_if(items.begin(), items.end(), std::back_inserter(low), 
			[med, col](vector<NumericType> vec) {
				return (vec[col] <= med[col]) && (vec != med);
			});

		std::copy_if(items.begin(), items.end(), std::back_inserter(high),
			[med, col](vector<NumericType> vec) {
				return vec[col] > med[col];
			});
		if (low.size()) {
			left = new KDTree<NumericType>(dim);
			left->balancedInsertion(low, depth + 1);
		}


		if (high.size()) {
			right = new KDTree<NumericType>(dim);
			right->balancedInsertion(high, depth + 1);
		}
		
	}

	void toMatrix(vector<vector<NumericType>>* matrix) {
		if (isEmpty()) {
			return;
		}
		
		matrix->push_back(value);
		if (left) {
			left->toMatrix(matrix);
		}
		if (right) {
			right->toMatrix(matrix);
		}
	}

	void privRemove(vector<NumericType> item) {
		if (value == item) {
			auto matrix = toMatrix();
			deleteItem(&matrix, item);
			this = balancedInsertion(matrix);
			return;
		}

		if (!left) {
			if (left->search(item)) {
				left->privRemove(item);
				return;
			}
		}

		if (!right) {
			if (right->search(item)) {
				right->privRemove(item);
			}
		}


	}

	KDTree<NumericType>* leftPointer() {
		return left;
	}

	KDTree<NumericType>* rightPointer() {
		return right;
	}

	string dotDeclarations(string prefix) {
		string DOT = "\tN" + prefix+"[label = \"<f0> |<f1> " 
			+ vectorToStringNNL(value) + "|<f2> \"];\n";

		if (left) {
			DOT += left->dotDeclarations(prefix + "l");
		}
		if (right) {
			DOT += right->dotDeclarations(prefix + "r");
		}
		return DOT;
	}

	string dotRelations(string prefix) {
		string DOT = "";

		if (left) {
			DOT += "\t\"N" + prefix + "\":f0 -> \"N" + prefix + "l\":f1;\n";
			DOT += left->dotRelations(prefix + "l");
		}
		if (right) {
			DOT += "\t\"N" + prefix + "\":f2 -> \"N" + prefix + "r\":f1;\n";
			DOT += right->dotRelations(prefix + "r");
		}
		return DOT;
	}


};


#include <stack>
using std::stack;

template<typename NumericType>
class KDTreeIterator
{
private:
	KDTree<NumericType>* root;
	stack<KDTree<NumericType>*> stk;

	void goLeftmost() {
		if (!stk.empty()) {
			auto temp = stk.top();
			temp = temp->leftPointer();
			while (temp) {
				stk.push(temp);
				temp = temp->leftPointer();
			}
		}
	}

	void goUpTilLeft() {
		auto temp = stk.top();
		stk.pop();
		auto temp2 = stk.top();
		while (stk.size() > 1 and temp2->leftPointer() != temp) {
			temp = temp2;
			stk.pop();
			temp2 = stk.top();
		}

		if (temp2->leftPointer() != temp) {
			stk.pop();
		}

	}

public:
	KDTreeIterator(KDTree<NumericType>* root) {
		this->root = root;
		stk.push(root);
		goLeftmost();
	}

	bool hasNext() {
		return !stk.empty();
	}

	std::vector<NumericType> next() {
		if (stk.empty()) {
			throw std::out_of_range("There is no next item");
		}

		auto temp = stk.top();

		if (temp->hasRightChild()) {
			stk.push(temp->rightPointer());
			goLeftmost();
		}
		else {
			goUpTilLeft();
		}

		return temp->getItem();

	}

};
