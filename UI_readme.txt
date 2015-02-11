------------------------------- Simulation Controls -------------------------------

q:                      Quits simulation

d:                      randomizes the rule set and randomizes the starting colors

x:                      randomizes only the starting colors
                        
c:                      toggles whether the sim draws black and white or colors

v:                      toggles whether to have the background change color if it has not yet been
                        interacted with yet
                        
Left Shift:             pauses the simulation but keeps changing the colors

                        
----------------------------- Board Initializations -----------------------------

space:                  randomly generates a new board with live cell density $density

g:                      generates a board of gliders

e:                      inits $num_gliders/4 squares of side length $density/10 in each quadrant
                        with vertical and horizontal symmetry

a:                      initializes a board with a center square of side length $density/10


------------------------------- Parameter Changes -------------------------------

0 -> 9:                 changes &density from low to high

F1 -> F12:              changes how fast the colors change from fast to slow

Arrow Up/Down:          increases/decreases the number of gliders to generate by 1
                        also changes the number of quadrant dots to generate by 1/4
                      
Arrow Right/Left:       increases/decreases the number of gliders to generate by 4
                        also changes the number of quadrant dots to generate by 1


------------------------------- Genetic Controls -------------------------------

+/=:                    Current ruleset is pretty, add it to the seeds and generate new rules

-/_:                    Current ruleset is not pretty, destroy it and generate new rules