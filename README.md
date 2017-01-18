# CM30225 Parallel Computing Coursework 1
http://people.bath.ac.uk/masrjb/CourseNotes/cm30225.html

# Grade
60/100 - Focused too much on code. Make sure you spend time on the report.

# Running the Coursework
Scripts to run the coursework on Balena are available in **./bin** in the submitted coursework.
- **./bin/cw-prepare.sh** puts the compiled program in your ~. The source directory should be changed to match a clean version of the coursework.
- **./bin/cw-run.sh** will produce python jobs in your ~ and submit them using sbatch to balena. Outputs will appear in ~/out
- Running the **python3 ./bin/slurm_job_gen.py** will produce jobs for Balena in batches of 15 mins. 
- **./bin/cw-status.sh** Will display job status.
- **./bin/cw-collect-results.sh** will process the output of Balena and can be pasted directly into Google Sheets for analysis.

# Feedback

- Somewhat over-engineered!  (OK, but why make it harder for yourself?  A simple program is all that is needed)
- More comments in your code, please!  Your code is hard to follow. For example,
- why the CRCs? And it's hard to find the core relaxation loop
- While it's not an error to use random numbers, it does need a little thought and care.  Using random (or randomised) data in your matrix makes interpretation and reproducibility of timing difficult (you could run several times and average, but that is extra work), and also checking for correctness harder.  Using a fixed initial pattern is slightly better, but better again a fixed non-random pattern gives better reproducibility during timings; also comparing across matrix sizes is more meaningful. Note that different initial data may take different times to converge, e.g., some take a long time to converge as their values are, by chance, much further away from their converged values.
- // Start work & reset: barriers can be reused.  No need to destroy and recreate
- Your code is OK for what it does, but this coursework can be done in a couple of pages of code.  I suspect more of your runtime will be spent in overheads than in actual relaxation computation
- Correctness testing: OK, but needs to include a few more results to be convincing. Note that the result could be symmetric without being correctly relaxed
- Good to see use of valgrind
- Scalability: a good start but more (results and discussion) would be better.
- How does your code scale with size of matrix?  Efficiency?
- Any thoughts on how your results relate to Amdahl and/or Gustafson? Do they support them or not?

# Code Structure
The code is separated into multiple files and uses pointer obfuscation, to structure the program.
- *mat.c* and the *mat_t* is the core of the code and represents the matrix that will be relaxed.
- *mat.c* holds helper functions that operate on mat_t} and also contains the sequential version of the smoothing algorithm.
- There are two different iterators provided (mat_itr_t and mat_itr_edge_t) to parse the matrix. This abstracts the representation of the matrix data inside and using co-ordinates during initialisation, you can easily specify which of the area of the matrix needs to be parsed.
- mat_itr_t can also be  split into two new iterators. The two iterators will now step twice as far in each iteration out of sync. This can be done recursively until you get the size of iterator desired.
- A smoother is provided. This is a key part of the program. It is made up of 6 iterators, 5 in a cross pattern iterating through the source matrix and a final iterator going through the target matrix that smoothed values will be put in. The central iterator is there to check the difference between the new smoothed value and the old value. If this is more than the diff given the smoother can now ignore the extra subtraction and does not need to iterate the central pointer, saving CPU cycles. As it uses iterators, a smoother can also be split recursively.
- matrix_smooth_parallel.c contains all the methods of parallelism and synchronisation used.
- *spool.c* (smoothing pool) is a thread pool used as the final method of parallelism;

# Speedup
On a 2x8 core machine

Maximum speed up tested: **14.8** - Row Size **20** - threads **20** - size **8192** - type **(4) Pool Row Cut Jump**
![Speedup Graph](http://i.imgur.com/0R0xq9C.png)
