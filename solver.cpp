#include "solver.h"
#include <mpi.h>
#include <stdlib.h>
#include <iostream>



/*************************** DECLARE YOUR HELPER FUNCTIONS HERE ************************/


unsigned int is_valid(std::vector<unsigned int>& current_solution, unsigned int n);

//i - j if i > j else j - i
unsigned int abs_difference(unsigned int i, unsigned int j);

void seq_solver(unsigned int n, std::vector<unsigned int>& current_solution,
	std::vector<std::vector<unsigned int> >& all_solns);


std::vector<unsigned int> partial_generator(unsigned int n, unsigned int k,
	std::vector<unsigned int> next_partial);


/*************************** solver.h functions ************************/


void seq_solver(unsigned int n, std::vector<std::vector<unsigned int> >& all_solns) {

	// printf("%lu", all_solns.size());

	std::vector<unsigned int> empty_solution;
	empty_solution.reserve(n);

	seq_solver(n, empty_solution, all_solns);

	// std::vector<unsigned int> current_solution = partial_generator(n, n, empty_solution);




	// std::cout << "answer" << std::endl;
	// for (std::vector<unsigned int>::iterator it = current_solution.begin(); it != current_solution.end(); ++it) {
	// 	std::cout << *it;
	// }

	// std::cout << std::endl;

	// unsigned int last_value = current_solution.back();
	// current_solution.pop_back();
	// current_solution.push_back(++last_value);

	// current_solution = partial_generator(n, n, current_solution);
	// std::cout << "answer" << std::endl;
	// for (std::vector<unsigned int>::iterator it = current_solution.begin(); it != current_solution.end(); ++it) {
	// 	std::cout << *it;
	// }

	// std::cout << std::endl;

	// while(true) {
	// 	unsigned int last_value = current_solution.back();
	// 	current_solution.pop_back();
	// 	current_solution.push_back(last_value + 1);
	// 	current_solution = partial_generator(n, n, current_solution);

	// 	if (current_solution.size() == 0) {
	// 		break;
	// 	}

	// 	all_solns.push_back(current_solution);
	// }

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

	int jobs_out = 0;
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	std::vector<unsigned int> last_partial_sol;
	for (int i = 1; i < size; i++) {

		last_partial_sol = partial_generator(n, k, last_partial_sol);
		int last_partial_sol_size = last_partial_sol.size();
		if (last_partial_sol_size > 0) {
			int terminating_signal = 1;
			MPI_Send(&terminating_signal, 1, MPI_INT, i, 100, MPI_COMM_WORLD);
			MPI_Send(&last_partial_sol[0], last_partial_sol_size, MPI_INT, i, 111, MPI_COMM_WORLD);
			jobs_out += 1;
			last_partial_sol[last_partial_sol.size() - 1] += 1;
		} else {
			// we have reached the end of the partial solutions before we have reached the end of
			// the amount of processors we have allotted
		}
	}

	std::vector< std::vector<unsigned int>> partial_solutions;

	/******************* STEP 2: Send partial solutions to workers as they respond ********************/
	/*
	 * while() {
	 * 		- receive completed work from a worker processor.
	 * 		- create a partial solution
	 * 		- send that partial solution to the worker that responded
	 * 		- Break when no more partial solutions exist and all workers have responded with jobs handed to them
	 * }
	 */

	 while(last_partial_sol.size() > 0 || jobs_out > 0) {

		 //get solns from worker
		 int vec_size;
		 std::vector<unsigned int> found_solns;
		 MPI_Status status;
		 MPI_Recv(&vec_size, 1, MPI_INT, MPI_ANY_SOURCE, 222, MPI_COMM_WORLD, &status);
		 found_solns.resize(vec_size);
		 MPI_Recv(&found_solns[0], vec_size, MPI_INT, status.MPI_SOURCE, 333, MPI_COMM_WORLD, &status);
		 jobs_out -= 1;
		 //add solns to all_solns vector
		 int num_solns = vec_size / n;

		 for (int i = 1; i <= num_solns; i++) {

			 std::vector<unsigned int> one_soln(found_solns.begin() +  ((i-1) * n), found_solns.begin() + (i * n));
			 all_solns.push_back(one_soln);

		 }



		 if (last_partial_sol.size() > 0) {
			 last_partial_sol = partial_generator(n, k, last_partial_sol);

			 int terminating_signal = 1;
			 MPI_Send(&terminating_signal, 1, MPI_INT, status.MPI_SOURCE, 100, MPI_COMM_WORLD);
			 MPI_Send(&last_partial_sol[0], last_partial_sol.size(), MPI_INT, status.MPI_SOURCE, 111, MPI_COMM_WORLD);
			 jobs_out += 1;
			 if (last_partial_sol.size() > 0 ) {
				 last_partial_sol[last_partial_sol.size() - 1] += 1;
			 }
		 }

	 }


	/********************** STEP 3: Terminate **************************
	 *
	 * Send a termination/kill signal to all workers.
	 *
	 */

	 for (int i = 1; i < size; i++) {
		 int terminating_signal = -1;
		 MPI_Send(&terminating_signal, 1, MPI_INT, i, 100, MPI_COMM_WORLD);
	 }




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

	 bool running = true;
	 while (running) {
		 int term_sig;
		 MPI_Status status;
		 MPI_Recv(&term_sig, 1, MPI_INT, 0, 100, MPI_COMM_WORLD, &status);
		 if (term_sig > 0) {
			 std::vector<std::vector<unsigned int>> all_finished_solns;
			 std::vector<unsigned int> partial_solution;
			 partial_solution.resize(k);
			 MPI_Recv(&partial_solution[0], k, MPI_INT, 0, 111, MPI_COMM_WORLD, &status);
			 seq_solver(n, partial_solution, all_finished_solns);
			 std::vector<unsigned int> sendable_vec;
			 for (int i = 0; i <  all_finished_solns.size(); i++) {
				 std::vector<unsigned int> cur_vec = all_finished_solns[i];
				 for (int j= 0; j < cur_vec.size(); j++) {
					 sendable_vec.push_back(cur_vec[j]);
				 }
			 }

			 int sendable_vec_size = sendable_vec.size();
			 MPI_Send(&sendable_vec_size, 1, MPI_INT, 0, 222, MPI_COMM_WORLD);
			 MPI_Send(&sendable_vec[0], sendable_vec.size(), MPI_INT, 0, 333, MPI_COMM_WORLD);


		 } else {
			 running = false;
		 }

	 }


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

		if (is_valid(current_solution,n)) {
			seq_solver(n, current_solution, all_solns);
		}

		current_solution.pop_back();
	}


}



std::vector<unsigned int> partial_generator(unsigned int n, unsigned int k,
	std::vector<unsigned int> next_partial) {


	for (std::vector<unsigned int>::iterator it = next_partial.begin(); it != next_partial.end(); ++it) {
		//std::cout << *it;
	}

	//std::cout << std::endl;

	if (is_valid(next_partial,n)) {
		if (next_partial.size() == k) {
			return next_partial;
		} else {
			next_partial.push_back(0);
			return partial_generator(n, k, next_partial);
		}
	} else {
		unsigned int last_value = next_partial.back();
		next_partial.pop_back();

		if (last_value + 1 >= n) {
			if (next_partial.size() == 0) {
				return next_partial;
			}


			unsigned int second_to_last_value = next_partial.back();
			next_partial.pop_back();

			next_partial.push_back(second_to_last_value + 1);
			return partial_generator(n, k, next_partial);

		} else {
			next_partial.push_back(last_value + 1);
			return partial_generator(n,k, next_partial);
		}
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


unsigned int is_valid(std::vector<unsigned int>& possible_sol, unsigned int n) {

	if (possible_sol.size() == 1) {
		return possible_sol[0] < n;
	}

	for (int i = 0; i < int(possible_sol.size()) - 1; i++) {
		unsigned int j = possible_sol[i];

		if (j >= n) {
			return 0;
		}

		for (int k = i + 1; k < int(possible_sol.size()); k++) {
			unsigned int l = possible_sol[k];

			if (l >= n) {
				return 0;
			}

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
