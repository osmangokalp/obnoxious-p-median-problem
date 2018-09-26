The instances were generated based on the OR-Library instances (Beasley, J. J Oper Res Soc (1990) 41: 1069. doi:10.1057/jors.1990.166).

However, we follow a particular format that we describe as follows. Please see any instance file to follow this explanation.

type=table  <-- Must have always this value
n=200       <-- Number of clients
m=200       <-- Number of facilities
p=25        <-- Value of p

clients = { List of clients identificators. They can be strings. }

facilities = { List of facilities identificators. They can be strings. }

Next, it comes the table that defines the distances between clients and facilities. The distances are stored sorted by facility. That is, the distances from the first facility to all clients are found in the first "{ }" element. Then, the distances of the second facility, and so on.

If we consider that D[i][j] is the distance between client i and facility j, here we have that the table is filled as described next (in this case, n=m=200):

table = {
{ D[0][0] , D[1][0] , D[2][0] , ... , D[199][0]},
{ D[0][1] , D[1][1] , D[2][1] , ... , D[199][1]},
{ D[0][2] , D[1][2] , D[2][2] , ... , D[199][2]},
...
{ D[0][198] , D[1][198] , D[2][198] , ... , D[199][198]},
{ D[0][199] , D[1][199] , D[2][199] , ... , D[199][199]}}
