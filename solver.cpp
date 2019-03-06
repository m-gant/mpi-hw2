#include "solver.h"
#include <mpi.h>
#include <stdlib.h>
#include <iostream>



/*************************** DECLARE YOUR HELPER FUNCTIONS HERE ************************/


unsigned int is_valid(std::vector<unsigned int>& current_solution);

//i - j if i > j else j - i
unsigned int abs_difference(unsigned int i, unsigned int j);

void seq_solver(unsigned int n, std::vector<unsigned int>& current_solution,
	std::vector<std::vector<unsigned int> >& all_solns);

void generate_next(int size, std::vector<unsigned int>& solution);

/*************************** solver.h functions ************************/


void seq_solver(unsigned int n, std::vector<std::vector<unsigned int> >& all_solns) {

	// printf("%lu", all_solns.size());

	std::vector<unsigned int> empty_solution;
	empty_solution.reserve(n);

	seq_solver(n, empty_solution, all_solns);

	// std::cout << all_solns.size() <<  std::endl;
}






void nqueen_master(	unsigned int n,
					unsigned int k,
					std::vector<std::vector<unsigned int> >& all_solns) {




	// TODO: Implement this function

	/* Following is a general high level layout that you can follow
	 (you are not obligated to design your solution in this manner.
	  This is provided just for your ease). */


	/******************* STEP 1: Send one partial solution to each worker ********************/
	/*
	 * for (all workers) {
	 * 		- create a partial solution.
	 * 		- send that partial solution to a worker
	 * }
	 */

	std::vector< std::vector<unsigned int>> partial_solutions;


	// if (current_node == 0)






	// int num_procs;
	// MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	// for (int i = 0; i < num_procs; ++i) {
	// 	//
	// }

	// std::cout << num_procs << std::endl;

	/******************* STEP 2: Send partial solutions to workers as they respond ********************/
	/*
	 * while() {
	 * 		- receive completed work from a worker processor.
	 * 		- create a partial solution
	 * 		- send that partial solution to the worker that responded
	 * 		- Break when no more partial solutions exist and all workers have responded with jobs handed to them
	 * }
	 */

	/********************** STEP 3: Terminate **************************
	 *
	 * Send a termination/kill signal to all workers.
	 *
	 */





}

void nqueen_worker(	unsigned int n,
					unsigned int k) {



	// TODO: Implement this function

	// Following is a general high level layout that you can follow (you are not obligated to design your solution in this manner. This is provided just for your ease).

	/*******************************************************************
	 *
	 * while() {
	 *
	 * 		wait for a message from master
	 *
	 * 		if (message is a partial job) {
	 *				- finish the partial solution
	 *				- send all found solutions to master
	 * 		}
	 *
	 * 		if (message is a kill signal) {
	 *
	 * 				quit
	 *
	 * 		}
	 *	}
	 */


}



/*************************** DEFINE YOUR HELPER FUNCTIONS HERE ************************/

void seq_solver(unsigned int n, std::vector<unsigned int>& current_solution,
	std::vector<std::vector<unsigned int> >& all_solns) {

	if (current_solution.size() == n) {
		all_solns.push_back(current_solution);
		return;
	}

	for (int i = 0; i < n; i++) {

		current_solution.push_back(i);

		if (is_valid(current_solution)) {
			seq_solver(n, current_solution, all_solns);
		} 

		current_solution.pop_back();
	}


}


// void set_next_partial_from_current(unsigned int size, std::vector<unsigned int>& partial_solution) {

// 	if (partial_solution.size() != size) {
// 		std::ostringstream err_msg;
// 		err_msg << "\n";

// 		err_msg << "partial_solution passed to set_next_partial_from_current isn't expected size";
// 		std::cout << err_msg.str() << std::endl;
// 		MPI_Abort(MPI_COMM_WORLD, 1);

// 	}

// 	if (partial_solution.size() < size) {
// 		for (int i = 0; i < possible_sol.size())
// 	}
// }


unsigned int is_valid(std::vector<unsigned int>& possible_sol) {

	for (int i = 0; i < possible_sol.size() - 1; i++) {
		unsigned int j = possible_sol[i];

		for (int k = i + 1; k < possible_sol.size(); k++) {
			unsigned int l = possible_sol[k];
		
			if (j == l || abs_difference(i, k) == abs_difference(j, l)) {
				return 0;
			}

		}
	}

	return 1;
}

unsigned int abs_difference(unsigned int i, unsigned int j) {
	return i > j ? i - j : j - i;
}




