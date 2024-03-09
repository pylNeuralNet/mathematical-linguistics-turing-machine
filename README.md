# Introduction
Turing Machine is a core part of CS academic syllabus. However, it is not clear why. 
The way this academic module is conducted, it may seem TM is just some obscure ancient monstrous device. 
However, abstract mathematical modeling is at the foundations of theory of computation. So whenever we want to go beyond the current state of technology, that for instance permamently bottlenecks ChatGPTs cap intelligence, we need mathematical modeling to reflect that.

This github contains a classic, very simple implementation by me, but more importantly, later contains a discussion of the magic that is possible due to such abstract mathematical modeling. 

I'm hoping to expand this git with implementation of said magical stuff. Unfortunately the part of the CS syllabus is just the basic TM with no further explanation during classes whatsoever, so if any of the future generations of the students stumble upon this exercise, please consider that there is much more to it and can be important/useful if treated in a certain way.
For context, this is maybe 10% of the entire LM module, and the module itself is just 20% of the semester, which I study part-time.

# Computer Science syllabus - Turing Machine
"Turing machines, first described by Alan Turing in Turing 1936–7, are simple abstract computational devices intended to help investigate the extent and limitations of what can be computed." [src:plato.stanford](https://plato.stanford.edu/entries/turing-machine/)

TM can be defined as a tuple: 
$$M = (Q, \Sigma, \Gamma, \delta, q_0, \Theta, A)$$

where:

$Q$ is a finite set of states.
$\Sigma$ is a finite set of input symbols.
$\Gamma$ is a finite set of tape symbols (tape alphabeth), $\Sigma \subset \Gamma \setminus \lbrace\Theta\rbrace$
$\Theta$ is a symbol sequence termination
$q_0$ initial state $q_0 \in Q$
$\delta$ is the transition function, a mapping from Q×Γ to Q×Γ×D where D is {L,R} representing the direction (left or right) to move the tape head:

$$\delta (q, a) = Q \times \Sigma \to Q \times \Gamma \times \lbraceL,R\rbrace$$

### Classic use cases of a TM
- Recognizing a formal language
    
### A CS Design problem statement
Task: Design a TM that will increment by 5 a natural number consisting of up to 3 digits.

Note: The point being, I suppose, that the task could've been like 'Design a Lexical Analyzer for a Simplified Programming Language'. But as I said, time allocation for this module was rough.

### Model
The model needs to handle the following design issues:
- detect numbers consisting of more digits than 3
- handle writing the carry after a sumation which results in a number higher than 9
- writing the carry in the event of encountering $\Theta$ symbol

My model is a DFA and consists of the following states:
   // q0 - first encountered symbol: either [0-9] or theta. If [0-9], increments by 5 and transitions state to q1 (without carry) or q2 (with carry), if theta - moves head left in an attempt to find a [0-9]

    // q1 - encountered 1 digit by far. writes back the encountered digit because there was no carry. Moves to an analogous state that means 2 digits have been read - one for carry and one without carry (q3/q4), if theta - no more action is taken (-,-,-) because the number has been incremented successfuly and the number has less than 3 digits

    // q2 - encountered 1 digits by far. writes the encountered digit increased by 1 because there was carry. Moves to an analogous state that means 2 digits have been read (q3/q4). if theta - overwrites theta with the carry and no more action is taken (1,-,-)

    // q3 - encountered 2 digits by far. writes back the encountered digit. the rest similarly to q1

    // q4 - encountered 2 digits by far. writes the encountered digit increased by 1 because there was carry. the rest similarly to q2

    // q5 - encountered 3 digits by far. if encountered a digit then moves to q7 which is an end state and no more action is done. if encountered theta then no more action is done -,-,- as the increment of a 3 digits number has succeeded

    // q6 - encountered 3 digits by far. if encountered a digit then moves to q7 which is an end state and no more action is done. if encountered theta then overwrites it with 1 (carry) and no more action is done (1,-,-)

    // q7 - encountered 4 digits, increment is not being performed, no more action is taken (-,-,-). this state is added to be able to discern if the end state is accepting or not.

Therefore, the transition table is:

| State | 0     | 1     | 2     | 3     | 4     | 5     | 6     | 7     | 8     | 9     | #     |
|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|
| q0    | 5,L,q1| 6,L,q1| 7,L,q1| 8,L,q1| 9,L,q1| 0,L,q2| 1,R,q2| 2,L,q2| 3,L,q2| 4,L,q2| -,L,- |
| q1    | 0,L,q3| 1,L,q3| 2,L,q3| 3,L,q3| 4,L,q3| 5,L,q4| 6,L,q4| 7,L,q4| 8,L,q4| 9,L,q4| -,-,- |
| q2    | 1,L,q3| 2,L,q3| 3,L,q3| 4,L,q3| 5,L,q3| 6,L,q4| 7,L,q4| 8,L,q4| 9,L,q4| 0,L,q4| 1,-,- |
| q3    | 0,L,q5| 1,L,q5| 2,L,q5| 3,L,q5| 4,L,q5| 5,L,q6| 6,L,q6| 7,L,q6| 8,L,q6| 9,L,q6| -,-,- |
| q4    | 1,L,q5| 2,L,q5| 3,L,q5| 4,L,q5| 5,L,q5| 6,L,q6| 7,L,q6| 8,L,q6| 9,L,q6| 0,L,q6| 1,-,- |
| q5    | -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,- |
| q6    | -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| 1,-,- |
| q7    | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- |

where '-' is "no instruction", implemented as no action taken by the TM (no change of state).

### Complexity
The poinit being that the complexity of the algorithm can be expressed with the number of steps required by TM to halt.

The number of steps required before the TM head halts is between 1 and N, where N=3, the max number of digits required by the task.

# Discussion
As it's been established in Introduction, TMs generally (or at a good university I guess) can be used to understand the limitations of current hardware architectures. 
(That kind of mathematical modeling might be particularly important to me as I'm working towards architectures that can handle new generations of "Neural Networks" or something beyond that which seems to be a peak structure of millions of years of evolution, apparently good for graph representation of the reality)
In section below some ideas.

### Future commits
It would be fun to commit some borderline insane experimentative elaboration on this basic TM example:
- implement different variants of the model
    - probabilistic
    - quantum
        - Shor's algorithm
        - Grover's algorithm
    - oracle machines
        - universality of NN computation?

But it might not be straight forward why. Monte Carlo techniues are used in some of the AGI paradigms or modern LLM attempts, actually, and probabilistic compilation might bestow upon me some deeper understanding on how that compares to aforementioned peak evolution, and perhaps it would theoretically be capable of reaching a higher level. (and yes, I have asked a question why do that, I've written like 30 pages of a meta-philosophical essay about it)
Furthermore, some stochastic model behaviours could be better modelled by a probabilistic TM. Not sure if it would be the best choice overall, I'm not feeling powerful enough to go against the entire academic Computer Science tradition on such a small detail.

Then the model could be compared to actual results of such NP-hard problem solutions and thus this formal method would turn useful.

Other ideas:
- multi-tape TM
- exploring the limits of computation for problems such as:
    - haltimg problem
    - busy beaver function
    - Church-Turing thesis
- implementing model of computation such as:
    - cellular automata
    - lambda calculus
    - neural networks