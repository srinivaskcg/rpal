# Rpal-Parser

Construction of Lexical Analyzer and Recursive Descent Parser in C++ for RPAL's Phase Structure Grammar

Programming Language Principles COP 5556

How to use:
----------

Compile : make
Clean   : make cl
Run     : ./p1 [-l] [-ast] file

To test all the programs against rpal:
-------------------------------------

perl difftest.pl -1 "./rpal -ast -noout FILE" -2 "./p1 -ast FILE" -t testDir/ -r results -v 1 -keep



