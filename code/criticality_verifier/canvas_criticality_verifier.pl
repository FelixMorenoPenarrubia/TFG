symbolicOutput(0).  % set to 1 to see symbolic output only; 0 otherwise.
readFile(1). % set to 1 to read from file
verboseOutput(0). % set to 1 to write outer colorings which extend to subgraphs but not whole graph


interiorListSize(5).
numColors(6).
%sizeOuterCycle(7).
%numVertices(10).
%edge(0,1).
%edge(0,7).
%edge(0,6).
%edge(1,2).
%edge(1,7).
%edge(2,3).
%edge(2,8).
%edge(2,7).
%edge(3,4).
%edge(3,9).
%edge(3,8).
%edge(4,5).
%edge(4,9).
%edge(5,6).
%edge(5,9).
%edge(6,7).
%edge(6,8).
%edge(6,9).
%edge(7,8).
%edge(8,9).


%%%%%% Some helpful definitions to make the code cleaner:

vertex(V):-         numVertices(N), NI is N-1, between(0, NI, V).
outerVertex(V):-    sizeOuterCycle(L), LI is L-1, between(0, LI, V).
interiorVertex(V):- vertex(V), sizeOuterCycle(L), between(L, V, V).
color(C):-          numColors(NC), NCI is NC-1, between(0, NCI, C).
contains_colors([]).
contains_colors([C|CL]):- color(C), contains_colors(CL).
coloring(CL):-      numVertices(NV), sizeOuterCycle(SZ), LEN is NV-SZ, length(CL, LEN), contains_colors(CL).
edge([X, Y]):- edge(X, Y).
interiorEdge([X, Y]):- edge([X, Y]), interiorVertex(X), interiorVertex(Y).
interiorEdge([X, Y]):- edge([X, Y]), outerVertex(X), interiorVertex(Y).
completelyInteriorEdge([X, Y]):- edge([X, Y]), interiorVertex(X), interiorVertex(Y).
outerEdge([X, Y]):- edge([X, Y]), outerVertex(X), outerVertex(Y).
outerInterEdge([X, Y]):- edge([X, Y]), outerVertex(X), interiorVertex(Y).
distinctEdges(A, B):- edge(A), edge(B), dif(A,B). 

randomAccess([X|_], 0, X).
randomAccess([_|L], I, Y):- randomAccess(L, NI, Y), I is NI+1.
randomAccessColor(CL, I, X):- sizeOuterCycle(SZ), randomAccess(CL, NI, X), I is NI+SZ .

invalidInteriorColoring(CL):- coloring(CL), completelyInteriorEdge([X, Y]), randomAccessColor(CL, X, CX), randomAccessColor(CL, Y, CY), CX is CY.
validInteriorColoring(CL):- coloring(CL), not(invalidInteriorColoring(CL)).

invalidInteriorColoringWOE(CL, E):- coloring(CL), interiorEdge(E), completelyInteriorEdge([X, Y]), distinctEdges(E, [X, Y]), randomAccessColor(CL, X, CX), randomAccessColor(CL, Y, CY), CX is CY.
validInteriorColoringWOE(CL, E):- coloring(CL), interiorEdge(E), not(invalidInteriorColoringWOE(CL, E)).

outerConflictWithColoring(C, V, CL):- color(C), outerVertex(V), coloring(CL), outerInterEdge([V, X]), randomAccessColor(CL, X, C).
outerConflictWithColoringWOE(C, V, CL, E):- color(C), outerVertex(V), coloring(CL), interiorEdge(E), outerInterEdge([V, X]), distinctEdges(E, [V, X]), randomAccessColor(CL, X, C).



%%%%%%  1. SAT Variables:

satVariable( colorInList(C, V) ):- color(C), interiorVertex(V).
satVariable( outerColoring(C, V, E) ):- color(C), outerVertex(V), interiorEdge(E).
satVariable( satisfiesList(CL) ):- coloring(CL).
satVariable( validColoring(CL, E)):- coloring(CL), interiorEdge(E). %%%includes validity wrt outer coloring
satVariable( validColoringWithoutEdge(CL, E) ):- coloring(CL), interiorEdge(E).
satVariable( satisfiesProperties(CL, E) ):- coloring(CL), interiorEdge(E).

%%%%%%  2. Clause generation:

writeClauses:-  
    assignLists,
    assignOuterColorings,
    validOuterColorings,
    defineSatisfiesList,
    defineValidColoring,
    defineValidColoringWOE,
    defineSatisfiesProperties,
    atLeastOneColSatisfying,
    allColsInvalid,
    
    true,!.
writeClauses:- told, nl, write('writeClauses failed!'), nl,nl, halt.


assignLists:- interiorVertex(V), findall(colorInList(C, V), color(C), Lits), interiorListSize(L), atLeast(L, Lits), fail.
assignLists.

assignOuterColorings:- outerVertex(V), interiorEdge(E), findall(outerColoring(C, V, E), color(C), Lits), exactly(1, Lits), fail.
assignOuterColorings.

validOuterColorings:- outerEdge([X, Y]), color(C), interiorEdge(E), writeClause([-outerColoring(C, X, E), -outerColoring(C, Y, E)]), fail.
validOuterColorings.

defineSatisfiesList:- coloring(CL), findall(colorInList(C, V), randomAccessColor(CL, V, C), Lits), expressAnd(satisfiesList(CL), Lits), fail.
defineSatisfiesList.

defineValidColoring:- validInteriorColoring(CL), interiorEdge(E), findall(-outerColoring(C, V, E), outerConflictWithColoring(C, V, CL), Lits), expressAnd(validColoring(CL, E), Lits), fail.
defineValidColoring:- invalidInteriorColoring(CL), interiorEdge(E), writeClause([-validColoring(CL, E)]), fail.
defineValidColoring. 

defineValidColoringWOE:- interiorEdge(E), validInteriorColoringWOE(CL, E), findall(-outerColoring(C, V, E), outerConflictWithColoringWOE(C, V, CL, E), Lits), expressAnd(validColoringWithoutEdge(CL, E), Lits), fail.
defineValidColoringWOE:- interiorEdge(E), invalidInteriorColoringWOE(CL, E),  writeClause([-validColoringWithoutEdge(CL, E)]), fail.
defineValidColoringWOE. 

defineSatisfiesProperties:- coloring(CL), interiorEdge(E), expressAnd(satisfiesProperties(CL, E), [satisfiesList(CL), validColoringWithoutEdge(CL, E)]), fail.
defineSatisfiesProperties.

atLeastOneColSatisfying:- interiorEdge(E), findall(satisfiesProperties(CL, E), coloring(CL), Lits), atLeast(1, Lits), fail.
atLeastOneColSatisfying.

allColsInvalid:- coloring(CL), interiorEdge(E), writeClause([-satisfiesList(CL), -validColoring(CL, E)]), fail.
allColsInvalid.


%%%%%%  3. DisplaySol: show the solution. Here M contains the literals that are true in the model:

%displaySol(M):-
%    day(D), nl, write('Day '), write(D), write(': '),
%    findall(E-Mod,(member(ed(E,D),M), member(em(E,Mod),M)), L),
%    member(E-Mod,L), write(' event('), write(E), write(')-mod('), write(Mod), write(') '), fail.
%displaySol(M):-nl,
%    moderator(Mod), nl, findall(D,(member(ed(E,D),M), member(em(E,Mod),M)), L),
%    write('Moderator '), write(Mod), write( ' works: '), sort(L,L1), write(L1), fail.
displaySolVerbose(M):-
    nl, write('List assignment: '), interiorVertex(V), nl, 
    write(V), write(': '), 
    findall(C, member(colorInList(C, V), M), L), member(CC, L), write(CC), write(' '), fail.
displaySolVerbose(M):-
    nl, nl, write('Outer colorings for each subgraph:'), nl, nl, 
    interiorEdge([X, Y]), write('-----'), nl, write('Outer coloring for subgraph without edge '), write(X), write('-'), write(Y), nl, 
    findall([V, C], member(outerColoring(C, V, [X, Y]), M), L),  member([VV, CC], L), write(VV), write(': '), write(CC), nl, fail.
displaySolVerbose(_).

displaySol(M):- verboseOutput(1), displaySolVerbose(M), fail.
displaySol(_).

%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Everything below is given as a standard library, reusable for solving 
%    with SAT many different problems.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Express that Var is equivalent to the disjunction of Lits:
expressOr( Var, Lits ):- symbolicOutput(1), write( Var ), write(' <--> or('), write(Lits), write(')'), nl, !. 
expressOr( Var, Lits ):- member(Lit,Lits), negate(Lit,NLit), writeClause([ NLit, Var ]), fail.
expressOr( Var, Lits ):- negate(Var,NVar), writeClause([ NVar | Lits ]),!.

%% expressOr(a,[x,y]) genera 3 clausulas (como en la Transformación de Tseitin):
%% a == x v y
%% x -> a       -x v a
%% y -> a       -y v a
%% a -> x v y   -a v x v y

% Express that Var is equivalent to the conjunction of Lits:
expressAnd( Var, Lits) :- symbolicOutput(1), write( Var ), write(' <--> and('), write(Lits), write(')'), nl, !. 
expressAnd( Var, Lits):- member(Lit,Lits), negate(Var,NVar), writeClause([ NVar, Lit ]), fail.
expressAnd( Var, Lits):- findall(NLit, (member(Lit,Lits), negate(Lit,NLit)), NLits), writeClause([ Var | NLits]), !.


%%%%%% Cardinality constraints on arbitrary sets of literals Lits:

exactly(K,Lits):- symbolicOutput(1), write( exactly(K,Lits) ), nl, !.
exactly(K,Lits):- atLeast(K,Lits), atMost(K,Lits),!.

atMost(K,Lits):- symbolicOutput(1), write( atMost(K,Lits) ), nl, !.
atMost(K,Lits):-   % l1+...+ln <= k:  in all subsets of size k+1, at least one is false:
      negateAll(Lits,NLits),
      K1 is K+1,    subsetOfSize(K1,NLits,Clause), writeClause(Clause),fail.
atMost(_,_).

atLeast(K,Lits):- symbolicOutput(1), write( atLeast(K,Lits) ), nl, !.
atLeast(K,Lits):-  % l1+...+ln >= k: in all subsets of size n-k+1, at least one is true:
      length(Lits,N),
      K1 is N-K+1,  subsetOfSize(K1, Lits,Clause), writeClause(Clause),fail.
atLeast(_,_).

negateAll( [], [] ).
negateAll( [Lit|Lits], [NLit|NLits] ):- negate(Lit,NLit), negateAll( Lits, NLits ),!.

negate( -Var,  Var):-!.
negate(  Var, -Var):-!.

subsetOfSize(0,_,[]):-!.
subsetOfSize(N,[X|L],[X|S]):- N1 is N-1, length(L,Leng), Leng>=N1, subsetOfSize(N1,L,S).
subsetOfSize(N,[_|L],   S ):-            length(L,Leng), Leng>=N,  subsetOfSize( N,L,S).


%%%%%% main:

main:- readFile(0), exec.
main(_):- readFile(0), exec.
main(F):- readFile(1), consult(F), exec.

exec:-  symbolicOutput(1), !, writeClauses, halt.   % print the clauses in symbolic form and halt
exec:-  initClauseGeneration,
        tell(clauses), writeClauses, told,          % generate the (numeric) SAT clauses and call the solver
        tell(header),  writeHeader,  told,
        numVars(N), numClauses(C),
        write('Generated '), write(C), write(' clauses over '), write(N), write(' variables. '),nl,
        shell('cat header clauses > infile.cnf',_),
        write('Calling solver....'), nl,
        shell('picosat -v -o model infile.cnf', Result),  % if sat: Result=10; if unsat: Result=20.
        treatResult(Result),!.

treatResult(20):- write('UNSATISFIABLE'), nl, write('!!!'), nl, halt.
treatResult(10):- write('Satisfiable'), nl, see(model), symbolicModel(M), seen, displaySol(M), nl,nl,halt.
treatResult( _):- write('cnf input error. Wrote anything strange in your cnf?'), nl,nl, halt.
    

initClauseGeneration:-  %initialize all info about variables and clauses:
        retractall(numClauses(   _)),
        retractall(numVars(      _)),
        retractall(varNumber(_,_,_)),
        assert(numClauses( 0 )),
        assert(numVars(    0 )),     !.

writeClause([]):- symbolicOutput(1),!, nl.
writeClause([]):- countClause, write(0), nl.
writeClause([Lit|C]):- w(Lit), writeClause(C),!.
w(-Var):- symbolicOutput(1), satVariable(Var), write(-Var), write(' '),!. 
w( Var):- symbolicOutput(1), satVariable(Var), write( Var), write(' '),!. 
w(-Var):- satVariable(Var),  var2num(Var,N),   write(-), write(N), write(' '),!.
w( Var):- satVariable(Var),  var2num(Var,N),             write(N), write(' '),!.
w( Lit):- told, write('ERROR: generating clause with undeclared variable in literal '), write(Lit), nl,nl, halt.


% given the symbolic variable V, find its variable number N in the SAT solver:
:-dynamic(varNumber / 3).
var2num(V,N):- hash_term(V,Key), existsOrCreate(V,Key,N),!.
existsOrCreate(V,Key,N):- varNumber(Key,V,N),!.                            % V already existed with num N
existsOrCreate(V,Key,N):- newVarNumber(N), assert(varNumber(Key,V,N)), !.  % otherwise, introduce new N for V

writeHeader:- numVars(N),numClauses(C), write('p cnf '),write(N), write(' '),write(C),nl.

countClause:-     retract( numClauses(N0) ), N is N0+1, assert( numClauses(N) ),!.
newVarNumber(N):- retract( numVars(   N0) ), N is N0+1, assert(    numVars(N) ),!.

% Getting the symbolic model M from the output file:
symbolicModel(M):- get_code(Char), readWord(Char,W), symbolicModel(M1), addIfPositiveInt(W,M1,M),!.
symbolicModel([]).
addIfPositiveInt(W,L,[Var|L]):- W = [C|_], between(48,57,C), number_codes(N,W), N>0, varNumber(_,Var,N),!.
addIfPositiveInt(_,L,L).
readWord( 99,W):- repeat, get_code(Ch), member(Ch,[-1,10]), !, get_code(Ch1), readWord(Ch1,W),!. % skip line starting w/ c
readWord(115,W):- repeat, get_code(Ch), member(Ch,[-1,10]), !, get_code(Ch1), readWord(Ch1,W),!. % skip line starting w/ s
readWord(-1,_):-!, fail. %end of file
readWord(C,[]):- member(C,[10,32]), !. % newline or white space marks end of word
readWord(Char,[Char|W]):- get_code(Char1), readWord(Char1,W), !.
%========================================================================================
