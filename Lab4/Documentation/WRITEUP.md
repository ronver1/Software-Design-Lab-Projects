# ECE 312 Lab 4 Write-Up: Tech Support Diagnosis Tool

**Name:** Ronit Verma
**EID:** RV24923
**Date:** \[Submission Date]

\---

## What This Document Is

1–2 pages of honest reflection on the decisions you made and the problems you hit. Not a summary of the lab spec.

Full credit requires:

* Two specific design choices with a stated reason and a named alternative
* Four Big-O analyses with reasoning shown
* Two concrete bugs (symptom → cause → fix → rule)
* Reflection on the knowledge base you grew through sessions
* A note on `find\\\\\\\\\\\\\\\_shortest\\\\\\\\\\\\\\\_path`

Vague entries ("I had a leak and fixed it") earn no credit.

\---

## Section 1 — Design Choices (two required, \~100 words each)

For each: what did you choose, what was the alternative, and why?

Candidate topics:

1. Array-backed stack vs. linked-list stack

I implemented an array based stack over a linked list stack as its advantages outweighed its disadvantages. For example, it is faster to access elements in an array rather than a linked list. With an array, one can access any elements via its index, resulting in a random access of O(1). On the other hand, a linked list required traversing from the head to the chosen element, significantly increasing its access time. Moreover, implementing an array is simpler and easier since one needs to just track the size and capacity. With a linked list, one must allocate a new linked list node each time an element needs to be added. The final reason as to why I chose to implement an array over a linked list references the cache. Because elements are contiguously stored in memory, the CPU's cache system efficiently works every time the Undo/Redo functions are called. However, because a linked list's elements are scattered in memory, the CPU's cache system does not work in its favor, ultimately making an array based stack a more effective and more efficient option.



2\. Ownership model for nodes in undo/redo — why not free on undo?

When the program undoes a node, it reverses the graft by putting the old leaf back in. However, it does not free that node that was just undone as its ownership is transferred to the redo stack. In other words, instead of the node being in the tree, it moves to being owned by the edit struct on the redo stack in case the user decides to redo their undo node. If the undo functions freed the node, then the redo stack would contain dangling pointers, and calling redo would crash the program. The only times when a node is freed occurs either when the entire tree must be freed or when the redo stack is freed. Essentially, I chose to not free upon undoing a node and instead transferring its ownership to the redo stack in case the user wishes to redo a node that they just previously undid.

\---



## Section 2 — Complexity Analysis (all four required)

Show the reasoning, not just the answer.

### 2.1 — Amortized cost of a single FrameStack push

For the function fs\_push, there is a best case time complexity of O(1), a worst case time complexity of O(N), and a space complexity of O(N). First, for the best case time complexity, if the realloc branch is not taken, there are no loops and the calloc function simply allocates more memory. Therefore, the time complexity is the sum of the times of individual statements, or O(1). For the worst case time complexity, if the realloc branch is taken, then the realloc function must copy all current elements into the newly allocated buffer, which is occurs in O(N). The rest of the function is still O(1). Therefore O(N+1) simplifies to O(N). However, since the realloc branch is only taken when size is greater than or equal to the capacity, it does not occur that often. Moreover, since it is really only taken when size is a multiple of 5, and then capacity doubles to a multiple of 5, the realloc branch causes 2 copies to push on average. Therefore, the amortized time complexity is O(1). On the other hand, since fs\_push has space usage proportionally realting to N, the space complexity is O(N).

### 2.2 — Hash table average-case lookup

For the function h\_contains, or the lookup function for the has table, we need to consider 3 variables: N = the number of entries in the has table, B = the number of buckets, and K = the number of keys. The while loop at the start of the function traverses a single bucket's chain. Since B is proportional to N (as seen in the function h\_put), the best case time complexity is for the average chain is O(1). However, if every hash corresponds to the same bucket, the chain length is N and the worst case time complexity is O(N). Moreover, since the function does not allocate any extra memory, the same complexity is simply O(1).

### 2.3 — Diagnosis traversal (best, worst, average)

The function run\_diagnosis has a best case time complexity of O(1), a worst case time complexity of O(N^2), and an average time complexity O(N). First, if the user calls the run\_diagnosis function and the they immediately find their solution, they simply went through many assignment calls and calls to functions that have a O(1) complexity, hence a O(1) time complexity. Second, if the user traverses to a deep node and the hash must travel through a long chain in the function h\_put, then since h\_put has a time complexity of O(N) and it is called inside a while loop, the worst case time complexity is O(N^2). Therefore, the average diagnosis traversal must be O(N) time complexity since it lies between the best case and worst case complexities. On the other hand, since the diagnosis traversal depends on the number of frames pushed on the framestack, the space complexity (best, worst, and average) must be O(N).



### 2.4 — `find\_shortest\_path` time and space

The function find\_shortest\_path contains calls to q\_init, q\_enqueue, and q\_dequeue. These 3 called functions are all O(1) since they do not contain loops. They are simply a sequence of if-then statements and assignments, hence, O(1) time complexity. Additionally, since there is a while loop inside find\_shortest\_path, the time complexity for this function must be O(N). On the other hand, inside the find\_shortest\_path function, I allocated memory for a ptr, path1, and path2 array, each of which have a O(N) space complexity since it depends on the solutions inputted when the function is called. Therefore, O(3N) simplifies to a O(N) space complexity. 

\---

## Section 3 — Bugs (two required)

### 3.A — \[get\_input shared buffer]

The function get\_input uses the same buffer each time it is called. As a result, in the function run\_diagnosis, when I called the function get\_input twice to receive the user's new solution and new question, both were originally written to the same buffer. Consequently, when I ran the tech support tool, the learning phase would cause a munmap\_chunk(): invalid pointer crash. To solve this error, I called get\_input for the new Solution, then called strdup to assign the new solution to a new temporary buffer. Then, I called get\_input for the new question and did the same to assign the new question to a new temporary buffer. As a result, I created independent copies of the new solution and new question, which were then later used in the run\_diagnosis function without crashing the tool.

\---

### 3.B — \[Grafting error in Learning Phase]

Every time the learning function of the function run\_diagnosis was called, the g\_root tree root was always replaced. As a result, every time a new question and solution were to be added, the tree would always stay at 3 nodes. This occurred since by the time the learning phase was triggered, all parent frames had already been popped off the frame stack, making it always appear empty. As a result, the program took the g\_root replacement path. To solve the bug, I tracked the parent and wasYes as local variables during traversal and popped them on the frame stack. As a result, the parent nodes were tracked, and when the learning phase was initiated, the proper number of frames were popped and written to the tree. 

\---

## Section 4 — Knowledge Base Reflection (\~100 words)

* How many nodes does your submitted `techsupport.dat` contain?

My submitted file has a total of 53 nodes with 25 question nodes.



* What categories of problems did you teach the program? Give one example question/solution pair for each category.

I taught the program to deal with issues by first classifying if the issue is a network device, audio, display, keyboard, or mouse issues. From there, the program had multiple sub-questions to pin down the exact cause of the issue and prescribe the solution. For example, the program deals with simple and complex solutions such as:

&#x09;- Headphones connected but no audio -> Turn up the headphone's volume

&#x09;- Graphics Card not working when using a monitor connected to a PC -> Reseat the RAM and graphics card



* Look at the tree with `\[V]`.  Are the questions you taught it good distinguishing questions — do they split the remaining candidates roughly in half?  Name one question you would improve and describe what you would replace it with.

The questions I taught the Tech Support tool are good distinguishing questions that split the candidates into first identifying which accessory is experiencing an issue and the diving deeper into possible issues and solutions. As a result, my Tech Support tool is effective and efficient if it were to serve as an actual tech support tool to solve IT issues. One set of questions I would improve relate to the headphones section of the tool. I should have first asked if there was an issue with the user's Bluetooth headphones and separately asked if there was an issue with the user's wired headphones. From there, I could have dived into deeper issues and solutions for each type since they often have different common issues and different solutions. Instead, I asked if there was an issue with the user's Bluetooth headphones and then used the "No" branch to ask if there was an issue with the user's wired headphones.



* Describe one `\[F]ind Path` result.  What were the two solutions, what was the shared path, and did the output match your expectation?

The two solutions I tested the Find Path mode on were "Change the HDMI cable to the correct port" and "Clear up the dust inside your PC and try again." As a result, the tech support told explained how the solutions diverged from the first question "Is the problem with a network device." The result aligns with my expectations since the first solution deals with issues with a monitor and the second issue deals with different issues. 
---

## Section 5 — Reflection (3–5 sentences)

Answer at least two:

* What was the hardest part and why?

The hardest part of creating the Tech Support tool was implementing the save\_tree and load\_tree functions. First, I have never really worked with saving or loading an item when a user first launches a tool. Second, I have never opened and edited a file through C programming. Lastly, the overall logic that each function required were difficult to come up with. These two functions took me the longest to implement and also gave me the most amount of issues and bug. As a result, I first wrote down everything that the functions should do in order. Then, I dissolved each step into its own procedure, essentially turning 7 hard steps into 4-5 steps that I must do 7 different times. This way, it was easier to implement, program, and debug the functions. In the end, I was able to successfully implement both functions and I learned a lot about software design in the process. 

* What did the iterative diagnosis loop teach you about recursion?

When we first learned about recursion in class, it did not make any sense to me. The logic behind recursive programming did not feel intuitive and I chose to stick with iterative programming for the moment. However, when I was implementing TODO 4: count\_nodes, it felt easier to think of each subtree as its own tree that could be passed into the count\_nodes function. As a result, it seemed easier to program count\_nodes using recursion. In fact, count\_nodes is the first function that I have ever programmed using recursion, and it made perfect sense to me. Now, I understand more about recursive programming and when it can have its advantages over iterative programming.

\---

## Section 6 — Time Log

|Date|Hours|What you worked on|
|-|-|-|
|3/30/26|1 Hour|TODOs 1-4|
|3/31/26|1 Hour|TODOs 5-9|
|4/1/26|1 Hour|TODOs 10-14|
|4/2/26|1 Hour|TODOs 15-19|
|4/3/26|1 Hour|TODOs 19-26|
|4/4/25|2 Hours|TODO 27|
|4/5/25|2 Hours|TODO 28|
|4/6/26|2 Hours|30|
|4/7/26 and 4/8/26|2 Hours and 2 Hours|31 and 32-33|
|4/12/26|1 Hour|Writeup|

**Total hours:** 16 Hours

