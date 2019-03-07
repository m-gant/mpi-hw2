#include "solver.h"
#include <mpi.h>
#include <stdlib.h>
#include <iostream>


#define KILL_MESSAGE_TAG 0
#define QUIT_MESSAGE_TAG
#define PARTIAL_SOLUTION_TAG 1



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


	int num_procs;
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	std::vector<unsigned int> empty_solution;
	empty_solution.reserve(n);

	std::vector<unsigned int> current_partial = partial_generator(n, k, empty_solution);

	for (int proc_rank = 1, i < num_procs; ++proc_rank) {
		MPI_Send(current_partial, k, MPI_INT, proc_rank, PARTIAL_SOLUTION_TAG, MPI_COMM_WORLD);
		
		std::cout<<current_partial.back();
		current_partial.back()++;
		std::cout<<current_partial.back()<<std::endl;

		current = partial_generator(n, k, current_partial);
	}


	bool go = true;
	bool final_partial_sent = false;

	int num_processors_confirmed_dead;


	while(not final_partial_sent || num_processors_confirmed_dead != num_procs - 1) {
		
		MPI_Status stat;
		std::vector<unsigned int> completed_solution;



		MPI_Recv(&completed_solution, n, MPI_INT, MPI_ANY_SOURCE,
			MPI_ANY_TAG, MPI_COMM_WORLD, &status);


		if (status.MPI_TAG == QUIT_MESSAGE_TAG) {
			num_processors_confirmed_dead++
		}

		all_solns.push_back(completed_solution);

		if (current_partial.size() == 0) {
			void * buff;
			MPI_Broadcast(buffer, k, MPI_INT, stat.MPI_SOURCE, KILL_MESSAGE_TAG, MPI_COMM_WORLD);
			final_partial_sent = true;
		} else {
			MPI_Send(current_partial, k, MPI_INT, stat.MPI_SOURCE, PARTIAL_SOLUTION_TAG, MPI_COMM_WORLD);

			current_partial.back()++;
			current_partial = partial_generator(n, k, current_partial);
		}
	}  



	// for (int i = 0; i < num_procs; ++i) {
	// 	//
	// }

	// std::cout << num_procs << std::endl;

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

		if (is_valid(current_solution,n)) {
			seq_solver(n, current_solution, all_solns);
		} 

		current_solution.pop_back();
	}


}



std::vector<unsigned int> partial_generator(unsigned int n, unsigned int k,
	std::vector<unsigned int> next_partial) {


	for (std::vector<unsigned int>::iterator it = next_partial.begin(); it != next_partial.end(); ++it) {
		std::cout << answer*it;
	}

	std::cout << std::endl;

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




