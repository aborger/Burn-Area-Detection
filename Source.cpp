#include <iostream>
#include <fstream>
#include <string>
#include "List.h"

using namespace std;


struct cell {
	char type;
	bool visited;
};

//size and numThisSize
struct format { 
	int size;
	int numThisSize;
};

// used for challenge, x represents the cell's column, y represents cell's row
// x then y
struct cellxy {
	int x;
	int y;
};

// int size, List<cellxy> cells
struct burnAreas {
	int size;
	List <cellxy> cells;
};

// ----------------------------------- Function Prototypes ----------------------------------------
void waitForEnterKey();
// Recursive function responible for the functionality of the program
int traverse(cell** &data, int thisRow, int thisCol, int polySize);
// Used for challenge, returns true if cell is burned
bool traverse(cell** &data, List<cellxy> &burnedTrees, int thisRow, int thisCol);
// used for challenge traverse but for island trees
bool islandTraverse(cell** &data, List<cellxy> &islandTrees, int thisRow, int thisCol);
// converts to better output format
List <format> convert(List <int> areas);
// runs through checking for number of areas and cells in those areas
void normalCheck(List <int> &areas, cell** &data, int cols, int rows);
void printGrid(cell** data, int rows, int cols);

bool challenge(cell** &data, int cols, int rows);
void islandCheck(cell** &data, int rows, int cols);

void burnTraverse(cell** &data, int thisRow, int thisCol, List<burnAreas> &burns);

void burnCheck(cell** &data, int cols, int rows);



int main(int argc, char* argv[]) {

	// gets file name
	//cout << "enter file";
	string fileName = argv[1];
	//cin >> fileName;
	ifstream input;
	input.open(fileName);
	// input validation
	while (!input) {
		cout << "File Not Found  ";
		cout << "\n";
		cout << "Input File Name: ";
		cin >> fileName;
		input.open(fileName);
	}
	// n = number of rows and columns
	string in;
	int rows;
	int cols;
	char garbage;

	// the int represents the number of cells in the area
	List <int> areas;

	// reads num of rows and columns from file
	//input.get(in);
	getline(input, in, '=');
	getline(input, in, ',');
	rows = stoi(in); // convert to int

	getline(input, in, '=');
	getline(input, in, '\n');
	cols = stoi(in);


	// dynamically allocate 2d array
	cell **data = new cell*[rows];
	for (int a = 0; a < rows; a++) {
		data[a] = new cell[cols];
	}


	// read in to array
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			char temp;
			input.get(temp);
			// turns any grass pixels to '0'
			if (temp == 'g') {
				data[r][c].type = '0';
			}
			else {
				data[r][c].type = temp;
			}
			data[r][c].visited = false;
			input.get(garbage);
		}
	}


	// Checks for challenge
	cout << "Enter (1) to Initiate Challenge Mode!\nEnter (2) to Continue Without Challenge Mode!" << endl;

	int choice;
	bool error;
	do
	{
		error = 0;
		cout << "Enter the number corresponding to your choice: ";
		cin >> choice;
		if (cin.fail() || choice < 1 || choice > 2) {
			cout << "Please enter a valid integer" << endl;
			error = 1;
			cin.clear();
			cin.ignore(80, '\n');
		}
	} while (error == 1);


	if (choice == 1) {
		// challenge mode
		cout << endl << "Input:";
		printGrid(data, rows, cols);
		cout << endl;
		if (challenge(data, cols, rows) == true) {
			cout << "Burn runs under trees, cells that were trees are now represented with 'B'." << endl;
		}
		else {
			cout << "Burn does not run under trees, any unburnt island is truly unburnt." << endl;
		}
		cout << "Output:";
		printGrid(data, rows, cols);
	}
	else if (choice == 2) {
		cout << "Calculating..." << endl << "Calculations take about 1 second for every 500 square cells." << endl;
		// Normal Mode
		normalCheck(areas, data, cols, rows);


		// Converts to better output format
		List <format> formattedList = convert(areas);


		// outputs
		if (argc == 2) {
			//Print input and output to console
			printGrid(data, rows, cols);

			//Prints output
			cout << endl << "Output:" << endl;
			for (int a = 0; a < formattedList.getListSize(); a++) {
				if (formattedList.getAt(a).numThisSize > 0)
					cout << formattedList.getAt(a).numThisSize << " shape(s) with size " << formattedList.getAt(a).size << endl;
			}
		}
		else if (argc == 3) {
			//Print output to file
			ofstream outFile;
			outFile.open(argv[2]);

			for (int a = 0; a < formattedList.getListSize(); a++) {
				if (formattedList.getAt(a).numThisSize > 0)
					outFile << formattedList.getAt(a).numThisSize << " shape(s) with size " << formattedList.getAt(a).size << "\n";
			}
		}
		else {
			// error
			cout << "Either too many command line arguments, or not enough" << endl;
		}
	}
	else {
		cout << "Error with challenge choice" << endl;
	}

	cout << "Program Complete, Press Enter to Exit" << endl;
	waitForEnterKey();
}

// converts to better output format
List <format> convert(List <int> areas) {
	// made a variable so it doesnt have to keep going through list to get size, just increases efficiency
	int areaSize = areas.getListSize();

	List <format> formattedList;

	// goes through every possible size and adds their count to the size's list
	areas.resetCurrent();

	bool happened = false; // I know its not really descriptive but i was out of ideas, it turns true if it finds an item in the list with the same amount of areas
	int percent = areaSize / 100;
	for (int area = 0; area < areaSize; area++) {

		happened = false;
		formattedList.resetCurrent();
		for (int f = 0; f < formattedList.getListSize(); f++) {
			if (areas.getCurrent() == formattedList.getCurrent().size && happened == false) {
				format temp;
				temp = formattedList.getCurrent();
				temp.numThisSize++;
				formattedList.setCurrent(temp);
				happened = true;
			}
			formattedList.nextCurrent();
		}
		if (happened == false) {
			format temp = { areas.getCurrent(), 1 };
			formattedList.newNodeTail(temp);
		}
		areas.nextCurrent();
	}
	return formattedList;
}

void waitForEnterKey() {
	cin.ignore();
	char getchar[1];
	std::cin.getline(getchar, 1);
	return;
}

// runs through checking for number of areas and cells in those areas
void normalCheck(List <int> &areas, cell** &data, int cols, int rows) {
	// sets the edge border to visited = true so it doesn't go outside of the border
	// top and bottom
	for (int c = 0; c < cols; c++) {
		data[0][c].visited = true;
		data[rows - 1][c].visited = true;
	}

	// left and right
	for (int r = 0; r < rows; r++) {
		data[r][0].visited = true;
		data[r][cols - 1].visited = true;
	}

	// goes through grid, if it hasnt been visited it adds its area to the list
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {

			if (data[r][c].visited == false && data[r][c].type == 't') {
				areas.newNodeTail(traverse(data, r, c, 0));
			}
		}
	}
}

void burnCheck(cell** &data, int cols, int rows) {
	List <burnAreas> burns;
	// turn all visited to false again
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			data[r][c].visited = false;
		}
	}
	// sets the edge border to visited = true so it doesn't go outside of the border
// top and bottom
	for (int c = 0; c < cols; c++) {
		data[0][c].visited = true;
		data[rows - 1][c].visited = true;
	}

	// left and right
	for (int r = 0; r < rows; r++) {
		data[r][0].visited = true;
		data[r][cols - 1].visited = true;
	}

	// adds burns to list
	// goes through grid, if it hasnt been visited it adds its area to the list
	burns.resetCurrent();
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if (data[r][c].visited == false && data[r][c].type == 'b') {
				burnAreas temp;
				temp.size = 0;
				burns.newNodeTail(temp);
				// make burns current the newest node
				burns.tailCurrent();
				burnTraverse(data, r, c, burns);
			}
		}
	}

	// turns to grass
	burns.resetCurrent();
	for (int a = 0; a < burns.getListSize(); a++) {
		// if true its shadow, make all pixels grass
		if (burns.getCurrent().size < 4) {
			burnAreas temp = burns.getCurrent();
			// makes every pixel in cells list grass
			temp.cells.resetCurrent();
			for (int b = 0; b < temp.size; b++) {
				int r = temp.cells.getCurrent().y;
				int c = temp.cells.getCurrent().x;
				data[r][c].type = '0';
				temp.cells.nextCurrent();
			}
			// temp goes out of scope
		}
		// if its greater than 4 than move on

		burns.nextCurrent();
	}
}

void burnTraverse(cell** &data, int thisRow, int thisCol, List<burnAreas> &burns) {
	if (data[thisRow][thisCol].visited == false && data[thisRow][thisCol].type == 'b') {
		data[thisRow][thisCol].visited = true;
		// tempBurns holds current burns from list (tempBurns represents one burn area)
		burnAreas tempBurns = burns.getCurrent();
		tempBurns.size++;
		// cellxy temp represents this new cell in the area being put in
		cellxy temp = { thisCol, thisRow };
		// puts temp in tempCells
		tempBurns.cells.newNodeTail(temp);
		// puts tempBurns back in burns
		burns.setCurrent(tempBurns);



		// goes to up cell
		burnTraverse(data, thisRow - 1, thisCol, burns);
		// goes to down cell
		burnTraverse(data, thisRow + 1, thisCol, burns);
		// goes to left cell
		burnTraverse(data, thisRow, thisCol - 1, burns);
		// goes to right cell
		burnTraverse(data, thisRow, thisCol + 1, burns);
	}
}

bool challenge(cell** &data, int cols, int rows) {
	burnCheck(data, cols, rows);
	// turn all visited to false again
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			data[r][c].visited = false;
		}
	}
	// Trees next to a burned pixel
	List <cellxy> burnedTrees;
	// displayed as G, T, B
	// Island - totally surrounded by burn pixels
		// if any grass pixels, leave as is, vegisland
		//  no grass pixels, change island to burned, TreeIsland
	// if burn is less than 4 pixels, change to grass
		// sets the edge border to visited = true so it doesn't go outside of the border
	// top and bottom
	for (int c = 0; c < cols; c++) {
		data[0][c].visited = true;
		data[rows - 1][c].visited = true;
	}

	// left and right
	for (int r = 0; r < rows; r++) {
		data[r][0].visited = true;
		data[r][cols - 1].visited = true;
	}

	// goes through grid, if a t is next to a b, it adds it to burned tree list
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {

			if (data[r][c].visited == false && data[r][c].type == 't') {
				if (traverse(data, burnedTrees, r, c) == 1) {
					cellxy temp = { r, c };
					burnedTrees.newNodeTail(temp);
				}
			}
		}
	}

	// turns all trees next to burns to 'T' if its an island it would turn the edges
	burnedTrees.resetCurrent();
	for (int a = 0; a < burnedTrees.getListSize(); a++) {
		data[burnedTrees.getCurrent().x][burnedTrees.getCurrent().y].type = 'T';
		burnedTrees.nextCurrent();
	}
	// goes through burnedTrees list, checks if there is a burn somewhere in its each directions before an edge is found
	// if all edges surrounded by burns any 't's in the way are now "in the island" or type 'i'
	burnedTrees.resetCurrent();
	for (int a = 0; a < burnedTrees.getListSize(); a++) {
		// list of t's to possibly turn into i's
		List <cellxy> ts;
		int r = burnedTrees.getCurrent().y;
		int c = burnedTrees.getCurrent().x;

		// counts how many directions have a burn in them
		int cnt = 0;
		// checks up (-r) direction
		int currentR = r;
		while (true) {
			// if grass or edge, it breaks
			if (data[currentR][c].type == '0') {
				break;
			}
			else if (data[currentR][c].type == 'b') {
				cnt++;
				break;
			}
			else if (data[currentR][c].type == 't') {
				cellxy temp = { c, currentR };
				ts.newNodeTail(temp);
			}
			// aren't checking for grass so if its anything else well move on, or if it was one of those we still move on
			currentR--;
		}
		// checks down (+r) direction
		currentR = r;
		while (true) {
			if (data[currentR][c].type == '0') {
				break;
			}
			else if (data[currentR][c].type == 'b') {
				cnt++;
				break;
			}
			else if (data[currentR][c].type == 't') {
				cellxy temp = { c, currentR };
				ts.newNodeTail(temp);
			}
			// aren't checking for grass so if its anything else well move on, or if it was one of those we still move on
			currentR++;
		}


		// checks left (-c) direction
		int currentC = c;
		while (true) {
			if (data[currentR][c].type == '0') {
				break;
			}
			else if (data[currentR][c].type == 'b') {
				cnt++;
				break;
			}
			else if (data[currentR][c].type == 't') {
				cellxy temp = { c, currentR };
				ts.newNodeTail(temp);
			}
			// aren't checking for grass so if its anything else we'll move on, or if it was one of those we still move on
			currentC--;
		}
		// checks right (+c) direction
		currentC = c;
		while (true) {
			if (data[currentR][c].type == '0') {
				break;
			}
			else if (data[currentR][c].type == 'b') {
				cnt++;
				break;
			}
			else if (data[currentR][c].type == 't') {
				cellxy temp = { c, currentR };
				ts.newNodeTail(temp);
			}
			// aren't checking for grass so if its anything else well move on, or if it was one of those we still move on
			currentC++;
		}

		// if all were burns change t's to i's
		if (cnt == 4) {
			ts.resetCurrent();
			for (int t = 0; t < ts.getListSize(); t++) {
				data[ts.getCurrent().y][ts.getCurrent().x].type = 'i';
				ts.nextCurrent();
			}
		}
		// if it wasn't, then move on
		burnedTrees.nextCurrent();
	}
	// if a 'T' is next to an 'i' it is part of the island change it to 'i' -----------------------------------------------------------

	islandCheck(data, rows, cols);
	// run again to get any 'T's in corners
	islandCheck(data, rows, cols);
	islandCheck(data, rows, cols);


	// turn all i's to b's, if no i's then theres no island --------------------------------------------------------------------------------------
	bool isIsland = false;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if (data[r][c].type == 'i') {
				isIsland = true;
				data[r][c].type = 'B';
			}
		}
	}

	// turn any T's not next to an island back to a 't'
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if (data[r][c].type == 'T') {
				data[r][c].type = 't';
			}
		}
	}
	return isIsland;
}

void islandCheck(cell** &data, int rows, int cols) {
	// turn all visited to false again
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			data[r][c].visited = false;
		}
	}

	List <cellxy> islandTrees;

	for (int c = 0; c < cols; c++) {
		data[0][c].visited = true;
		data[rows - 1][c].visited = true;
	}

	// left and right
	for (int r = 0; r < rows; r++) {
		data[r][0].visited = true;
		data[r][cols - 1].visited = true;
	}

	// goes through grid, if a 'T' is next to an i, it adds it to burned tree list
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {

			if (data[r][c].visited == false && data[r][c].type == 'T') {
				if (islandTraverse(data, islandTrees, r, c) == true) {
					cellxy temp = { r, c };
					islandTrees.newNodeTail(temp);
				}
			}
		}
	}

	// turns all T's next to i's to 'b'
	islandTrees.resetCurrent();
	for (int a = 0; a < islandTrees.getListSize(); a++) {
		data[islandTrees.getCurrent().x][islandTrees.getCurrent().y].type = 'i';
		islandTrees.nextCurrent();
	}
}

bool islandTraverse(cell** &data, List<cellxy> &islandTrees, int thisRow, int thisCol) {
	if (data[thisRow][thisCol].type == 'i') {
		return 1;
	}
	else if (data[thisRow][thisCol].visited == false && data[thisRow][thisCol].type == 'T') {
		data[thisRow][thisCol].visited = true;

		// goes to up cell
		if (islandTraverse(data, islandTrees, thisRow - 1, thisCol) == true) {
			cellxy temp = { thisRow, thisCol };
			islandTrees.newNodeTail(temp);
		}
		// goes to down cell
		if (islandTraverse(data, islandTrees, thisRow + 1, thisCol) == true) {
			cellxy temp = { thisRow, thisCol };
			islandTrees.newNodeTail(temp);
		}
		// goes to left cell
		if (islandTraverse(data, islandTrees, thisRow, thisCol - 1) == true) {
			cellxy temp = { thisRow, thisCol };
			islandTrees.newNodeTail(temp);
		}
		// goes to right cell
		if (islandTraverse(data, islandTrees, thisRow, thisCol + 1) == true) {
			cellxy temp = { thisRow, thisCol };
			islandTrees.newNodeTail(temp);
		}

	}
	return 0;
}


bool traverse(cell** &data, List<cellxy> &burnedTrees, int thisRow, int thisCol) {
	if (data[thisRow][thisCol].type == 'b') {
		return 1;
	}
	else if (data[thisRow][thisCol].visited == false && data[thisRow][thisCol].type == 't') {
		data[thisRow][thisCol].visited = true;

		// goes to up cell
		if (traverse(data, burnedTrees, thisRow - 1, thisCol) == true) {
			cellxy temp = { thisRow, thisCol };
			burnedTrees.newNodeTail(temp);
		}
		// goes to down cell
		if (traverse(data, burnedTrees, thisRow + 1, thisCol) == true) {
			cellxy temp = { thisRow, thisCol };
			burnedTrees.newNodeTail(temp);
		}
		// goes to left cell
		if (traverse(data, burnedTrees, thisRow, thisCol - 1) == true) {
			cellxy temp = { thisRow, thisCol };
			burnedTrees.newNodeTail(temp);
		}
		// goes to right cell
		if (traverse(data, burnedTrees, thisRow, thisCol + 1) == true) {
			cellxy temp = { thisRow, thisCol };
			burnedTrees.newNodeTail(temp);
		}

	}
	return 0;
}

// Recursive function responible for the functionality of the program
int traverse(cell** &data, int thisRow, int thisCol, int polySize) {
	if (data[thisRow][thisCol].visited == false && data[thisRow][thisCol].type == 't') {
		data[thisRow][thisCol].visited = true;
		polySize++;

		// goes to up cell
		polySize = traverse(data, thisRow - 1, thisCol, polySize);
		// goes to down cell
		polySize = traverse(data, thisRow + 1, thisCol, polySize);
		// goes to left cell
		polySize = traverse(data, thisRow, thisCol - 1, polySize);
		// goes to right cell
		polySize = traverse(data, thisRow, thisCol + 1, polySize);
	}
	return polySize;
}

void printGrid(cell** data, int rows, int cols) {
	//Prints grid/input
	cout << endl;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			cout << data[r][c].type << " ";
		}
		cout << "\n";
	}
}