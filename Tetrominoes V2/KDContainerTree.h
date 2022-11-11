#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "VectorMethods.h"
#include "MatrixMethods.h"


using std::string;
using std::vector;

// This is an implementation of a KDTree - A binary tree that sorts in multiple dimensions
// However, for the purposes of this project it has a container instead of a value in each node

/* Note: at this point I was into "programming as compactly as possible"
*  there are a lot of `if(object)` that represented `if(object is not null)`
*/ 



template<class T, typename NumericType>
class KDContainerTree
{
	template<class U,typename V> friend class KDContainerTreeIterator;
	
private:
	// Children
	KDContainerTree*left;
	KDContainerTree*right;

	// Coordinate value identifier
	vector<NumericType> value;

	// Container for other objects
	vector<T> container;

	// how many dimensions does the tree have
	int dim;
	

public:
	// Constructors
	KDContainerTree() {
	}

	KDContainerTree(int dimensions) {
		value = vector<NumericType>(NULL,dim);
		left = nullptr;
		right = nullptr;
		this->dim = dimensions;
	}
	
	// trying to do c++ right 
	void free() {
		if (left) {
			left->free();
			delete left;
		}
		if (right) {
			right->free();
			delete right;
		}
	}


	bool isEmpty() {
		return !left and !right and value.empty() and container.empty();
	}

	// adds a coordinate to the tree
	void populate(vector<NumericType> val) {
		if (val.size() != dim) {
			throw std::invalid_argument("Dimensions don't match");
		}


		// calling a private method with handles the actual logic
		populate(val, 0);
		
	}
	

	// Getters, setters, 
	vector<NumericType> getValue() {
		return value;
	}

	KDContainerTree leftChild() {
		return left ? *left : NULL;
	}

	KDContainerTree rightChild() {
		return right ? *right : NULL;
	}

	// abuse of automatic type casting
	// `return child is not nullptr` would be a better option
	bool hasRightChild() {
		return right;
	}

	bool hasLeftChild() {
		return left;
	}

	vector<T> getContainer() {
		return container;
	}
	
	// end of getters and setters

	// brief summary of the tree's structure - used for debugging
	string preOrder() {
		if (isEmpty()) {
			return "";
		}

		// formats the value vector as python's `print(list)` and appends the amount of items in the countainer
		string str = vectorToStringNNL(value) + " - " +
			std::to_string(container.size()) + "\n";

		if (left != nullptr) {
			str += left->preOrder();
		}
		if (right != nullptr) {
			str += right->preOrder();
		}
		return str;
	}
	
	// searches if a coordinate exists in the tree
	bool searchValue(vector<NumericType> val) {
		if (val.size() != dim) {
			throw std::invalid_argument("Dimensions don't match");
		}
		searchValue(val, 0);
	}

	// given a coordinate, return the coordinate's container
	vector<T> getItemsWithValue(vector<NumericType> val) {
		if (val.size() != dim) {
			throw std::invalid_argument("Dimensions don't match");
		}

		//private method that handles the actual logic
		return *getItemsWithValue(val, 0);
	}

	// Check if an item is cointained in any of the tree's containers
	// The item's coordinate is used to find the container that might have it
	bool searchItem(T item, vector<NumericType> val) {
		if (val.size() != dim) {
			throw std::invalid_argument("Dimensions don't match");
		}

		auto items = getItemsWithValue(val);

		//linear search
		for (auto& i : items) {
			if (item == i) {
				return true;
			}
		}
		return false;
	}
	
	// given the set of expected coordinates, create a somewhat balanced tree with each of them
	KDContainerTree balancedPopulation(vector<vector<NumericType>> val) {
		if (!isRectangular(val)) {
			throw std::invalid_argument("Incoherent dimensions");
		}
		
		KDContainerTree<T,NumericType> KDCT(val[0].size());

		// calling a private method with handles the actual logic
		KDCT.balancedPopulation(val, 0);
		return KDCT;
	}
	
	// Inserts an item into the container with the same coordinate
	void insertItem(T item, vector<NumericType> val) {
		if (val.size() != dim) {
			throw std::invalid_argument("Dimensions don't match");
		}
		auto temp = getItemsWithValue(val, 0);
		temp->push_back(item);
	}

	// returns how many items in total are contained in the tree
	int getAmount() {
		return isEmpty() ? 0 :
			container.size() + (left ? left->getAmount() : 0)
			+ (right ? right->getAmount() : 0);
	}


	void removeItem(T item, vector<NumericType> val) {
		if (val.size() != dim) {
			throw std::invalid_argument("Dimensions don't match");
		}
		auto temp = getItemsWithValue(val, 0);
		if (temp) {
			deleteItem(temp, item);
		}

	}

	// creates a file in .dot format so that a script can turn it into a visual representation of the tree
	// Used for debugging
	string toDot() {
		string str = "digraph G {\n\tlabel = \"Total: ";
		str += std::to_string(getAmount());
		str += "\"\n\t labelloc = b\n\tnode [shape = record,height=.1];\n";
		str += dotDeclarations("ROOT") + dotRelations("ROOT") + "}";

		return str;
	}

	// creates a botched iterator 
	// it iterates through each node of the tree
	// but its not compliant with the normal c++ iterators 
	KDContainerTreeIterator<T, NumericType> iterator() {
		return KDContainerTreeIterator<T, NumericType>(this);
	}
	
private:
	// creates a node in the first place avialable with the given coordinates
	// depth is used to keep track of which dimension's turn is it
	void populate(vector<NumericType> val, int depth) {
		// if there is no real tree, plant it?
		if (isEmpty()) {
			value = val;
			return;
		}
		
		// checking with dimension to check in the vectors holding the coordinates
		int pos = depth % dim;

		// should it be on the left or right child?
		if (val[pos] <= value[pos]) {

			// is there a left child?
			if (!left) {

				// is there a left child?
				left = new KDContainerTree<T,NumericType>(dim);
			}

			// yes - keep looking
			left->populate(val, depth + 1);
			return;
		}

		// is there a right child?
		if (!right) {
			// is there a left child?
			right = new KDContainerTree<T,NumericType>(dim);
		}
		// yes - keep looking
		right->populate(val, depth + 1);
		return;


	}
	
	bool searchValue(vector<NumericType> val, int depth) {
		// there's no tree to search in
		if (isEmpty()) {
			return false;
		}

		// found the value
		if (val == value) {
			return true;
		}

		// same comments as the previous method
		int pos = depth % dim;

		if (val[pos] <= value[pos]) {
			if (!left) {
				return false;
			}

			return left->searchValue(val, depth + 1);
		}

		if (!right) {
			return false;
		}
		
		return right->searchValue(val, depth + 1);
	}

	// given a coordinate, return the coordinate's container
	// I guess it returns a pointer to avoid copying so many objects
	vector<T>* getItemsWithValue(vector<NumericType> val, int depth) {
		// there is no tree, there is no container
		if (isEmpty()) {
			return nullptr;
		}

		// found it
		if (val == value) {
			return &container;
		}

		// same comments as the previous method
		int pos = depth % dim;

		if (val[pos] <= value[pos]) {
			if (!left) {
				return nullptr;
			}

			return left->getItemsWithValue(val, depth + 1);
		}

		if (!right) {
			return nullptr;
		}

		return right->getItemsWithValue(val, depth + 1);
	
	}
	
	// this method allows the creation of a somewhat balanced tree
	void balancedPopulation(vector<vector<NumericType>> val, int depth) {
		int col = depth % dim, medpos = (val.size() - 1) / 2;
		vector<vector<NumericType>> low;
		vector<vector<NumericType>> high;

		// find the median of the coordinates, when sorted by a specific dimension
		auto med = medianByColumn(val, col);
		value = med;

		// put every coordinate "lower" than the median into a vector
		std::copy_if(val.begin(), val.end(), std::back_inserter(low), 
			[med, col](vector<NumericType> vec) {
				return (vec[col] <= med[col]) && (vec != med);
			});

		// put every coordinate "higher" than the median into a vector
		std::copy_if(val.begin(), val.end(), std::back_inserter(high),
			[med, col](vector<NumericType> vec) {
				return vec[col] > med[col];
			});

		//recursion
		if (low.size()) {
			left = new KDContainerTree<T,NumericType>(dim);
			left->balancedPopulation(low, depth + 1);
		}

		//recursion
		if (high.size()) {
			right = new KDContainerTree<T,NumericType>(dim);
			right->balancedPopulation(high, depth + 1);
		}
		
	}


	// Scaffholding for creating a DOT diagram for easier deubugging
	string dotDeclarations(string prefix) {
		string DOT = "\tN" + prefix+"[label = \"<f0> |<f1> " 
			+ vectorToStringNNL(value) + "|<f2> " 
			+ std::to_string(container.size()) + "|<f3> \"];\n";

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
			DOT += "\t\"N" + prefix + "\":f3 -> \"N" + prefix + "r\":f1;\n";
			DOT += right->dotRelations(prefix + "r");
		}
		return DOT;
	}
	

	KDContainerTree<T,NumericType>* leftPointer() {
		return left;
	}

	KDContainerTree<T,NumericType>* rightPointer() {
		return right;
	}
};

#include <stack>
using std::stack;

template<class T, typename NumericType>
class KDContainerTreeIterator
{
private:
	// tree to iterate through
	KDContainerTree<T,NumericType>* root;

	// stack to keep track of current tree
	stack<KDContainerTree<T,NumericType>*> stk;

	
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

		if (stk.empty()) {
			return;
		}

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
	KDContainerTreeIterator(KDContainerTree<T,NumericType>* root) {
		this->root = root;
		stk.push(root);
		goLeftmost();
	}

	bool hasNext() {
		return !stk.empty();
	}

	std::vector<T> next() {
		if (!stk.size()) {
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

		return temp->getContainer();

	}

};

//True iteratpr
//Commented out, might revise later
/*
template<class T, typename NumericType>
class KDContainerTreeIterator
{
private:
	KDContainerTree<T, NumericType>* root;
	stack<KDContainerTree<T, NumericType>*> stk;
	vector<T> currentContainer;
	int currentPos;

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

	void updateContainer() {
		auto temp = stk.top();
		currentContainer = temp->getContainer();
		currentPos = 0;
	}

	void goUpTilLeft() {
		auto temp = stk.top();
		stk.pop();
		if (stk.empty()) { return; }
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
	KDContainerTreeIterator(KDContainerTree<T, NumericType>* root) {
		this->root = root;
		stk.push(root);
		goLeftmost();
		updateContainer();
	}

	
	bool hasNext() {
		while (currentPos >= currentContainer.size() and !stk.empty()) {
			auto temp = stk.top();

			if (temp->hasRightChild()) {
				stk.push(temp->rightPointer());
				goLeftmost();
			}
			else {
				goUpTilLeft();
			}
			updateContainer();
		}

		return !stk.empty();
	}

	T next() {
		while (currentPos >= currentContainer.size()) {
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
			updateContainer();
		}
		auto ret = currentContainer[currentPos];
		currentPos++;
		return ret;

	}

};*/