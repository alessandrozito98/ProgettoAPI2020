# ProgettoAPI2020

Description of the project can be found [here](https://github.com/alessandrozito98/ProgettoAPI2020/blob/master/Specific%20Documents/ProvaFinale2020.pdf).

Specific document can be found [here](https://github.com/alessandrozito98/ProgettoAPI2020/blob/master/Specific%20Documents/ProvaFinale2020specifica.pdf).

## edU, or ed multiplies Undo 
*This project has been developed as part of the "Algoritmi e Principi dell'Informatica" course at [Politecnico di Milano](https://www.polimi.it/).* It has been evaluated "29/30".

A C program that represents a text editor, where you can add, remove, print lines and do undo or redo actions.

### Input Format
The program expects its input from stdio with the following format.

_**NOTE**_: MAX length of a line: 1024. Commands are considered correct.
#### Addition of lines
To add a line to the editor, it is needed to write the lines after the ```ind1,ind2c``` instruction, and after the last line, write a '.'.

Example:
```
1,2c
First Line
Second Line
.

```

#### Deletion of a line
To delete lines, it is needed to use the following format:
```ind1,ind2d```
_**NOTE**_: if you try to delete lines that doesn't exists, the command will have no effect.

#### Printing lines
To print a group of lines, it is needed to use the following format:
```
ind1,ind2p
```
_**NOTE**_: if the line doesn't exists, it will print a '.'.

#### Undo action
In order to go back to a previous version, you can do the following command 
``ind1u``
where ind1 is the number of version that you want to go back.

#### Redo Action
Like undo, the command to do a redo is the following one:
``ind1r``

***Example of the input stream:***
 ```
1,2c
first line
second line
.
2,3c
new second line
third line
.
1,3p
1,1c
new first line.
1,2p
2,2d
4,5p
1,3p
4,5d
1,4p
3u
1,6p
1r
1,3p
q

 ```

***Example of the output stream:***
 ```
 first line
 new second line
 third line
 new first line
 new second line
 .
 .
 new first line
 third line
 .
 new first line
 third line
 .
 .
 first line
 new second line
 third line
 .
 .
 .
 new first line
 new second line
 third line
 ```

### Testing Result

The testing platform was divided in public and private tests. Public tests can be found [here](https://github.com/alessandrozito98/ProgettoAPI2020/blob/master/Public%20Tests/).

However, i also tested my program using a test generator written by me and other students. The original repo can be found [here](https://github.com/AlessioColombo/ProgettoAPI2020_GeneratoreTest).


#### Private Testing Results

###### WriteOnly

Limits: 7,000 s - 3 GiB

* Subtask 2: 0,161 s - 29,1 MiB
* Subtask 3: 0,210 s - 38,7 MiB
* Subtask 4: 0,263 s - 48,3 MiB

###### Bulkreads

Limits: 1,250 s - 240 MiB

* Subtask 2: 0,386 s - 72,3 MiB 
* Subtask 3: 0,509 s - 96,4 MiB
* Subtask 4: 0,626 s - 121 MiB

###### TimeForAChange

Limits: 2,100 s - 540 MiB

* Subtask 2: 0,471 s - 92,2 MiB
* Subtask 3: 0,714 s - 140 MiB
* Subtask 4: 0,952 s - 188 MiB

###### Rollingback

Limits: 2,100 s - 550 MiB

* Subtask 2: 1,223 s - 97,8 MiB
* Subtask 3: 2,095 s - 145 MiB
* Subtask 4: Execution timed out.

###### AlteringHistory

Limits: 2,100 s - 400 MiB

* Subtask 2: 0,613 s - 95,4 MiB
* Subtask 3: 0,856 s - 140 MiB
* Subtask 4: 1,158 s - 192 MiB

###### Rollercoaster

Limits: 2,700 s - 1,03 GiB

* Subtask 2: 0,517 s - 109 MiB
* Subtask 3: 0,780 s - 163 MiB
* Subtask 4: 1,038 s - 217 MiB

###### Lode

Limits: 2,000 s - 340 MiB

* Subtask 1: 0,943 s - 183 MiB
